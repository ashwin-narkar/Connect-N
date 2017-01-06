
#include <vector>
#include <iostream>
#include "support.h"

using namespace std;

bool isGameWon(const Scaffold& s, int lastCol, int nToWin, int& winner)
{
	int inRow = 1;
	for (int j = s.levels(); j >= 2; j--)	//checks vertical in a row
		if (s.checkerAt(lastCol, j - 1) != VACANT && s.checkerAt(lastCol, j) == s.checkerAt(lastCol, j - 1))
		{
			inRow++;
			if (inRow == nToWin)	//inRow doesnt account for first two in a row. so 
			{
				winner = s.checkerAt(lastCol, j);
				//cerr << "Vertical Winner" << endl;
				return true;
			}
		}
	inRow = 1;
	//check horizontal
	//find first row not vacant. from the top of of lastcol
	int rowToCheck;
	for (rowToCheck = s.levels(); rowToCheck >= 1; rowToCheck--)
		if (s.checkerAt(lastCol, rowToCheck) != VACANT)
			break;
	int leftCol = lastCol;
	//if the checker is in the middle. move the position we're checking all the way to the left. and then go right
	if (isValidPos(s, rowToCheck, lastCol - 1) && s.checkerAt(lastCol, rowToCheck) == s.checkerAt(lastCol - 1, rowToCheck))	//if there is a valid position at the left
	{
		leftCol = lastCol - 1;
		while (isValidPos(s, rowToCheck, leftCol - 1) && s.checkerAt(leftCol, rowToCheck) == s.checkerAt(leftCol - 1, rowToCheck))
		{
			leftCol--;

		}
	}
	while (isValidPos(s, rowToCheck, leftCol + 1) && s.checkerAt(leftCol, rowToCheck) != VACANT && s.checkerAt(leftCol, rowToCheck) == s.checkerAt(leftCol + 1, rowToCheck) && inRow != nToWin)
	{
		inRow++;
		leftCol++;
	}

	if (inRow == nToWin) {
		winner = s.checkerAt(leftCol, rowToCheck);
		//cerr << "Horizontal Winner" << endl;
		return true;
	}
	inRow = 1;
	//diagonal positive sloping
	for (rowToCheck = s.levels(); rowToCheck >= 1; rowToCheck--)
		if (s.checkerAt(lastCol, rowToCheck) != VACANT)
			break;
	if (rowToCheck == 0)
		rowToCheck++;
	leftCol = lastCol;
	//if the checker is in the middle. move the position we're checking all the way to the down and left. and then go up and right
	if (isValidPos(s, rowToCheck - 1, lastCol - 1) && s.checkerAt(lastCol, rowToCheck) == s.checkerAt(lastCol - 1, rowToCheck - 1))	//if there is a valid position at the left
	{
		while (isValidPos(s, rowToCheck - 1, leftCol - 1) && s.checkerAt(leftCol, rowToCheck) == s.checkerAt(leftCol - 1, rowToCheck - 1))
		{
			leftCol--;
			rowToCheck--;
		}
	}
	//keep moving up diagonally and checking that the values match
	while (isValidPos(s, rowToCheck + 1, leftCol + 1) && s.checkerAt(leftCol, rowToCheck) != VACANT && s.checkerAt(leftCol, rowToCheck) == s.checkerAt(leftCol + 1, rowToCheck + 1) && inRow != nToWin)
	{
		inRow++;
		leftCol++;
		rowToCheck++;
	}
	if (inRow == nToWin) {
		winner = s.checkerAt(leftCol, rowToCheck);
		//cerr << "Diagonal Winner" << endl;
		return true;
	}

	inRow = 1;
	//diagonal negative sloping
	for (rowToCheck = s.levels(); rowToCheck >= 1; rowToCheck--)
		if (s.checkerAt(lastCol, rowToCheck) != VACANT)
			break;
	if (rowToCheck == 0)
		rowToCheck++;
	leftCol = lastCol;
	//if the checker is in the middle. move the position we're checking all the way to the down and left. and then go up and right
	if (isValidPos(s, rowToCheck + 1, lastCol - 1) && s.checkerAt(lastCol, rowToCheck) == s.checkerAt(lastCol - 1, rowToCheck + 1))	//if there is a valid position at the left
	{
		while (isValidPos(s, rowToCheck + 1, leftCol - 1) && s.checkerAt(leftCol, rowToCheck) == s.checkerAt(leftCol - 1, rowToCheck + 1))
		{
			leftCol--;
			rowToCheck++;
		}
	}
	//keep going down diagonally and check if values match
	while (isValidPos(s, rowToCheck - 1, leftCol + 1) && s.checkerAt(leftCol, rowToCheck) != VACANT && s.checkerAt(leftCol, rowToCheck) == s.checkerAt(leftCol + 1, rowToCheck - 1) && inRow != nToWin)
	{
		inRow++;
		leftCol++;
		rowToCheck--;
	}
	if (inRow == nToWin) {
		winner = s.checkerAt(leftCol, rowToCheck);
		//cerr << "Negative Diagonal Winner" << endl;
		return true;
	}
	if (s.numberEmpty() == 0)
	{
		winner = TIE_GAME;
		return true;
	}
	return false;
}

bool isValidPos(const Scaffold& s, const int& r, const int& c)
{
	if (r > 0 && r <= s.levels())
		if (c > 0 && c <= s.cols())
			return true;
	return false;
}

int determineGoodComputerMove(Scaffold s, int nToWin, int color, int col)
{
	int winner = TIE_GAME;
	int result = 0;
	if (s.makeMove(col, color))
	{

	}
	s.undoMove();
	return result;
}

int rateScaffold(const Scaffold& s, int color, int lastMove, int winning)
{
	int winner = 9999;
	if (isGameWon(s, lastMove, winning, winner))
	{
		if (winner == TIE_GAME)
			return 0;
		if (winner == color)
			return 1;
		else
			return -1;
	}
	else
		return winner;
}




/*
01	   function minimax(node, depth, maximizingPlayer)
02     if depth = 0 or node is a terminal node
03         return the heuristic value of node

04     if maximizingPlayer
05         bestValue := −∞
06         for each child of node
07             v := minimax(child, depth − 1, FALSE)
08             bestValue := max(bestValue, v)
09         return bestValue

10     else    (* minimizing player *)
11         bestValue := +∞
12         for each child of node
13             v := minimax(child, depth − 1, TRUE)
14             bestValue := min(bestValue, v)
15         return bestValue


Node is the scaffold after every move. depth is just  0 at the beginning
*/