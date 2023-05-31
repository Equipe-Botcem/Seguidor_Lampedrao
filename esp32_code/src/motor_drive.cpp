#include "motor_drive.h"
#include "Arduino.h"

#if defined(DIAG_MODE)
void Motor_drive::set_name(String nome){
	name = nome;
}

void Motor_drive::telemetria()
{
	Serial.println("|===============TELEMETRIA DO MOTOR DO ROBO===============|");
	Serial.print("\tMOTOR VERIFICADO: ");
	Serial.println(name);
	Serial.print("\tTIME: ");
	Serial.print((millis() - timer_exec) / 1000);
	Serial.println(" sec");
	Serial.println("---------------------------------------------------------------");
	Serial.print("MOTOR DIRECTION: ");
	if (motor_direction)
	{
		Serial.println("FRENTE");
		analogWrite(pin_PWM_1, motor_speed);
		analogWrite(pin_PWM_2, LOW);

	}
	else
	{
		Serial.println("TRAS");
		analogWrite(pin_PWM_1, LOW);
		analogWrite(pin_PWM_2, motor_speed);
	}
	Serial.print("MOTOR SPEED: ");
	Serial.println(motor_speed);
	Serial.print("MOTOR CURRENT: ");
	Serial.println(Get_current_milliamps());
	Serial.println("|==============================================================|");
}
#endif

Motor_drive::Motor_drive()
{
}

Motor_drive::Motor_drive(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char OCM)
{

	pin_PWM_1 = PWM_1;
	pin_PWM_2 = PWM_2;
	pin_EN = EN;
	pin_OCM = OCM;
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

void Motor_drive::Set_speed(int speed)
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
