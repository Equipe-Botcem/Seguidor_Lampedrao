#include "Seguidor.h"
#include "Utilities.h"

Seguidor seguidor = Seguidor();
Sensor teste = Sensor(35);

void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	initBluetooth();
}

void loop(){
	SerialBT.print("Anal: ");
	SerialBT.println(analogRead(35));
	SerialBT.print("Seg: ");
	uint16_t teste = seguidor.sensor_linha.Read_line(4);
	SerialBT.println(seguidor.sensor_linha.Read_line(4));
	delay(1000);
	//seguidor.Behavior(comunica_serial());
	//int behavior = comunica_serial();
	/*
	seguidor.Behavior(comunica_serial());
	if(seguidor.start_condition && !seguidor.stop_condition){
		seguidor.u_c.controle();
	}
	else if (seguidor.stop_condition){
		seguidor.Stop();
	}
	*/

}


