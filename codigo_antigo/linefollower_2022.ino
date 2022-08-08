#include "linefollower_2022.hpp"



void setup() {
    Serial.begin(115200);
	pin_mode_initiator();
	BLE_initiator();
}

void loop(){
	if (deviceConnected) {

		//if que vai fazer o robo iniciar o percurso da pista
		if(RUN){

		}
    //characteristicTX->setValue(txString); //seta o valor que a caracteristica notificará (enviar) 
    //characteristicTX->notify(); // Envia o valor para o smartphone
    }
}