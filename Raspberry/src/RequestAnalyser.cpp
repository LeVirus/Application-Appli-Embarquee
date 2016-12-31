#include "RequestAnalyser.h"

void RequestAnalyser::analyse(string request)
{
	vector<string> requestSplitted = Utilities::split(request,";");
	if (requestSplitted.at(0) == "movement") {
		if (requestSplitted.at(1) == "move") {
			RobotControl::move();
		}
		else if (requestSplitted.at(1) == "stop") {
			RobotControl::stop();
		}
		else if (requestSplitted.at(1) == "forward") {
			RobotControl::forward();
		}
		else if (requestSplitted.at(1) == "reverse") {
			RobotControl::reverse();
		}
		else if (requestSplitted.at(1) == "speed") {
			int speed = std::stoi(requestSplitted.at(2));
			RobotControl::setSpeed(speed);
		}
		else if (requestSplitted.at(1) == "wheels") {
			string direction = requestSplitted.at(2);
			if (direction == "rightForward") {
				RobotControl::turnRightForward();
			}
			else if (direction == "rightTop") {
				RobotControl::turnRightForward();
			}
			else if (direction == "straightforward") {
				RobotControl::forward();
			}
			else if (direction == "leftTop") {
				RobotControl::turnLeftForward();
			}
			else if (direction == "leftForward") {
				RobotControl::turnLeftForward();
			}
			else if (direction == "leftReverse") {
				RobotControl::turnLeftReverse();
			}
			else if (direction == "leftBottom") {
				RobotControl::turnLeftReverse();
			}
			else if (direction == "straightforwardReverse") {
				RobotControl::reverse();
			}
			else if (direction == "rightBottom") {
				RobotControl::turnRightReverse();
			}
			else if (direction == "rightReverse") {
				RobotControl::turnRightReverse();
			}
		}
	}
	else if (requestSplitted.at(0) == "camera") {
		if (requestSplitted.at(1) == "turnLeft") {
			RobotControl::turnCameraLeft();
		}
		else if (requestSplitted.at(1) == "turnRight") {
			RobotControl::turnCameraRight();
		}
		else if (requestSplitted.at(1) == "turnRight") {
			RobotControl::stopCameraRotation();
		}
	}
}