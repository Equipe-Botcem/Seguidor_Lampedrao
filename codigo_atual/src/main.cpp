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
}

unsigned long time = 0;
void loop(){
	time = millis();
	seguidor.Behavior();
	seguidor.Check_stop();
	if(seguidor.start_condition && !seguidor.stop_condition){
		seguidor.controle();
	}
	else if (seguidor.stop_condition){
		seguidor.Stop();
	}
	seguidor.SerialBT.println(millis()-time);
}