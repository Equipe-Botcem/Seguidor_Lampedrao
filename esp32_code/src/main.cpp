#include "Seguidor.h"
#include "Sensor_linha.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


Seguidor seguidor = Seguidor();
Sensor_linha sensor_linha = Sensor_linha();


void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	seguidor.initBluetooth();
}


void loop(){
	
	// seguidor.teste();
	
	seguidor.Behavior();
	seguidor.CheckLed();

	if(seguidor.isStart()){
		seguidor.controle();
		
		if(!seguidor.IsOut())seguidor.stopRoutine();

		seguidor.mapeamento();	
		// Serial.println("getangle: " + String(sensor_linha.getAngleRaw()));
	}
}