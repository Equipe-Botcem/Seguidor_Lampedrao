#include "Encoder/test/encoder_test.h"


void Encoder_command_1(){
	Motor_drive motor_teste(6, 5, 4, 7, A2);
	Encoder encoder(2);

	motor_teste.Init();
	encoder.Init();

	motor_teste.Enable_drive();
	motor_teste.Set_motor_forward();
	motor_teste.Set_speed(25);

	bool stop_cond = false;
	unsigned long time = millis();
	while (!stop_cond)
	{
		if (400 <= encoder.Get_contador())
		{
			motor_teste.Disable_drive();
			stop_cond = true;
			encoder.Reset();
		}
		//Serial.println(encoder.Get_contador());
	}
	Serial.println(millis()-time);
}