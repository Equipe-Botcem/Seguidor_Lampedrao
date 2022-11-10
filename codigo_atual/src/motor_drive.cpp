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

Motor_drive::Motor_drive(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char OCM)
{

	pin_PWM_1 = PWM_1;
	pin_PWM_2 = PWM_2;
	pin_EN = EN;
	pin_OCM = OCM;

	motor_direction = 1;
}

void Motor_drive::Init()
{
	pinMode(pin_PWM_1, OUTPUT);
	pinMode(pin_PWM_2, OUTPUT);
	pinMode(pin_EN, OUTPUT);
	pinMode(pin_OCM, INPUT);
}

void Motor_drive::Enable_drive()
{
	digitalWrite(pin_EN, HIGH);
}

void Motor_drive::Disable_drive()
{
	Set_speed(0);
	digitalWrite(pin_EN, LOW);
}

//! Pino DIAG não está sendo usado
/* 
unsigned char Motor_drive::Get_fault()
{
	return !digitalRead(pin_DIAG);
}
*/


void Motor_drive::Set_motor_forward()
{
	motor_direction = 1;
	On();
}

void Motor_drive::Set_motor_reverse()
{
	motor_direction = 0;
	On();
}

void Motor_drive::Set_speed(int speed)
{
	Serial.print("motor speed:");
	Serial.println(speed);
	motor_speed = speed;
	On();
}

void Motor_drive::On(){
	if (motor_direction)
	{
		analogWrite(pin_PWM_1, motor_speed);
		analogWrite(pin_PWM_2, LOW);
	}
	else
	{
		analogWrite(pin_PWM_1, LOW);
		analogWrite(pin_PWM_2, motor_speed);
	}
}

double Motor_drive::Get_current_milliamps(){
	return analogRead(pin_OCM) * 9.765625;
}
