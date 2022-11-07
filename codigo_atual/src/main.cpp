#include "Seguidor/Seguidor.h"
#include "Utilities/Utilities.h"

Seguidor seguidor = Seguidor();
void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	initBluetooth();
}

void loop(){
	//seguidor.Behavior(comunica_serial());
	//int behavior = comunica_serial();
	seguidor.Behavior(comunica_serial());
	if(seguidor.start_condition && !seguidor.stop_condition){
		seguidor.u_c.controle();
	}
	else if (seguidor.stop_condition){
		seguidor.Stop();
	}
}


