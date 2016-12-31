#include "Utilities.h"

std::vector<std::string> Utilities::split(std::string s, std::string delimiter)
{
	std::vector<std::string> v;
	int delimiterSize = delimiter.length();
	size_t pos = 0-delimiterSize;
	size_t posPrec = 0-delimiterSize;
	while ((pos = s.find(delimiter,pos+delimiterSize)) != std::string::npos) {
		if (posPrec+delimiterSize < pos) {
			v.push_back(s.substr(posPrec+delimiterSize, pos-(posPrec+delimiterSize)));
		}
		posPrec = pos;
	}
	v.push_back(s.substr(posPrec+delimiterSize, s.length()-1));
	return v;
}