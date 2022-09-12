#define _TEST_MODE_

#ifdef _TEST_MODE_ 

	#include "Sensors/test/Sensor_teste.h"

	bool tested = 0;

	void setup()
	{
		Serial.begin(115200);
	}

	void loop()
	{
		if (!tested)
		{
			Sensor_teste_init();

			//Serial.println(Sensor_teste_read());

			//Sensor_linha_test_init();

			//Sensor_linha_test_read();
			
			tested = 1;
			
		}
	}

#else

	//* CODIGO PARA EXECUÇÃO NORMAL

	#include "Controlador/Controlador.h"

	void setup()
	{
		// pin_config();
		// calibracao_handler();
	}

	void loop()
	{
		
	}

#endif


