#include "Functii.h"
#include "Structuri.h"
#include <algorithm>

using namespace std;

char Rank(char c)
{
	if (c == '•')
		return 0;
	if (c == '(' || c == ')')
		return 1;
	if (c == '+' || c == '-')
		return 2;
	if (c == '*' || c == '/')
		return 3;
	return 4;
}

int poz = 0;

string Solve_Recursiv(string exp)
{
	//pos_op = Pozitia ultimului operator + sau - aflat la nivelul in care lucram
	//pos_op2 = Pozitia ultimului operator * sau / aflat la nivelul in care lucram
	int pos_op = -1, pos_op2 = -1;

	//countp = Contor numar de paranteze deschise
	int countp = 0;

	//opz = String operanzi
	//op = String operator
	string opz = "", op = "";


	int i = 1;
	//Daca primul caracter este o paranteza verificam daca aceasta paranteza se inchide pe ultima pozitie a sirului
	if (exp[0] == '(')
	{
		countp++;
		while (i < exp.length() && countp)
		{
			if (exp[i] == '(')
				countp++;
			else if (exp[i] == ')')
				countp--;
			i++;
		}
		//Daca ajungem la capatul sirului inseamna ca paranteza este redundanta si o eliminam
		if (i == exp.length())
			exp = exp.substr(1, i - 2);
	}

	//Verificam daca primul caracter este -. Daca este marcam evenimentul si eliminam caracterul pentru a continua evaluarea.
	bool minusfirst =(exp[0] == '-');
	if (minusfirst)
		exp = exp.substr(1,exp.length()-1);
	
	//Daca sirul nu contine niciun operator atunci el este un operand al operatiei de la nivelul anterior
	if (exp.find('+') == string::npos && exp.find('-') == string::npos && exp.find('*') == string::npos && exp.find('/') == string::npos)
		return (minusfirst ? "-0" : "") + exp;

	//Altfel inseamna ca sirul dat nu este un operand ci o intreaga expresie. Analizam expresia
	//Vom cauta ultimul semn de + sau - din sir, dar care nu este inclus intr-o paranteza.
	//In paralel vom cauta ultimul semn de * sau / din sir, dar care nu este inclus intr-o paranteza.
	i = 0;
	while (i < exp.length())
	{
		if (exp[i] == '(')
			countp++;
		else if (exp[i] == ')')
			countp--;
		else 
			if (countp == 0)
			{
				if (Rank(exp[i]) == 2)
					pos_op = i;
				if (Rank(exp[i]) == 3)
					pos_op2 = i;
			}
		i++;
	}
	//Daca gasim semne de + sau - care satisfac conditia, atunci ultimul dintre ele este operatorul operatiei de care ne ocupam la acest nivel al executiei. (Operatia prioritara)
	//Daca nu gasim astfel de semne, atunci inseamna ca operatia este * sau /. Ultima astfel de operatie gasita, care satisface conditia este operatorul de care ne ocupam la acest nivel al executiei.
	if (pos_op == -1)
		pos_op = pos_op2;

	//Operatorul este cel de pe pozitia data. Operanzii se obtin prin reluarea algoritmului pe expresia din stanga operatorului, respectiv pe cea din dreapta.
	op = op + exp[pos_op] + " ";
	opz = Solve_Recursiv(exp.substr(0, pos_op)) + " " + Solve_Recursiv(exp.substr(pos_op + 1, exp.length() - pos_op - 1));
	if (minusfirst)
		opz = "-0 " + opz;
	//Returnam operatia in forma prefixata.
	return op + opz;
}


string Solve_Iterativ(string exp)
{
	personal::Stiva op, opz;
	string res = "";

	reverse(exp.begin(),exp.end());

	for (int i = 0; i < exp.length(); i++)
	{
		if (exp[i] == ')')
			op.push(")");
		else if (exp[i] == '(')
		{
			while (op.top()[0] != ')')
			{
				res = op.top() + " " + res;
				op.pop();
			}
			op.pop();
		}
		else if (Rank(exp[i]) == 4)
			res = exp[i] + res;
		else if (Rank(exp[i]) >= Rank(op.top()[0]))
		{
			op.push(string() + exp[i]);
			res = " " + res;
		}
		else
		{
			while (Rank(op.top()[0])>Rank(exp[i]))
			{
				res = op.top() + " " + res;
				op.pop();
			}
			op.push(string() + exp[i]);
			res = " " + res;
		}
	}

	while (op.Count())
	{
		res = op.top() + " " + res;
		op.pop();
	}

	return res;
}