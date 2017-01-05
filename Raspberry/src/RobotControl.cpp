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

void RobotControl::reverse()
{	
	if (!initialised) { init(); }
	
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
	
	digitalWrite(motorLTIN1, HIGH);
	digitalWrite(motorLTIN2, LOW);
	digitalWrite(motorRTIN1, LOW);
	digitalWrite(motorRTIN2, HIGH);
	digitalWrite(motorLDIN1, HIGH);
	digitalWrite(motorLDIN2, LOW);
	digitalWrite(motorRDIN1, HIGH);
	digitalWrite(motorRDIN2, LOW);
}
void RobotControl::turnLeftForward()
{
	if (!initialised) { init(); }
	
	digitalWrite(motorLTIN1, LOW);
	digitalWrite(motorLTIN2, HIGH);
	digitalWrite(motorRTIN1, HIGH);
	digitalWrite(motorRTIN2, LOW);
	digitalWrite(motorLDIN1, HIGH);
	digitalWrite(motorLDIN2, LOW);
	digitalWrite(motorRDIN1, HIGH);
	digitalWrite(motorRDIN2, LOW);
}

void RobotControl::turnRightReverse()
{
	if (!initialised) { init(); }
	
	digitalWrite(motorLTIN1, LOW);
	digitalWrite(motorLTIN2, HIGH);
	digitalWrite(motorRTIN1, LOW);
	digitalWrite(motorRTIN2, HIGH);
	digitalWrite(motorLDIN1, LOW);
	digitalWrite(motorLDIN2, HIGH);
	digitalWrite(motorRDIN1, HIGH);
	digitalWrite(motorRDIN2, LOW);
}

void RobotControl::turnLeftReverse()
{
	if (!initialised) { init(); }
	
	digitalWrite(motorLTIN1, LOW);
	digitalWrite(motorLTIN2, HIGH);
	digitalWrite(motorRTIN1, LOW);
	digitalWrite(motorRTIN2, HIGH);
	digitalWrite(motorLDIN1, HIGH);
	digitalWrite(motorLDIN2, LOW);
	digitalWrite(motorRDIN1, LOW);
	digitalWrite(motorRDIN2, HIGH);
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