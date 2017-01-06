// Scaffold.cpp

#include "provided.h"
#include <vector>
#include <iostream>
#include <stack>
using namespace std;

class ScaffoldImpl
{
public:
	ScaffoldImpl(int nColumns, int nLevels);
	int cols() const;
	int levels() const;
	int numberEmpty() const;
	int checkerAt(int column, int level) const;
	void display() const;
	bool makeMove(int column, int color);
	int undoMove();
private:
	vector<vector<int>> scaffold;
	stack<int> moves;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
	scaffold.resize(nLevels);
	if (nColumns <= 0 || nLevels <= 0)
	{
		cerr << "Invalid Scaffold size" << endl;
		exit(1);
	}
	for (int i = 0; i < nLevels; i++) {
		scaffold[i].resize(nColumns);
	}
	for (size_t i = 0; i < scaffold.capacity(); i++) {
		for (size_t j = 0; j < scaffold[i].capacity(); j++)
			scaffold[i][j] = VACANT;
	}
}

int ScaffoldImpl::cols() const
{
	return scaffold[0].capacity();
}

int ScaffoldImpl::levels() const
{
	return scaffold.capacity();
}

int ScaffoldImpl::numberEmpty() const
{
	int empty = 0;
	for (size_t i = 0; i < scaffold.capacity(); i++) {
		for (size_t j = 0; j < scaffold[i].capacity(); j++)
			if (scaffold[i][j] == -1)
				empty++;
	}
	return empty;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
	return scaffold[level - 1][column - 1];
}

void ScaffoldImpl::display() const
{
	for (size_t i = scaffold.capacity() - 1; i<scaffold.capacity() && i >= 0; i--)
	{
		cout << "|";
		for (size_t j = 0; j < scaffold[i].capacity(); j++)
		{
			switch (scaffold[i][j]) {
			case VACANT:
				cout << " ";
				break;
			case BLACK:
				cout << "B";
				break;
			case RED:
				cout << "R";
				break;
			}
			cout << "|";
		}
		cout << endl;
	}
	cout << "+";
	for (size_t i = 0; i < scaffold[0].capacity(); i++)
	{
		cout << "-+";
	}
	cout << endl;

}

bool ScaffoldImpl::makeMove(int column, int color)
{
	if ((size_t)column > scaffold[0].capacity()||column <1)
		return false;
	for (size_t i = 0; i < scaffold.capacity(); i++)
	{
		if (scaffold[i][column - 1] == VACANT)
		{
			scaffold[i][column - 1] = color;
			moves.push(column);
			return true;
		}
	}
	return false;  //  This is not always correct; it's just here to compile
}


int ScaffoldImpl::undoMove()
{
	if (moves.empty())
		return 0;
	int mostRecentMoveCol = moves.top();
	moves.pop();
	for (int i = scaffold.capacity() - 1; i >= 0; i--)
	{
		if (scaffold[i][mostRecentMoveCol - 1] != VACANT)
		{
			scaffold[i][mostRecentMoveCol - 1] = VACANT;
			return mostRecentMoveCol;
		}
	}
	return 0;  //  This is not always correct; it's just here to compile
}

//******************** Scaffold functions *******************************

// These functions simply delegate to ScaffoldImpl's functions.
// You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
	m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
	delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
	m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
	if (this != &rhs)
	{
		Scaffold temp(rhs);
		swap(m_impl, temp.m_impl);
	}
	return *this;
}

int Scaffold::cols() const
{
	return m_impl->cols();
}

int Scaffold::levels() const
{
	return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
	return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
	return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
	m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
	return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
	return m_impl->undoMove();
}
