//#define _TEST_MODE_

#ifdef _TEST_MODE_ 

	#include "Controlador/test/Controlador_test.h"

#else

	#include <Encoder/Encoder.h>
	#include "Controlador/Controlador.h"

#endif



//#include <Controlador_class/Controlador.h>

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
		tested = 1;
		
	}
}