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
	seguidor.tempo_corrido = millis();
}


void loop(){
	// seguidor.testeSensores();
	// seguidor.Check_stop();
	// delay(1000);
	

	seguidor.Behavior();

	
	if(seguidor.start_condition){
		seguidor.controle();
	}
	
	// Checa se chegou no final
	if(millis() - seguidor.tempo_corrido > 5000){
		Serial.println("Habilitei parada");

		if (seguidor.Check_stop()){
			delay(500);
			seguidor.Stop();
		}
	}

	
}