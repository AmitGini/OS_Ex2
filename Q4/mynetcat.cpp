#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>  // for fork(), execv(), dup2(), getopt 
#include <cstring>   // for strdup()
#include <cstdlib>   // for exit()
#include <sys/wait.h> // for waitpid()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

#include "tcp_server.hpp"
#include "tcp_client.hpp"
#include "udp_server.hpp"
#include "udp_client.hpp"


using namespace std;

// Utility function to close file descriptors and free memory
void closeAndFree(int inputFd, int outputFd, int serverSocket, int clientSocket, vector<char*>& execArgs);

// Execute the command with input and output redirection if specified
int executeCommand(const string& command, const string& inputSource, const string& outputDestination);

// Handle input and output redirection and set up the server
int handle_input(int& inputFd, int& serverSocket, const string& inputSource, const string& outputDestination);

// Handle output redirection and connect client to a server
int handle_output(int& outputFd, int& clientSocket, const string& outputDestination);

// Forward declaration of utility functions
int redirectIO(int inputFd, bool redirectInput, int outputFd, bool redirectOutput);

int chat(int inputFd, int outputFd);

bool eFlag = false;

// Main application entry
int main(int argc, char* argv[]) {
    string command, inputSource, outputDestination, timeout;
    bool bFlag = false, iFlag = false, oFlag;

    // Parsing command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "e:i:o:b:t:")) != -1) {
        switch (opt) {
            case 'e':
                command = optarg;
                eFlag = true;
                break;
            case 'i':
                inputSource = optarg;
                iFlag = true;
                break;
            case 'o':
                outputDestination = optarg;
                oFlag = true;
                break;
            case 'b':
                inputSource = outputDestination = optarg;
                bFlag = true;
                break;
            case 't':
                timeout = optarg;
                break;
            default:
                cerr << "Usage: mync -e <\"command\"> [-i <input source>] [-o <output destination>] [-b <bi-directional source>]" << endl;
                exit(EXIT_FAILURE);
        }
    }
    
    if(!timeout.empty()){
        int time = stoi(timeout);
        alarm(time);
    }

    if(iFlag && bFlag || oFlag && bFlag){
        cerr << "Cannot use -b flag with -i or -o flags" << endl;
        exit(EXIT_FAILURE);
    }

    // Execute the command
    executeCommand(command, inputSource, outputDestination);
    return 0;
}

// Execute the command with input and output redirection if specified
int executeCommand(const string& command, const string& inputSource, const string& outputDestination) {
    // Setup execution environment
    stringstream commandStream(command);
    string executable;
    vector<string> arguments;
    commandStream >> executable;
    string arg;

    while (commandStream >> arg) 
    {
        arguments.push_back(arg);
    }

    vector<char*> execArgs;
    execArgs.push_back(strdup(executable.c_str())); // Executable name
    
    for (const string& arg : arguments) 
    {
        execArgs.push_back(strdup(arg.c_str())); // Command arguments
    }
    
    execArgs.push_back(nullptr); // Null terminate for execv

    int inputFd = STDIN_FILENO, outputFd = STDOUT_FILENO; //fd for input and output
    int serverSocket = -1, clientSocket = -1; // Server and client sockets
    int inputStatus = 0, outputStatus = 0; // Status of input and output redirection

    if (!inputSource.empty()) 
    {
        inputStatus = handle_input(inputFd, serverSocket, inputSource, outputDestination);
        if(inputStatus == -1)
        {
            closeAndFree(inputFd, outputFd, serverSocket, clientSocket, execArgs);
            exit(EXIT_FAILURE);
        }
    }

    if (!outputDestination.empty())
    {
        outputStatus = handle_output(outputFd, clientSocket, outputDestination);
        if(outputStatus == -1)
        {
            closeAndFree(inputFd, outputFd, serverSocket, clientSocket, execArgs);
            exit(EXIT_FAILURE);
        }
    }

    if(eFlag){
        // Fork and execute
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            closeAndFree(inputFd, outputFd, serverSocket, clientSocket, execArgs);
            exit(EXIT_FAILURE);
        }
        
        else if (pid == 0) { // Child process
            execv(execArgs[0], execArgs.data()); // Execute the command with the arguments given
            perror("execv failed"); // execv only returns if it fails 
            closeAndFree(inputFd, outputFd, serverSocket, clientSocket, execArgs);
            exit(EXIT_FAILURE);
        } 
        
        else // Parent process
        { 
            int status;
            waitpid(pid, &status, 0); // Wait for the child process to complete
            
            if (WIFEXITED(status)) // Check if the child process exited successfully
            {
                cout << "Child process exited with status " << WEXITSTATUS(status) << endl;
            } 
            else 
            {
                cout << "Child process did not exit successfully" << endl;
            }
            
            closeAndFree(inputFd, outputFd, serverSocket, clientSocket, execArgs);
            return status;
        }
    }
    else
    {
        int status = chat(inputFd, outputFd);
        closeAndFree(inputFd, outputFd, serverSocket, clientSocket, execArgs);
        return status;
    }
}

// Handle input redirection and set up the server
int handle_input(int& inputFd, int& serverSocket, const string& inputSource, const string& outputDestination){
    if (inputSource.substr(0, 4) == "TCPS") 
    {
        int port = stoi(inputSource.substr(4));
        serverSocket = startTCPServer(port);

        if(serverSocket == -1){
            cerr << "Failed to start TCP server" << endl;
            return -1;
        }
        
        inputFd = acceptTCPConnection(serverSocket);
        if (inputFd == -1) {
            cerr << "Failed to start TCP server" << endl;
            return -1;
        }
    }

    else if (inputSource.substr(0, 4) == "UDPS") 
    {
        int port = stoi(inputSource.substr(4));
        inputFd = startUDPServer(port);

        if(inputFd == -1){
            cerr << "Failed to start UDP Server" << endl;
            return -1;
        }
    }

    if(inputSource == outputDestination) /* Output redirection to the client (-b Flag) */
    { 
        redirectIO(inputFd, true, inputFd, true); //redirect the input to the server and output to the client of the server
    } 
    else 
    { 
        redirectIO(inputFd, true, STDOUT_FILENO, true); //redirect only the input from the client to the server, output in standard output
    }

    return 0;
}

// Handle output redirection and connect client to a server
int handle_output(int& outputFd, int& clientSocket,const string& outputDestination)
{
    /* TCP-Client: redirection output to the client (-o Flag) */
    if (outputDestination.substr(0, 4) == "TCPC") 
    {
        size_t commaPos = outputDestination.find(',');
        string hostname = outputDestination.substr(4, commaPos - 4);
        int port = stoi(outputDestination.substr(commaPos + 1));
        outputFd = startTCPClient(hostname, port);
        if (outputFd == -1) 
        {
            cerr << "Failed to connect to TCP server" << endl;
            return -1;
        }
    }

    else if(outputDestination.substr(0, 4) == "UDPC")
    {
        size_t commaPos = outputDestination.find(',');
        string hostname = outputDestination.substr(4, commaPos - 4);
        int port = stoi(outputDestination.substr(commaPos + 1));
        outputFd = startUDPClient(hostname, port);
        if(outputFd == -1)
        {
            cerr << "Failed to connect to UDP server" << endl;
            return -1;
        }
    }

    redirectIO(STDIN_FILENO, true, outputFd, true); // Redirect the output to the client
    return 0;
}

// Utility function to redirect standard input and output
int redirectIO(int inputFd, bool redirectInput, int outputFd, bool redirectOutput) 
{
    if (redirectInput && inputFd != STDIN_FILENO)  // Redirect input from the inputFd
    {
        if (dup2(inputFd, STDIN_FILENO) < 0) 
        {
            perror("Failed to redirect standard input");
            return -1;
        }
    }

    if (redirectOutput && outputFd != STDOUT_FILENO)  // Redirect output to the outputFd
    {
        if (dup2(outputFd, STDOUT_FILENO) < 0) 
        {
            perror("Failed to redirect standard output");
            return -1;
        }
        if(dup2(outputFd, STDERR_FILENO) <0)
        {
            perror("Failed to redirect standard error output");
            return -1;
        }
    }

    return 0;
}

//print to the stdout from the inputFd and send to outpudFd from the stdin
int chat(int inputFd, int outputFd)
{
    fd_set readFds;
    int maxFd = inputFd;

    // if the outputFd is greater than the inputFd, set the maxFd to the outputFd
    while (1) 
    {
        FD_ZERO(&readFds); // clear the readFds set
        FD_SET(inputFd, &readFds);  // add the inputFd to the readFds set
        FD_SET(STDIN_FILENO, &readFds); // add the stdin to the readFds set

        // wait for any of the file descriptors to have data to read
        if (select(maxFd + 1, &readFds, NULL, NULL, NULL) == -1) 
        {
            perror("select Failed");
            return -1;
        }

        // check if the inputFd has data to read (from the input source)
        if (inputFd != STDIN_FILENO && FD_ISSET(inputFd, &readFds)) 
        {
            char buffer[1024];
            int bytes_read = read(inputFd, buffer, sizeof(buffer));  // read from the inputFd
            if (bytes_read == -1) 
            {
                perror("read Failed");
                return -1;
            }
            if (bytes_read == 0) 
            {
                break;
            }
            // write to the stdout
            if (write(STDOUT_FILENO, buffer, bytes_read) == -1) 
            {
                perror("write Failed");
                return -1;
            }
        }

        // check if the stdin has data to read (keyboard input)
        if (FD_ISSET(STDIN_FILENO, &readFds) && outputFd != STDOUT_FILENO) 
        {
            char buffer[1024];
            int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));  // read from the stdin
            if (bytes_read == -1) 
            {
                perror("read Failed");
                return -1;
            }
            if (bytes_read == 0) 
            {
                break;
            }
            if (write(outputFd, buffer, bytes_read) == -1) 
            {
                perror("write Failed");
                return -1;
            }
        }
    }

    return 0;
}

// Utility function to close file descriptors and free memory
void closeAndFree(int inputFd, int outputFd, int serverSocket, int clientSocket, vector<char*>& execArgs)
{
    if (inputFd != STDIN_FILENO) 
    {
        close(inputFd);
    }
    
    if (outputFd != STDOUT_FILENO) 
    {
        close(outputFd);
    }
    
    if (serverSocket != -1) 
    {
        close(serverSocket);
    }
    
    if (clientSocket != -1) 
    {
        close(clientSocket);
    }

    for (char* arg : execArgs) 
    {
        free(arg);
    }
    fflush(stdout);
}
