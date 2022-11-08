#include "Seguidor.h"
#include "Utilities.h"

Seguidor seguidor = Seguidor();
Sensor teste = Sensor(38);

void setup()
{
	Serial.begin(115200);
	seguidor.Init();
	initBluetooth();
}

void loop(){
	Serial.print("SLE:");
	Serial.print(analogRead(39));
	Serial.print("  ");
	Serial.print("S0:");
	Serial.print(analogRead(36));
	Serial.print("  ");
	Serial.print("S1:");
	Serial.print(analogRead(13));
	Serial.print("  ");
	Serial.print("S2:");
	Serial.print(analogRead(14));
	Serial.print("  ");
	Serial.print("S3:");
	Serial.print(analogRead(35));
	Serial.print("  ");
	Serial.print("S4:");
	Serial.print(analogRead(32));
	Serial.print("  ");
	Serial.print("S5:");
	Serial.print(analogRead(33));
	Serial.print("  ");
	Serial.print("S6:");
	Serial.print(analogRead(25));
	Serial.print("  ");
	Serial.print("S7:");
	Serial.print(analogRead(26));
	Serial.print("  ");
	Serial.print("S8:");
	Serial.print(analogRead(27));
	Serial.print("  ");
	Serial.print("SLD:");
	Serial.print(analogRead(27));
	Serial.print("  ");
	Serial.println("  ");
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


