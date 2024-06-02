#include "tttfunctions.hpp"

using namespace std;

// This function check if the input string 1) has exactly 9 characters 2) Contains only digits from '1' to '9'
// Each digit appears only once.
bool validateInput(const string& inputStrategy) {
    if (inputStrategy.size()!=9) return false;
    vector<bool> seen(9, false);
    for (char c : inputStrategy) {
        if(c < '1' || c > '9') {
            return false;
        }
        int index = c - '1';
        if (seen[index]) return false;
        seen[index] = true;
    }
    return true;
}

// This function prints the board. 
void printBoard(const vector<char>& board) {
    for (int i = 0; i < 9; i++) {
        cout << board[i];
        if (i % 3 == 2) cout << endl; // if its the end of row, new line
        else cout << ' '; // else, space
    }
    cout << endl;
}

// This function check if player is the winner by pass all possible winning combinations
bool checkWinner(const vector<char>& board, char player) {
    const vector<vector<int>> win_conditions = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };

    for (const auto& condition : win_conditions) {
        if (board[condition[0]] == player && board[condition[1]] == player && board[condition[2]] == player) {
            return true;
        }
    }
    return false;
}

int getMove(const vector<char>& board, const vector<int>& numStrategy) {
    for (int position : numStrategy) {
        if (board[position] == '_') return position;
    }
    return numStrategy.back();
}