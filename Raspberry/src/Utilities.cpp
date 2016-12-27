#include "Utilities.h"

std::vector<std::string> Utilities::split(std::string s, std::string delimiter)
{
	std::cout << "Utilities Split" << std::endl;
	std::vector<std::string> v;
	size_t pos = 0;
	size_t posPrec = 0;
	int delimiterSize = delimiter.length();
	std::cout << "Split While begin" << std::endl;
	while ((pos = s.find(delimiter,pos)) != std::string::npos) {
		v.push_back(s.substr(posPrec+delimiterSize, pos));
	}
	std::cout << "Split While end" << std::endl;
	v.push_back(s.substr(posPrec, s.length()-1));
	return v;
}