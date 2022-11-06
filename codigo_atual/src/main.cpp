#include "Seguidor/Seguidor.h"
#include "Utilities/Utilities.h"


Seguidor seguidor = Seguidor();
String command = "";

void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	initBluetooth();
}

void loop(){
	seguidor.Behavior(comunica_serial());
}


