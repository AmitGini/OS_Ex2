# Operation System - Assignment 2

## Overview
This repository contains several exercises related to operating systems, implemented in C++. The exercises include creating a tic-tac-toe game and a custom version of the netcat utility. Each exercise is contained in its own folder with a Makefile for building and running the code.

## Assignments

### Assignment 1: Tic-Tac-Toe (ttt)
The goal is to create the worst AI for a tic-tac-toe game that follows a predefined strategy.

#### Instructions
1. Navigate to the `ttt` directory:
   ```sh
   cd ttt
   ```
2. Build the project using the provided Makefile:
   ```sh
   make
   ```
3. Run the program with a 9-digit strategy number:
   ```sh
   ./ttt 123456789
   ```

### Assignment 2: Basic Netcat (mync)
Create a custom version of netcat that forwards input and output to a specified program.

#### Instructions
1. Navigate to the `mync` directory:
   ```sh
   cd mync
   ```
2. Build the project using the provided Makefile:
   ```sh
   make
   ```
3. Run the program with the `-e` parameter to execute another program:
   ```sh
   ./mync -e "ttt 123456789"
   ```

### Assignment 3: Netcat with TCP Support (mync)
Extend the custom netcat to support TCP connections.

#### Instructions
1. Navigate to the `mync` directory:
   ```sh
   cd mync
   ```
2. Build the project using the provided Makefile:
   ```sh
   make
   ```
3. Run the program with TCP server/client parameters:
   - Start a TCP server:
     ```sh
     ./mync -e "ttt 123456789" -i TCPS4050
     ```
   - Start a TCP client:
     ```sh
     ./mync -e "ttt 123456789" -o TCPClocalhost,4455
     ```

### Assignment 4: Netcat with UDP Support (mync)
Extend the custom netcat to support UDP connections.

#### Instructions
1. Navigate to the `mync` directory:
   ```sh
   cd mync
   ```
2. Build the project using the provided Makefile:
   ```sh
   make
   ```
3. Run the program with UDP server/client parameters:
   - Start a UDP server:
     ```sh
     ./mync -e "ttt 123456789" -i UDPS4050 -t 10
     ```
   - Start a UDP client:
     ```sh
     ./mync -e "ttt 123456789" -o UDPClocalhost,4455
     ```

### Assignment 6: Netcat with Unix Domain Sockets (mync)
Extend the custom netcat to support Unix domain sockets.

#### Instructions
1. Navigate to the `mync` directory:
   ```sh
   cd mync
   ```
2. Build the project using the provided Makefile:
   ```sh
   make
   ```
3. Run the program with Unix domain socket parameters:
   - Start a Unix domain socket server:
     ```sh
     ./mync -e "ttt 123456789" -i UDSSD/path/to/socket
     ```
   - Start a Unix domain socket client:
     ```sh
     ./mync -e "ttt 123456789" -o UDSCD/path/to/socket
     ```

## Additional Information
- It's recommended to use `getopt` for parameter handling.
- Use `alarm(2)` for implementing timeouts.

## Contact
For any questions or issues, please contact [Amit Gini](https://github.com/AmitGini).

---

Feel free to provide additional details or modifications to this structure.
