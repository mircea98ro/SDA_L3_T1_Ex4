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