#include "motor_drive.h"

Motor_drive::Motor_drive()
{

	//pin_PWM_1 = 99;
	//pin_PWM_2 = 99;
	//pin_EN = 99;
	//pin_DIAG = 99;
	//pin_OCM = 99;

	motor_direction = 1;
}

Motor_drive::Motor_drive(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char DIAG, unsigned char OCM)
{

	pin_PWM_1 = PWM_1;
	pin_PWM_2 = PWM_2;
	pin_EN = EN;
	pin_DIAG = DIAG;
	pin_OCM = OCM;

	motor_direction = 1;
}

void Motor_drive::init()
{
	pinMode(pin_PWM_1, OUTPUT);
	pinMode(pin_PWM_2, OUTPUT);
	pinMode(pin_EN, OUTPUT);
	pinMode(pin_DIAG, INPUT_PULLUP);
	pinMode(pin_OCM, INPUT);
}

void Motor_drive::enable_drive()
{
	digitalWrite(pin_EN, HIGH);
}

void Motor_drive::disable_drive()
{
	digitalWrite(pin_EN, LOW);
}

unsigned char Motor_drive::get_fault()
{
	return !digitalRead(pin_DIAG);
}

void Motor_drive::set_motor_forward()
{
	if (motor_direction)
		return;
	motor_direction = 1;
}

void Motor_drive::set_motor_reverse()
{
	if (!motor_direction)
		return;
	motor_direction = 0;
}

void Motor_drive::set_speed(int speed)
{
	if (motor_direction)
	{
		analogWrite(pin_PWM_1, speed);
		analogWrite(pin_PWM_2, LOW);
	}
	else
	{
		analogWrite(pin_PWM_1, LOW);
		analogWrite(pin_PWM_2, speed);
	}
}

double Motor_drive::get_current_milliamps(){
	return analogRead(pin_OCM) * 9.765625;
}