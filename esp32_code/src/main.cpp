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
}


void loop(){
	

	seguidor.Behavior();

	if(seguidor.isStar()){
		seguidor.controle();
	}
	
	// Para o seguidor no final
	if(millis() - seguidor.tempo_corrido > 5000){
		Serial.println("Habilitei parada");
		if (seguidor.Check_stop() and seguidor.isEnd() == false){
			seguidor.habiliteiStop();
		}else if (millis() - seguidor.tempo_stop > 300){
			seguidor.Stop();
		}
	}

	
}