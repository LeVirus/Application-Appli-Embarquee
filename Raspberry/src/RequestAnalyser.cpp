#include "RequestAnalyser.h"

void RequestAnalyser::analyse(string request)
{
	vector<string> requestSplitted = Utilities::split(request,";");
	if (requestSplitted.at(0) == "deplacement") {
		if (requestSplitted.at(1) == "forward") {
			
		}
		else if (requestSplitted.at(1) == "reverse") {
			
		}
	}
}