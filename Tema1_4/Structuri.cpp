#include "Structuri.h"

using namespace personal;



Stiva::Stiva()
{
	e_top = nullptr;
	count = 0;
}

Stiva::~Stiva()
{
	if (count == 0)
		return;
	Element *next;
	while (count)
	{
		next = e_top->urm;
		delete e_top;
		e_top = next;
		--count;
	}
}

std::string Stiva::top()
{
	if (count)
		return e_top->val;
	 return "•"; // •
}

bool Stiva::push(std::string val)
{
	Element *aux = new Element();
	aux->val = val;
	aux->urm = e_top;
	e_top = aux;
	++count;
	return true;
}

bool Stiva::pop()
{
	if (count)
	{
		--count;
		Element *aux = e_top;
		e_top = e_top->urm;
		delete aux;
		return true;
	}
	return false;
}

int Stiva::Count()
{
	return count;
}