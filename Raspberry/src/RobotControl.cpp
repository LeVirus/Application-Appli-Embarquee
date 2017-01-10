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
vector<double> RobotControl::rangefinderTMeasures;
vector<double> RobotControl::rangefinderDMeasures;
int RobotControl::nbRangefinderMeasures = 10;				

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
	
	//piThreadCreate(rangefinderT);
	//piThreadCreate(rangefinderD);
	
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
	
	direction = "move";
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
	if (isEnabledToMoveForward) {
		digitalWrite(motorLTIN1, HIGH);
		digitalWrite(motorLTIN2, LOW);
		digitalWrite(motorRTIN1, HIGH);
		digitalWrite(motorRTIN2, LOW);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
		if (direction == "stopped") { move(); }
		
		direction = "forward";
	}
	piUnlock(0);
}

void RobotControl::reverse()
{	
	if (!initialised) { init(); }
	
	piLock(0);
	if (isEnabledToMoveReverse) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, LOW);
		digitalWrite(motorLDIN2, HIGH);
		digitalWrite(motorRDIN1, LOW);
		digitalWrite(motorRDIN2, HIGH);
		if (direction == "stopped") { move(); }
		
		direction = "reverse";
	}
	piUnlock(0);
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
	if (isEnabledToMoveForward) {
		digitalWrite(motorLTIN1, HIGH);
		digitalWrite(motorLTIN2, LOW);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
		if (direction == "stopped") { move(); }
	}
	piUnlock(0);
}
void RobotControl::turnLeftForward()
{
	if (!initialised) { init(); }
	
	piLock(0);
	if (isEnabledToMoveForward) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, HIGH);
		digitalWrite(motorRTIN2, LOW);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
		if (direction == "stopped") { move(); }
	}
	piUnlock(0);
}

void RobotControl::turnRightReverse()
{
	if (!initialised) { init(); }
	
	piLock(0);
	if (isEnabledToMoveReverse) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, LOW);
		digitalWrite(motorLDIN2, HIGH);
		digitalWrite(motorRDIN1, HIGH);
		digitalWrite(motorRDIN2, LOW);
		if (direction == "stopped") { move(); }
		piUnlock(0);
	}
}

void RobotControl::turnLeftReverse()
{
	if (!initialised) { init(); }
	
	piLock(0);
	if (isEnabledToMoveReverse) {
		digitalWrite(motorLTIN1, LOW);
		digitalWrite(motorLTIN2, HIGH);
		digitalWrite(motorRTIN1, LOW);
		digitalWrite(motorRTIN2, HIGH);
		digitalWrite(motorLDIN1, HIGH);
		digitalWrite(motorLDIN2, LOW);
		digitalWrite(motorRDIN1, LOW);
		digitalWrite(motorRDIN2, HIGH);
		if (direction == "stopped") { move(); }
	}
	piUnlock(0);
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
	typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<double> sec;
    sec duration;
	auto start = Time::now();
	auto end = Time::now();
	auto timeout = Time::now();
	sec waitingTime;
	double distance;
	double average;
	while (1) {
		digitalWrite(rangefinderTTrig, LOW);
		sleep(1);
		digitalWrite(rangefinderTTrig, HIGH);
		waitingTime = start - start;
		timeout = Time::now();
		while (digitalRead(rangefinderTEcho) == LOW && waitingTime.count() < 1.5)
		{
			digitalWrite(rangefinderTTrig, LOW);
			start = Time::now();
			waitingTime = start - timeout;
		}
		if (waitingTime.count() < 1.5) {
			waitingTime = start - start;
			timeout = Time::now();
			do 
			{
				end = Time::now();
				waitingTime = end - timeout;
			} while (digitalRead(rangefinderTEcho) == HIGH  && waitingTime.count() < 1.5);
			if (waitingTime.count() < 1.5) {
				duration = end - start;
				distance = 17150*duration.count();
				rangefinderTMeasures.push_back(distance);
				if (rangefinderTMeasures.size() >= nbRangefinderMeasures) {  
					rangefinderTMeasures.erase(rangefinderTMeasures.begin());
					average = Utilities::average(rangefinderTMeasures);
					std::cout << "Distance avant : " << distance << " moyenne = " << average << std::endl;
					if (average < 40.0 || average > 75.0) {
						if (isEnabledToMoveForward) {
							piLock(0);
							if (direction == "forward") { stop(); }
							isEnabledToMoveForward = false;
							piUnlock(0);
						}
					}
					else {
						piLock(0);
						isEnabledToMoveForward = true;
						piUnlock(0);
					}
				}
			}
		}
	}
	return NULL;
}

void *RobotControl::rangefinderD(void *dummy)
{
	digitalWrite(rangefinderDTrig, LOW);
	
	std::this_thread::sleep_for (std::chrono::seconds(2));
	typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<double> sec;
    sec duration;
	auto start = Time::now();
	auto end = Time::now();
	auto timeout = Time::now();
	sec waitingTime;
	double distance;
	double average;
	while (1) {
		digitalWrite(rangefinderDTrig, LOW);
		sleep(1);
		digitalWrite(rangefinderDTrig, HIGH);
		waitingTime = start - start;
		timeout = Time::now();
		while (digitalRead(rangefinderDEcho) == LOW && waitingTime.count() < 1.5)
		{
			digitalWrite(rangefinderDTrig, LOW);
			start = Time::now();
			waitingTime = start - timeout;	
		}
		if (waitingTime.count() < 1.5) {
			waitingTime = start - start;
			timeout = Time::now();
			do 
			{
				end = Time::now();
				waitingTime = end - timeout;
			} while (digitalRead(rangefinderDEcho) == HIGH  && waitingTime.count() < 1.5);
			duration = end - start;
			if (waitingTime.count() < 1.5) {
				distance = 17150*duration.count();
				rangefinderDMeasures.push_back(distance);
				if (rangefinderDMeasures.size() >= nbRangefinderMeasures) {  
					rangefinderDMeasures.erase(rangefinderDMeasures.begin());
					average = Utilities::average(rangefinderDMeasures);
					std::cout << "Distance arrière : " << distance << " moyenne = " << average << std::endl;
					if (average < 40.0 || average > 75.0) {
						std::cout << "Test entree boucle" << std::endl;
						if (isEnabledToMoveReverse) {
							std::cout << "Test boucle enabled" << std::endl;
							piLock(0);
							if (direction == "reverse") { std::cout << "Test stop" << std::endl; stop(); }
							isEnabledToMoveReverse = false;
							piUnlock(0);
						}
					}
					else {
						piLock(0);
						isEnabledToMoveReverse = true;
						piUnlock(0);
					}
				}
			}
		}
	}
	return NULL;
}
