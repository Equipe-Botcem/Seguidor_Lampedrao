#include "Seguidor.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


Seguidor seguidor = Seguidor();


void Rotina(){
	//seguidor.Behavior(comunica_serial());
	//int behavior = comunica_serial();
	
	//seguidor.Behavior(comunica_serial());
	if(seguidor.start_condition && !seguidor.stop_condition){
		seguidor.controle();
	}
	else if (seguidor.stop_condition){
		seguidor.Stop();
	}
}


void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	seguidor.initBluetooth();
}

void loop(){
	seguidor.SerialBT.println(seguidor.sensor_linha[0].Read_sensor());
	seguidor.testeSensores();

	Rotina();

}
