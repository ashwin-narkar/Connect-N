// Player.cpp

#include "provided.h"
#include "support.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
using namespace std;



class Timer
{
public:
	Timer()
	{
		start();
	}
	void start()
	{
		m_time = std::chrono::high_resolution_clock::now();
	}
	double elapsed() const
	{
		std::chrono::duration<double, std::milli> diff =
			std::chrono::high_resolution_clock::now() - m_time;
		return diff.count();
	}
private:
	std::chrono::high_resolution_clock::time_point m_time;
};




class HumanPlayerImpl
{
public:
	int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
public:
	int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
	int chooseMove(const Scaffold& s, int N, int color);
	int determineBestComputerMove(Scaffold& s, int&bestMove, int nToWin, int depth, int color, int lastMove);
	int determineBestHumanMove(Scaffold& s, int&bestMove, int nToWin, int depth, int color, int lastMove);
private:
	Timer timer;
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	if (s.numberEmpty() == 0)
		return -1;
	cout << "Please enter the column to make your move" << endl;
	int col = 0;
	cin >> col;
	cout << endl;
	while (!isValidPos(s,s.levels(),col) || s.checkerAt(col, s.levels()) != VACANT)
	{
		if (!isValidPos(s, s.levels(), col))
		{
			cout << "Please enter a valid column number" << endl;
			cin >> col;
			cout << endl;
		}
		else
		{
			cout << "This column is full. You cannot move here" << endl;
			cin >> col;
			cout << endl;
		}	
	}
	
	return col;  //  This is not always correct; it's just here to compile
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	if (s.numberEmpty() == 0)
		return -1;
	for (int i = 1; i <= s.cols(); i++)
		if (s.checkerAt(i, s.levels()) == VACANT)
			return i;
	return 0;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	if (s.numberEmpty() == 0)
		return -1;
	int result = 0;
	Scaffold x(s);	//only copy the scaffold once per turn. so that it is not copied in each recursive call
	timer.start();	//start the timer
	determineBestComputerMove(x, result, N, 1, color, 1);
	return result;
	//  This is not always correct; it's just here to compile
}

int SmartPlayerImpl::determineBestComputerMove(Scaffold& s, int&bestMove, int nToWin, int depth, int color, int lastMove)
{
	//cerr << "Comp Depth: " << depth << endl;
	int numFreeCols = 0;
	int vacantCol = 1;
	for (int i = 1; i <= s.cols(); i++)
	{
		if (s.checkerAt(i, s.levels()) == VACANT)
		{
			numFreeCols++;
			vacantCol = i; //if there is only 1 freecol, then vacant col will have that number.
		}
	}
	if (numFreeCols == 1) //if there are more than 1 free columns then vacant col wont matter
	{
		if (s.makeMove(vacantCol, color))	//make the move in the only vacant column and return the result
		{
			//s.display();
			bestMove = vacantCol;
			int result = rateScaffold(s, color, vacantCol, nToWin);
			s.undoMove();
			return result;
		}
	}
	// if there are multiple moves possible go through each move
	vector<int> outcome;
	outcome.resize(s.cols());
	//if there are more than two moves, you can only win
	for (int i = 1; i <= s.cols(); i++)		//go through each column
	{
		if (s.makeMove(i, color))	//if you can make the move at the column make it
		{
			//s.display();
			//check all possible human moves at the location.
			int result = rateScaffold(s, color, i, nToWin);
			if (result == 1) //if the move won, return right away
			{
				bestMove = i;
				s.undoMove();	
				return result-depth;	 //smaller depth with make the value more positive
			}
			else
			{
				int newColor;
				if (color == RED)
					newColor = BLACK;
				else
					newColor = RED;
				
				if (timer.elapsed() <= 4900)
					outcome[i - 1] = determineBestHumanMove(s, bestMove, nToWin, depth + 1, newColor, i);
				else
				{
					s.undoMove();
					bestMove = i;
					//cerr << "Depth: " << depth << endl;
					int result = rateScaffold(s, color, i, nToWin);
					//cerr << "Comp Result: " << result << endl;
					return result-depth;
				}
				
				//outcome[i - 1] = determineBestHumanMove(s, bestMove, nToWin, depth + 1, newColor, i);
			}
			s.undoMove();
		}
		else
			outcome[i - 1] = -999;
	}
	int maxOutcome = outcome[0];
	bestMove = 1;
	for (int i = 1; i < s.cols(); i++)
	{
		if (outcome[i] > maxOutcome)
		{
			maxOutcome = outcome[i];
			bestMove = i + 1;
		}
	}
	return maxOutcome;
}

int SmartPlayerImpl::determineBestHumanMove(Scaffold& s, int&bestMove, int nToWin, int depth, int color, int lastMove)
{
	//cerr << "Human Depth: " << depth << endl;
	int newColor;
	if (color == RED)
		newColor = BLACK;
	else
		newColor = RED;
	//base case is only one possible column to move into
	int numFreeCols = 0;
	int vacantCol = 1;
	for (int i = 1; i <= s.cols(); i++)
	{
		if (s.checkerAt(i, s.levels()) == VACANT)
		{
			numFreeCols++;
			vacantCol = i; //if there is only 1 freecol, then vacant col will have that number.
		}
	}
	if (numFreeCols == 1) //if there are more than 1 free columns then vacant col wont matter
	{
		if (s.makeMove(vacantCol, color))	//make the move in the only vacant column and return the result
		{
			//s.display();
			bestMove = vacantCol;
			int result = rateScaffold(s, newColor, vacantCol, nToWin);
			s.undoMove();
			return result;
		}
	}

	// if there are multiple moves possible go through each move
	vector<int> outcome;
	outcome.resize(s.cols());
	//if there are more than two moves, you can only win
	for (int i = 1; i <= s.cols(); i++)		//go through each column
	{
		if (s.makeMove(i, color))	//if you can make the move at the column make it
		{
			//s.display();
			//check all possible human moves at the location.
			int result = rateScaffold(s, newColor, i, nToWin);
			if (result == -1) //if the move won, return right away
			{
				bestMove = i;
				s.undoMove();	//undo the move
				return depth + result;	//return the depth plus our result. this means that at more depths the result will be more postive which the human doesnt want
			}
			else
			{					
				if (timer.elapsed() <= 4900)		//if 5 seconds have not passed, continue recursion
					outcome[i - 1] = determineBestComputerMove(s, bestMove, nToWin, depth + 1, newColor, i);
				else
				{
					s.undoMove();	//undo the most recent move
					bestMove = i;	//otherwise just return the move that was just checked
					//cerr << "Depth: " << depth << endl;
					int result = rateScaffold(s, color, i, nToWin);	//return the result of the scaffold at the point to the upper level
					//cerr << "Human Result: " << result << endl;
					return result+depth;
				}
			}
			s.undoMove();	//undo the move after making it before checking the next possible move
		}
		else
			outcome[i - 1] = 999;	//if you cannot make the move, the result is some bignumber bc we want to mimize and we dont want the function to select this outcome ever
	}
	int minOutcome = outcome[0];
	bestMove = 1;
	for (int i = 1; i< s.cols(); i++)
	{
		if (outcome[i] < minOutcome)		//find the most minimum  outcome
		{
			minOutcome = outcome[i];
			bestMove = i + 1;	//make move takes i+1 bc the cols start at 1
		}
	}
	return minOutcome;	//return this outcome
}


//******************** Player derived class functions *************************

// These functions simply delegate to the Impl classes' functions.
// You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
	: Player(nm)
{
	m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
	delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
	return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
	: Player(nm)
{
	m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
	delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
	return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
	: Player(nm)
{
	m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
	delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
	return m_impl->chooseMove(s, N, color);
}
