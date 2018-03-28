#include "Structuri.h"

using namespace structuri;


//Functii clasa Stiva

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

//Functii clasa Stiva_count

Stiva_count::Stiva_count()
{
	Stiva::Stiva();
	e_top_i = new Element_i();
}

Stiva_count::~Stiva_count()
{
	int c2 = count;
	Stiva::~Stiva();

	if (c2 == 0)
		return;
	Element_i *next;
	while (c2)
	{
		next = e_top_i->urm;
		delete e_top_i;
		e_top_i = next;
		--c2;
	}
}

int Stiva_count::top_i()
{
	if (count)
		return e_top_i->val;
	return -1;
}

bool Stiva_count::push(std::string val)
{
	Stiva::push(val);

	Element_i *aux = new Element_i();
	aux->val = 0;
	aux->urm = e_top_i;
	e_top_i = aux;
	return true;
}

bool Stiva_count::pop()
{
	if (Stiva::pop())
	{
		Element_i *aux = e_top_i;
		e_top_i = e_top_i->urm;
		delete aux;
		return true;
	}
	return false;
}

int Stiva_count::top_add()
{
	e_top_i->val++;
	return e_top_i->val;
}