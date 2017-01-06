// Game.cpp

#include "provided.h"
#include "support.h"
#include <iostream>
using namespace std;

class GameImpl
{
public:
	GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
	bool completed(int& winner) const;
	bool takeTurn();
	void play();
	int checkerAt(int c, int r) const;
private:
	Scaffold s;
	Player *r, *b;
	int nToWin, currPlayer, lastCol;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black) : s(nColumns, nLevels)
{
	r = red;
	b = black;
	nToWin = N;
	currPlayer = RED;
	lastCol = 1;
}

bool GameImpl::completed(int& winner) const
{

	if (isGameWon(s, lastCol, nToWin, winner))
		return true;
	return false;

}





bool GameImpl::takeTurn()
{
	//completed only changes curr player if it returns true
	if (currPlayer == RED)
	{
		cout << "Red Players move" << endl;
		lastCol = r->chooseMove(s, nToWin, RED);
		s.makeMove(lastCol, RED);
		currPlayer = BLACK;

	}
	else
	{
		cout << "Black Players moves" << endl;
		lastCol = b->chooseMove(s, nToWin, BLACK);
		s.makeMove(lastCol, BLACK);
		currPlayer = RED;
	}
	if (!completed(currPlayer))	
		return true;
	else
		return false;
	//if the game is complete after the turn return false. curr player is the winner and has been changed
}

void GameImpl::play()
{
	if (r->isInteractive())	//in the beginning, if the human is playing, display an empty scaffold, otherwise just let the computer play
		s.display();
	while (takeTurn())	//if you can take a turn, keep taking a turn
	{
		s.display();
		cout << endl;
		cout << endl;
		cout << endl;
		if (!r->isInteractive() && !b->isInteractive())
		{
			cin.ignore(); //if both the players arent human, let the user press enter for the next move
		}
	}
	s.display();		//display the winning board
	switch (currPlayer)	//once the game is over, find out who won
	{
	case RED:
		cout << "red won!" << endl;
		break;
	case BLACK:
		cout << "black won!" << endl;
		break;
	case TIE_GAME:
		cout << "Tie!" << endl;
		break;
	}
}

int GameImpl::checkerAt(int c, int r) const
{
	return s.checkerAt(r, c);
}

//******************** Game functions *******************************

// These functions simply delegate to GameImpl's functions.
// You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
	m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
	delete m_impl;
}

bool Game::completed(int& winner) const
{
	return m_impl->completed(winner);
}

bool Game::takeTurn()
{
	return m_impl->takeTurn();
}

void Game::play()
{
	m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
	return m_impl->checkerAt(c, r);
}
