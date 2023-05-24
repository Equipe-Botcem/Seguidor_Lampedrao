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
	//analogReadResolution(10);
}


void loop(){
	
	seguidor.testeSensores();
	delay(500);


	// seguidor.Behavior();

	// if(seguidor.isStar()){
	// 	seguidor.controle();
	// }

	// seguidor.stopRoutine();

	
}