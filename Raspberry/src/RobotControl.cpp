#include "RobotControl.h"

int RobotControl::motorLTIN1 = 2;
int RobotControl::motorLTIN2 = 3;
int RobotControl::motorLTPWM = 0;
int RobotControl::motorRTIN1 = 4;
int RobotControl::motorRTIN2 = 5;
int RobotControl::motorRTPWM = 1;
int RobotControl::motorLDIN1 = 13;
int RobotControl::motorLDIN2 = 14;
int RobotControl::motorLDPWM = 12;
int RobotControl::motorRDIN1 = 10;
int RobotControl::motorRDIN2 = 11;
int RobotControl::motorRDPWM = 6;
bool RobotControl::initialised = false;
int RobotControl::speed = 100;
int RobotControl::servomotor = 7;
string RobotControl::direction = "stopped";
int RobotControl::rangefinderTEcho = 8;
int RobotControl::rangefinderTTrig = 9;
int RobotControl::rangefinderDEcho = 16;
int RobotControl::rangefinderDTrig = 15;
bool RobotControl::isEnabledToMoveForward = true;
bool RobotControl::isEnabledToMoveReverse = true;					

void RobotControl::init()
{
	if (wiringPiSetup() == -1) {
		std::cout << "Erreur, autorisation root requise." << std::endl;
		exit(1);
	};
	
	pinMode(motorLTIN1, OUTPUT);
	pinMode(motorLTIN2, OUTPUT);
	pinMode(motorLTPWM, OUTPUT);
	pinMode(motorRTIN1, OUTPUT);
	pinMode(motorRTIN2, OUTPUT);
	pinMode(motorRTPWM, OUTPUT);
	pinMode(motorLDIN1, OUTPUT);
	pinMode(motorLDIN2, OUTPUT);
	pinMode(motorLDPWM, OUTPUT);
	pinMode(motorRDIN1, OUTPUT);
	pinMode(motorRDIN2, OUTPUT);
	pinMode(motorRDPWM, OUTPUT);
	
	pinMode(servomotor, OUTPUT);
	
	pinMode(rangefinderTTrig, OUTPUT);
	pinMode(rangefinderTEcho, OUTPUT);
	digitalWrite(rangefinderTEcho, LOW);
	pinMode(rangefinderTEcho, INPUT);
	pinMode(rangefinderDTrig, OUTPUT);
	pinMode(rangefinderDEcho, OUTPUT);
	digitalWrite(rangefinderDEcho, LOW);
	pinMode(rangefinderDEcho, INPUT);
	
	piThreadCreate(rangefinderT);
	piThreadCreate(rangefinderD);
	
	softPwmCreate(motorLDPWM, 100, 100);
	softPwmCreate(motorRDPWM, 100, 100);
	softPwmCreate(motorLTPWM, 100, 100);
	softPwmCreate(motorRTPWM, 100, 100);
	
	softPwmCreate(servomotor, 15, 200);
	
	initialised = true;
}

void RobotControl::move()
{
	if (!initialised) { init(); }
	
	pwmWrite(motorLDPWM, 50);
	pwmWrite(motorRDPWM, 50);
	pwmWrite(motorLTPWM, 50);
	pwmWrite(motorRTPWM, 50);
	
	softPwmWrite(motorLTPWM, speed);
	softPwmWrite(motorRTPWM, speed);
	softPwmWrite(motorLDPWM, speed);
	softPwmWrite(motorRDPWM, speed);
	
	digitalWrite(motorLDPWM, HIGH);
	digitalWrite(motorRDPWM, HIGH);
	digitalWrite(motorLTPWM, HIGH);
	digitalWrite(motorRTPWM, HIGH);
}

void RobotControl::stop()
{
	if (!initialised) { init(); }
	
	digitalWrite(motorLTPWM, LOW);
	digitalWrite(motorRTPWM, LOW);
	digitalWrite(motorLDPWM, LOW);
	digitalWrite(motorRDPWM, LOW);
	
	pwmWrite(motorLTPWM, 0);
	pwmWrite(motorRTPWM, 0);
	pwmWrite(motorLDPWM, 0);
	pwmWrite(motorRDPWM, 0);
	
	softPwmWrite(motorLTPWM, 0);
	softPwmWrite(motorRTPWM, 0);
	softPwmWrite(motorLDPWM, 0);
	softPwmWrite(motorRDPWM, 0);
	
	direction = "stopped";
}

void RobotControl::forward()
{
	if (!initialised) { init(); }
	
	piLock(0);
	piUnlock(0);
	if (isEnabledToMoveForward) {
		digitalWrite(motorLTIN1, HIGH);
		digitalWrite(motorLTIN2, LOW);
		digitalWrite(motorRTIN1, HIGH);
		digitalWrite(motorRTIN2, LOW);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
		
		direction = "forward";
	}
}

void RobotControl::reverse()
{	
	if (!initialised) { init(); }
	
	piLock(0);
	piUnlock(0);
	if (isEnabledToMoveReverse) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, LOW);
		digitalWrite(motorLDIN2, HIGH);
		digitalWrite(motorRDIN1, LOW);
		digitalWrite(motorRDIN2, HIGH);
	
		direction = "reverse";
	}
}

void RobotControl::setSpeed(int speed)
{
	if (speed >= 0 && speed <= 100) {
		if (!initialised) { init(); }
		softPwmWrite(motorLTPWM, speed);
		softPwmWrite(motorRTPWM, speed);
		softPwmWrite(motorLDPWM, speed);
		softPwmWrite(motorRDPWM, speed);
		RobotControl::speed = speed;
	}
}

void RobotControl::speedUp()
{
	if (!initialised) { init(); }
	speed = speed + 10;
	if (speed > 100) { speed = 100; }
	softPwmWrite(motorLTPWM, speed);
	softPwmWrite(motorRTPWM, speed);
	softPwmWrite(motorLDPWM, speed);
	softPwmWrite(motorRDPWM, speed);
}

void RobotControl::slowDown()
{
	if (!initialised) { init(); }
	speed = speed - 10;
	if (speed < 0) { speed = 0; }
	softPwmWrite(motorLTPWM, speed);
	softPwmWrite(motorRTPWM, speed);
	softPwmWrite(motorLDPWM, speed);
	softPwmWrite(motorRDPWM, speed);
}

void RobotControl::turnRightForward()
{
	if (!initialised) { init(); }
	
	piLock(0);
	piUnlock(0);
	if (isEnabledToMoveForward) {
		digitalWrite(motorLTIN1, HIGH);
		digitalWrite(motorLTIN2, LOW);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
	}
}
void RobotControl::turnLeftForward()
{
	if (!initialised) { init(); }
	
	piLock(0);
	piUnlock(0);
	if (isEnabledToMoveForward) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, HIGH);
		digitalWrite(motorRTIN2, LOW);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
	}
}

void RobotControl::turnRightReverse()
{
	if (!initialised) { init(); }
	
	piLock(0);
	piUnlock(0);
	if (isEnabledToMoveReverse) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, LOW);
		digitalWrite(motorLDIN2, HIGH);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
	}
}

void RobotControl::turnLeftReverse()
{
	if (!initialised) { init(); }
	
	piLock(0);
	piUnlock(0);
	if (isEnabledToMoveReverse) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, LOW);
		digitalWrite(motorRDIN2, HIGH);
	}
}

void RobotControl::turnLeft()
{
	if (direction == "reverse") { turnLeftReverse(); }
	else { turnLeftForward(); }
}

void RobotControl::turnRight()
{
	if (direction == "reverse") { turnRightReverse(); }
	else { turnRightForward(); }
}

void RobotControl::turnCameraRight()
{
	if (!initialised) { init(); }
	
	softPwmCreate(servomotor, 15, 200);
	//Valeur pour la vitesse de rotation maximale : 10
	softPwmWrite(servomotor,13);
}

void RobotControl::turnCameraLeft()
{
	if (!initialised) { init(); }
	
	softPwmCreate(servomotor, 15, 200);
	//Valeur pour la vitesse de rotation maximale : 20
	softPwmWrite(servomotor,17);
}

void RobotControl::stopCameraRotation()
{
	if (!initialised) { init(); }
	
	softPwmCreate(servomotor, 0, 0);
	softPwmWrite(servomotor,0);
}

void *RobotControl::rangefinderT(void *dummy)
{
	digitalWrite(rangefinderTTrig, LOW);
	
	std::this_thread::sleep_for (std::chrono::seconds(2));
	/*time_t start;
	time_t end;*/
	typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<double> sec;
    sec duration;
	auto start = Time::now();
	auto end = Time::now();
	/*boost::posix_time::ptime start;
	boost::posix_time::ptime end;*/
	//double duration;
	//boost::posix_time::time_duration duration;
	double distance;
	while (1) {
		digitalWrite(rangefinderTTrig, LOW);
		sleep(1);
		digitalWrite(rangefinderTTrig, HIGH);
		//std::this_thread::sleep_for (std::chrono::seconds(0.00001));
		usleep(10);
		digitalWrite(rangefinderTTrig, LOW);
		usleep(10);
		while (digitalRead(rangefinderTEcho) == LOW)
		{
			start = Time::now();
			std::cout << "TEST START" << std::endl;
			//time(&start);
			//start = boost::posix_time::second_clock::local_time();
		}
		do 
		{
			end = Time::now();
			/*time(&end);*/ /*end = boost::posix_time::second_clock::local_time();*/
			std::cout << "TEST END" << std::endl;
		} while (digitalRead(rangefinderTEcho) == HIGH);
		duration = end - start;
		//duration = difftime(start, end);
		distance = 17150*duration.count();
		/*duration = end - start;
		distance = 17150*duration.total_microseconds()*1000000;*/
		std::cout << "Distance avant : " << distance << " duration = " << duration.count() << std::endl;
		if (distance > 15 || distance < 10) {
			piLock(0);
			stop();
			isEnabledToMoveForward = false;
			piUnlock(0);
		}
		else {
			isEnabledToMoveForward = true;
		}
	}
	return NULL;
}

void *RobotControl::rangefinderD(void *dummy)
{
	digitalWrite(rangefinderDTrig, LOW);
	std::this_thread::sleep_for (std::chrono::seconds(2));
	time_t start;
	time_t end;
	double duration;
	double distance;
	while (1) {
		digitalWrite(rangefinderDTrig, LOW);
		sleep(1);
		digitalWrite(rangefinderDTrig, HIGH);
		//std::this_thread::sleep_for (std::chrono::seconds(0.00001));
		usleep(10);
		digitalWrite(rangefinderDTrig, LOW);
		while (rangefinderDEcho == LOW) { start = time(NULL); }
		while (rangefinderDEcho == HIGH) { end = time(NULL); }
		duration = difftime(start, end);
		distance = 17150*duration;
		//std::cout << "Distance arrière : " << distance << " start = " << start << " end = " << end << std::endl;
		if (distance > 15 || distance < 10) {
			piLock(0);
			stop();
			isEnabledToMoveReverse = false;
			piUnlock(0);
		}
		else {
			isEnabledToMoveForward = true;
		}
	}
	return NULL;
}