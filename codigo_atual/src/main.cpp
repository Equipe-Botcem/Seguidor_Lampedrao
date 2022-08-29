
#include <Motor_drive_teste.h>
#include "Encoder.h"

bool tested = 0;

void setup()
{

	Serial.begin(115200);
	// pin_config();
	// calibracao_handler();
}

void loop()
{
	if (!tested)
	{
		//! arrumar as portas 
		Motor_drive motor_teste(6,5,4,7,A2);
		Encoder encoder(8);

		motor_teste.Init();
		encoder.Init();

		motor_teste.Enable_drive();
		motor_teste.Set_motor_forward();
		motor_teste.Set_speed(100);

		bool stop_cond = false;
		unsigned long time = millis();
		while (!stop_cond)
		{
			if (millis() - time > 5000)
			{
				stop_cond = true;
			}
			Serial.println(encoder.Get_contador());
		}
		motor_teste.Disable_drive();
		Serial.println(encoder.Get_contador());
		encoder.Reset();
		Serial.println(encoder.Get_contador());
		tested = 1;
	}
}