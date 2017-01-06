#ifndef SUPPORT_H
#define SUPPORT_H
#include "provided.h"

bool isGameWon(const Scaffold& s, int lastCol, int nToWin, int& winner);
bool isValidPos(const Scaffold& s, const int& r, const int& c);
int determineGoodComputerMove(Scaffold s, int nToWin, int color, int col);
int rateScaffold(const Scaffold& s, int color, int lastMove, int winning);

#endif // !SUPPORT_H


