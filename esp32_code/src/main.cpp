#include "Seguidor.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


Seguidor seguidor = Seguidor();



void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	seguidor.initBluetooth();
	analogReadResolution(8);
	timer_exec = millis();
}


void loop(){


	seguidor.testeSensores();
	/* 
	seguidor.Behavior();
	if(seguidor.start_condition){
		seguidor.controle();
	}
	
	// Checa se chegou no final
	if (seguidor.Check_stop()){
		seguidor.Stop();
	}
	*/
}