#include "RequestAnalyser.h"

void RequestAnalyser::analyse(string request)
{
	vector<string> requestSplitted = Utilities::split(request,";");
	if (requestSplitted.at(0) == "movement") {
		if (requestSplitted.at(1) == "move") {
			RobotControl::move();
		}
		if (requestSplitted.at(1) == "stop") {
			RobotControl::stop();
		}
		else if (requestSplitted.at(1) == "forward") {
			RobotControl::forward();
		}
		else if (requestSplitted.at(1) == "reverse") {
			RobotControl::reverse();
		}
	}
}