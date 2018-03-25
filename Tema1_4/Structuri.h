#pragma once
#include <string>

namespace personal
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
		unsigned int count;

	public:
		Stiva();
		~Stiva();

		std::string top();
		bool push(std::string);
		bool pop();
		int Count();
	};
}