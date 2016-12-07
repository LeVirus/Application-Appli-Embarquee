#include "LogStream.h"

int LogStream::overflow(int ch)
{
	buffer.push_back((char) ch);
	if (ch == '\n') {
		// End of line, write to logging output and clear buffer.
		std::cout << buffer << std::endl; 
		buffer.clear();
	}
	 
	return ch;
	 
	//  Return traits::eof() for failure.
}