#include "Motor.h"
#include "Arduino.h"

Motor::Motor()
{
}

Motor::Motor(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char OCM)
{

	pin_PWM_1 = PWM_1;
	pin_PWM_2 = PWM_2;
	pin_EN = EN;
	pin_OCM = OCM;
}

void Motor::Init()
{
	pinMode(pin_PWM_1, OUTPUT);
	pinMode(pin_PWM_2, OUTPUT);
	pinMode(pin_EN, OUTPUT);
	pinMode(pin_OCM, INPUT);
}

void Motor::Enable_drive()
{
	digitalWrite(pin_EN, HIGH);

}

void Motor::Disable_drive()
{
	Set_speed(0);
	digitalWrite(pin_EN, LOW);
}

void Motor::Set_speed(int speed)
{

	// Checa se a velocidade está entre os valores permitidos
	if(speed > 255) speed = 255;
	if(speed < -255) speed = -255;


	if (speed >= 0)
	{
		analogWrite(pin_PWM_1, speed);
		analogWrite(pin_PWM_2, LOW);
	}
	else
	{
		speed = -1*speed;
		analogWrite(pin_PWM_1, LOW);
		analogWrite(pin_PWM_2, speed);
	}

}
