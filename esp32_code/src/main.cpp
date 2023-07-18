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
	
	//seguidor.teste();

	seguidor.Behavior();

	if(seguidor.isStart()){
		seguidor.controle();

		// Rotina de mapeamento
		//seguidor.mapeamento();

		//Rotina de parada 
		seguidor.stopRoutine();

	}


	
}