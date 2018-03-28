#include "Functii.h"
#include "Structuri.h"
#include <algorithm>
#include <sstream>
#include <iostream>

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
	if (c >= '0' || c <= '9')
		return 5;
	return 4;
}

string dtos(double d)
{
	//Folosesc libraria <sstream> pentru tipul de date ostringstream.
	//ostringstream = output string stream.
	//"Afisez" variabila double in stream.
	//Folosesc functiile predefinite pentru formatul ostringstream si returnez stringul creat din str_s.

	ostringstream str_s;
	str_s << d;
	return str_s.str();
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
	
	//Daca sirul nu contine niciun operator atunci el este un operand al operatiei de la nivelul anterior si il returnam.
	if (exp.find('+') == string::npos && exp.find('-') == string::npos && exp.find('*') == string::npos && exp.find('/') == string::npos)
		return (minusfirst ? "-0" : "") + exp;

	//Altfel inseamna ca sirul dat nu este un operand ci o intreaga expresie. Analizam expresia.
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
	//Stiva op va retine operatorii ramasi de adaugat in forma prefixata.
	structuri::Stiva op;
	//Rezultatul transformarii va fi retinut in variabila res.
	string res = "";

	//Inversam expresia initiala.
	reverse(exp.begin(),exp.end());

	//Parcurgem expresia initiala de la coada la cap.
	//Daca ignoram regulile uzuale de ordine a operatiilor
	//primul operator din forma prefixata va fi ultimul din forma infixata. 
	for (int i = 0; i < exp.length(); i++)
	{
		//Paranteza inchisa se adauga in stiva. Urmatorii operatori gasiti in parcurgerea sirului
		//vom sti ca se gasesc intr-o paranteza.
		if (exp[i] == ')')
			op.push(")");
		//La gasirea parantezei deschise stim ca am parcurs toata paranteza si am retinut operatorii din paranteza in stiva
		//elementul top al stivei fiind primul operator din paranteza, elementul imediat urmator al 2-lea operator din paranteza, s.a.m.d..
		else if (exp[i] == '(')
		{
			//Daca am parcurs toata paranteza inseamna ca operanzii din aceasta au fost deja adaugati la rezultat
			//si vom adauga operatorii din paranteza.
			while (op.top()[0] != ')')
			{
				//Adaugarea se face prin atasarea elementului top al stivei la inceputul expresiei.
				//Astfel primul operator din res va fi cel a carui operatie se realizeaza ultima in evaluarea expresiei in forma infixata.
				res = op.top() + " " + res;
				//Stergem operatorul proaspat adaugat intru-cat el a fost tratat si inclus in rezultat.
				op.pop();
			}
			//Stergem operatorul ')' pentru a sti ca am terminat tratarea expresiei din paranteze. Operatorii '(' si ')' nu apar oricum in forma prefixata.
			op.pop();
		}
		//Daca rangul caracterului citit este 4, el este un caracter ce face parte din numele operandului, asadar il lipim la inceputul rezultatului.
		//Lipirea caracter cu caracter a numelui unui operand la inceputul expresiei prefixate inverseaza ordinea caracterelor din numele operandului,
		//insa expresia pe care lucram este inversa expresiei initiale, deci prin dubla inversare operandul va aparea in forma prefixata
		//exact cu acelasi nume ca in expresia infixata initiala.
		else if (Rank(exp[i]) == 4)
			res = exp[i] + res;
		//Daca gasim un operator cu rangul mai mare sau egal decat ultimul operator gasit
		//atunci il adaugam in stiva pentru ca el nu are prioritate in fata ultimului operator gasit.
		else if (Rank(exp[i]) >= Rank(op.top()[0]))
		{
			op.push(string() + exp[i]);
			res = " " + res;
		}
		//Altfel inseamna ca la momentul actual caracterul evaluat nu este nici paranteza, nici parte a numelui unui operand.
		//Caracterul evaluat este un operator cu rang mai mic decat ultimul operator inregistrat in stiva. (adica un + sau -, in timp ce stiva contine un * sau /) 
		else
		{
			//De aceea vom lipii la raspuns toti operatorii din stiva pana la intalnirea unuia cu rang mai mic sau egal
			//decat operatorul evaluat din expresia initiala.
			while (Rank(op.top()[0])>Rank(exp[i]))
			{
				res = op.top() + " " + res;
				op.pop();
			}
			//Apoi vom memora in stiva operatorul din expresia initiala.
			op.push(string() + exp[i]);
			res = " " + res;
		}
	}

	//La finalizarea parcurgerii expresiei initiale toti operanzii sunt prezenti in stringul raspuns.
	//Daca mai exista operatori ce nu au fost inca inclusi in raspuns, atunci ei sunt extrasi din stiva si lipiti rand pe rand la raspuns.
	while (op.Count())
	{
		res = op.top() + " " + res;
		op.pop();
	}

	//Se returneaza rezultatul
	return res;
}

double Evaluare(string exp)
{
	//Stiva in care retinem operanzii.
	structuri::Stiva opz;

	//Stiva in care retinem operatorii.
	//Fiecare element al stivei are un contor
	//Ce va numara cati dintre operanzii operatiei
	//Se gasesc in forma lor finala in stiva opz.
	structuri::Stiva_count op;
	string aux = "";
	for (int i = 0; i < exp.length(); i++)
	{
		//Daca este o cifra, atunci caracterul face parte dintr-un operand.
		//Adaugam cifra in variabila ce retine operandul curent.
		if (Rank(exp[i]) == 5)
		{
			aux = aux + exp[i];
			continue;
		}

		//Daca este un operator, caracterul este adaugat in stiva op.
		if (Rank(exp[i]) == 2 || Rank(exp[i]) == 3)
		{
			op.push(string() + exp[i]);
			continue;
		}
		//Daca intalnim un spatiu, iar aux contine un operand
		//atunci am terminat de citit operandul si il adaugam in stiva opz.
		if (exp[i] == ' ' && aux.length())
		{
			opz.push(aux);
			//Structura formei prefixate presupune ca operandul proaspat adaugat
			//este folosit in operatia din varful stivei op.
			//Asadar marcam finalizarea gasirii unui operand prin incrementarea 
			//contorului elementului din varful stivei op.
			op.top_add();
			aux.clear();
		}

		//Cat timp operatia din varful listei de operatori are contorul = 2
		//ii cunoastem operanzii si deci vom calcula rezultatul operatiei.
		//Rezultatul va fi un operand pentru operatia de pe elementul imediat sub varf.
		while (op.top_i() == 2)
		{
			//Extragem cei 2 operanzi
			double auxd,opz1,opz2;
			opz2 = stod(opz.top());
			opz.pop();
			opz1 = stod(opz.top());
			opz.pop();

			//Calculam in auxd rezultatul operatiei
			switch (op.top()[0])
			{
				case '+':
					auxd = opz1 + opz2;
					break;
				case '-':
					auxd = opz1 - opz2;
					break;
				case '*':
					auxd = opz1 * opz2;
					break;
				default:
					auxd = opz1 / opz2;
			}
			//Retinem rezultatul in stiva.
			opz.push(dtos(auxd));

			//Eliminam operatia mai sus efectuata din stiva.
			op.pop();

			//Incrementam contorul noii operatii din varful stivei,
			//Pentru ca rezultatul stocat mai sus in opz este un operator
			//pentru aceasta operatie.
			op.top_add();
		}
	}

	//In forma prefixata ultimele caractere din expresie vor constitui cu siguranta un operand.
	//Asadar, dupa parcurgerea expresiei, vom stoca in stiva si ultimul operand si vom 
	//efectua rand pe rand operatiile ramase in stiva op.
	opz.push(aux);
	op.top_add();
	aux.clear();
	//TO:DO Scurteaza codul aici
	while (op.top_i() == 2)
	{
		double auxd, opz1, opz2;
		opz2 = stod(opz.top());
		opz.pop();
		opz1 = stod(opz.top());
		opz.pop();
		switch (op.top()[0])
		{
		case '+':
			auxd = opz1 + opz2;
			break;
		case '-':
			auxd = opz1 - opz2;
			break;
		case '*':
			auxd = opz1 * opz2;
			break;
		default:
			auxd = opz1 / opz2;
		}
		opz.push(dtos(auxd));
		op.pop();
		op.top_add();
	}

	//Stiva op va contine in final un element cu string = "" si contor = 1.
	//contor != 2 asa ca se va iesi din bucla while, iar stiva opz va contine rezultatul final.
	return stod(opz.top());
}

void Citire(string &exp)
{
	cout << "Expresie: ";
	getline(cin, exp);
	if (exp[0] == '-')
		exp = "0" + exp;
}