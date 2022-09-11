#include "Motor_drive/test/Motor_drive_teste.h"

void Motor_drive_teste_init()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
}

void Motor_drive_teste_enable()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
}

void Motor_drive_teste_disable()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Disable_drive();
}

void Motor_drive_teste_set_forward()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
}

void Motor_drive_teste_set_reverse()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_reverse();
}

void Motor_drive_teste_set_speed(int speed)
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	motor_teste.Set_speed(speed);
	Serial.println("tested");
}

double Motor_drive_teste_get_current(int speed)
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	motor_teste.Set_speed(speed);
	return motor_teste.Get_current_milliamps();
}

void Motor_drive_teste_command_1()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	motor_teste.Set_speed(100);
	Serial.println("start");
	delay(5000);
	Serial.println("stop");
	motor_teste.Disable_drive();
}

void Motor_drive_teste_command_2()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	motor_teste.Set_speed(50);
	Serial.println("start");
	delay(5000);
	Serial.println("revert");
	motor_teste.Set_motor_reverse();
	motor_teste.Set_speed(50);
	delay(5000);
	Serial.println("stop");
	motor_teste.Disable_drive();
}

void Motor_drive_teste_command_3()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	for (int i = 0; i < 255; i++)
	{
		motor_teste.Set_speed(i);
		delay(20);
	}
	motor_teste.Disable_drive();
}

void Motor_drive_teste_command_4()
{
	Motor_drive motor_teste(6, 5, 4, 8, 7);
	motor_teste.Init();
	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	for (int i = 0; i < 255; i++)
	{
		motor_teste.Set_speed(i);
		delay(50);
		Serial.println(motor_teste.Get_current_milliamps());
	}
	motor_teste.Disable_drive();
}