#include "tttfunctions.hpp"
using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Error: invalid command, try: ./ttt <InputStrategy>" << endl;
        return 1;
    }

    string strategy = argv[1];
    if (!validateInput(strategy)) {
        cout << "Error: Strategy must have exactly 9 unique digits from '1' to '9'."<< endl;
        return 1;
    }

    vector <int> numStrategy;
    for (char i : strategy) {
        numStrategy.push_back(i -'1');
    }

    vector<char> board(9, '_'); // Init new empty board

    while (true) {
        // AI's move
        int move = getMove(board, numStrategy);
        board[move] = 'X';
        cout << "Computer next move: "<<move + 1 << endl; // Adding 1 to convert the 0-based index to a 1-based index for the user.
        printBoard(board);
        cout.flush(); //  Ensures the output is immediately displayed.

        // Check if AI won
        if (checkWinner(board, 'X')) {
            cout << "I win" << endl;
            break;
        }
        // Check for draw
        if (count(board.begin(), board.end(), '_') == 0) {
            cout << "DRAW" << endl;
            break;
        }

        // Player's move
        int playerMove;
        cout << "Please enter your next move: "<<endl;
        cin >> playerMove;
        // Validate input
        while (cin.fail() || playerMove < 1 || playerMove > 9) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a number between 1 and 9: ";
            cin >> playerMove;
        }
        playerMove -= 1; // Match 0-based index for the board

        // Validate player's move
        while (board[playerMove] != '_') {
            cout << "Error: Invalid move. Try again: ";
            cin >> playerMove;
            playerMove -= 1; // Match 0-based index for the board
        }
        board[playerMove] = 'O';
        printBoard(board);
        cout.flush(); // Ensure the board state is displayed immediately

        // Check if player won
        if (checkWinner(board, 'O')) {
            cout << "I lost" << endl;
            break;
        }

        // Check for draw again
        if (count(board.begin(), board.end(), '_') == 0) {
            cout << "DRAW" << endl;
            break;
        }
    }
    
    return 0;
}
    


