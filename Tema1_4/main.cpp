#include "Structuri.h"
#include "Functii.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	string opt;
	char optiune;
	string exp;

	cout << "Legenda optiuni:\n\t"
		<< "0 - Opreste programul.\n\t"
		<< "1 - INFIX->PREFIX Recursiv. \n\t"
		<< "2 - INFIX->PREFIX Iterativ. \n\t"
		<< "3 - Evaluare expresie sub forma PREFIX. \n\n";

	while (true)
	{
		cout << "Optiunea? : ";
		getline(cin, opt);
		if (opt.length() > 1)
			optiune = 4;
		else
			optiune = opt[0];

		if (optiune == '0')
			return 0;

		optiune -= '0';

		switch (optiune)
		{
		case 1:
			Citire(exp);
			//Rezolvare Recursiva
			cout << "Rezolvare Recursiva:\n" << Solve_Recursiv(exp) << '\n';
			break;
		case 2:		
			Citire(exp);
			//Rezolvare Iterativa
			cout << "Rezolvare Iterativa:\n" << Solve_Iterativ(exp) << '\n';
			break;
		case 3:
			Citire(exp);
			//Evaluare expresie data
			cout << "Evaluare forma prefixata:\n" << Evaluare(exp) << '\n';
			break;
		default:
			cout << "Optiune incorecta. Reintroduceti.\n\n";
			break;
		}
	}

	return 0;
}