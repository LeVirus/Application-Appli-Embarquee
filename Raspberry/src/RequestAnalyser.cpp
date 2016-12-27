#include "RequestAnalyser.h"

void RequestAnalyser::analyse(string request)
{
	vector<string> requestSplitted = Utilities::split(request,";");
	std::cout << "SPLITTED 0 = " << requestSplitted.at(0)  << "END" << std::endl;
	if (requestSplitted.at(0) == "movement") {
		std::cout << "SPLITTED 1 = " << requestSplitted.at(1)  << "END" << std::endl;
		if (requestSplitted.at(1) == "move") {
			std::cout << "Move RequestAnalyser" << std::endl;
			RobotControl::move();
		}
		if (requestSplitted.at(1) == "stop") {
			RobotControl::move();
		}
		else if (requestSplitted.at(1) == "forward") {
			RobotControl::forward();
		}
		else if (requestSplitted.at(1) == "reverse") {
			RobotControl::reverse();
		}
	}
}