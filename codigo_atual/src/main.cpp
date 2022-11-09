#include "Seguidor.h"
#include "Utilities.h"

Seguidor seguidor = Seguidor();

void Rotina(){
	seguidor.Behavior(comunica_serial());
	int behavior = comunica_serial();
	
	seguidor.Behavior(comunica_serial());
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
	//seguidor.Init();
	//initBluetooth();
	pinMode(39, INPUT);
	pinMode(36, INPUT);
	pinMode(13, INPUT);
	pinMode(14, INPUT);
	pinMode(35, INPUT);
	pinMode(32, INPUT);
	pinMode(33, INPUT);
	pinMode(25, INPUT);
	pinMode(26, INPUT);
	pinMode(27, INPUT);
}

void loop(){

	seguidor.testeSensores();

	Rotina();

}
