#include "Utilities.h"

std::vector<std::string> Utilities::split(std::string s, std::string delimiter)
{
	std::cout << "Utilities Split" << std::endl;
	std::vector<std::string> v;
	int delimiterSize = delimiter.length();
	size_t pos = 0-delimiterSize;
	size_t posPrec = 0-delimiterSize;
	std::cout << "Split While begin" << std::endl;
	while ((pos = s.find(delimiter,pos+delimiterSize)) != std::string::npos) {
		std::cout << "Split While step 1, pos = " << pos << std::endl;
		if (posPrec+delimiterSize < pos) {
			std::cout << "Split While step 2, pos = " << pos << std::endl;
			v.push_back(s.substr(posPrec+delimiterSize, pos));
			std::cout << "Split While step 3, pos = " << pos << std::endl;
		}
		posPrec = pos;
	}
	std::cout << "Split While end" << std::endl;
	v.push_back(s.substr(posPrec+delimiterSize, s.length()-1));
	return v;
}