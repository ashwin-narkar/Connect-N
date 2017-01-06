#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;

#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
	/*
Scaffold s(5, 4);
s.makeMove(1,BLACK);
s.makeMove(1, BLACK);
s.undoMove();
s.display();
cout << s.checkerAt(1, 1);
string name;
cout << "What's your name?  ";
getline(cin, name);
HumanPlayer hp("Ash");
HumanPlayer bp("Bad Player");
//s.display();
cout << endl;
cout << endl;
cout << endl;
for (int j = 0; j < 5; j++) {
int i = bp.chooseMove(s, 4, RED);
s.makeMove(i, RED);
cout << endl;
cout << endl;
cout << endl;
s.display();
}
while (1) {
int i = hp.chooseMove(s, 4, RED);
if (i == -1)
break;
s.makeMove(i, RED);
s.display();*/
	HumanPlayer hp("Ash");
SmartPlayer sp("Computer");
//BadPlayer bp2("bp2");
Game g1(6, 4, 4, &hp, &sp);
g1.play();
//cout << endl << "This time I'll go first." << endl;
//Game g2(4, 3, 3, &bp, &hp);
//g2.play();

}