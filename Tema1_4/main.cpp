#include "Structuri.h"
#include "Functii.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	string exp, expr;

	getline(cin, exp);
	if (exp[0] == '-')
		exp = "0" + exp;
	expr = exp;
	reverse(expr.begin(), expr.end());

	//Rezolvare Recursiva
	cout << "Rezolvare Recursiva:\n" << Solve_Recursiv(exp) << '\n';
	
	//Rezolvare Iterativa
	cout << "Rezolvare Iterativa:\n" << Solve_Iterativ(exp) << '\n';

	//Evaluare expresie data
	//TODO

	return 0;
}