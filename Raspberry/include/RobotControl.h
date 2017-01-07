#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H


#include <iostream>
#include <wiringPi/wiringPi.h>
#include <wiringPi/softPwm.h>
#include <thread>
#include <chrono>
#include <time.h>
#include <unistd.h>
//#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;

class RobotControl
{	
	public:
		static void init();
		static void move();
		static void stop();
		static void forward();
		static void reverse();
		static void setSpeed(int speed);
		static void turnRightForward();
		static void turnLeftForward();
		static void turnRightReverse();
		static void turnLeftReverse();
		static void turnCameraLeft();
		static void turnCameraRight();
		static void turnLeft();
		static void turnRight();
		static void stopCameraRotation();
		static void speedUp();
		static void slowDown();
	
	private:
		static int motorLTIN1;
		static int motorLTIN2;
		static int motorLTPWM;
		static int motorRTIN1;
		static int motorRTIN2;
		static int motorRTPWM;
		static int motorLDIN1;
		static int motorLDIN2;
		static int motorLDPWM;
		static int motorRDIN1;
		static int motorRDIN2;
		static int motorRDPWM;
		static bool initialised;
		static int speed;
		static int servomotor;
		static string direction;
		static int rangefinderTEcho;
		static int rangefinderTTrig;
		static int rangefinderDEcho;
		static int rangefinderDTrig;
		static bool isEnabledToMoveForward;
		static bool isEnabledToMoveReverse;
		
		static PI_THREAD(rangefinderT);
		static PI_THREAD(rangefinderD); 		
};

#endif
