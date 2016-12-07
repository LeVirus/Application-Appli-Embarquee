#include <iostream>

using namespace std;

class LogStream : public streambuf
{    
	private:
		string buffer;
		 
	protected:
		int overflow(int ch) override;
};