#ifndef UTILITIES_H
#define UTILITIES_H

#include <string.h>
#include <iostream>
#include <vector>

class Utilities
{
	public:
		static std::vector<std::string> split(std::string s, std::string delimiter);
};

#endif