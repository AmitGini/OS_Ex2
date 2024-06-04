#ifndef TTTFUNCTIONS_HPP
#define TTTFUNCTIONS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // for count() function
#include <limits> // For numeric_limits

using namespace std;

bool validateInput(const string& inputStrategy);

void printBoard(const vector<char>& board);

bool checkWinner(const vector<char>& board, char player);

int getMove(const vector<char>& board, const vector<int>& numStrategy);

#endif // TTTFUNCTIONS_HPP
