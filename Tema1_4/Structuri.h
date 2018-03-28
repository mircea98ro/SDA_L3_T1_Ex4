#pragma once
#include <string>

namespace structuri
{
	class Stiva
	{
	private:

		struct Element
		{
			std::string val;
			Element* urm;
		};

		Element* e_top;

	protected:
		unsigned int count;

	public:
		Stiva();
		~Stiva();

		std::string top();
		bool push(std::string);
		bool pop();
		int Count();
	};

	class Stiva_count : public Stiva
	{
	private:
		struct Element_i
		{
			int val;
			Element_i* urm;
		};

		Element_i *e_top_i;

	public:
		Stiva_count();
		~Stiva_count();

		int top_i();
		bool push(std::string);
		bool pop();
		int top_add();
	};
}