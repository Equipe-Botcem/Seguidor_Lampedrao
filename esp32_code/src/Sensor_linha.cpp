#include "Sensor_linha.h"

Sensor_linha::Sensor_linha(){}

Sensor_linha::Sensor_linha(unsigned char* pins){
    for(unsigned i = 0; i < 8; i++){
		sensores[i] = Sensor(pins[i]);
		sensores[i].setAngle(angulos[i]);
	}
    last_read = 0;
}

void Sensor_linha::Init(){
    for(unsigned i = 0; i < 8; i++){
		sensores[i].Init();
	}
}

float Sensor_linha::getAngle(){
    int j = 4;

	for(int i = 3; i >= 0; i--){
		if(sensores[i].Read_histerese()){
            last_read = mediaPond(i);
            return last_read;  
        } 
		if(sensores[j].Read_histerese()){
            last_read = mediaPond(j);
            return last_read;
        } 
		j++;
	}

	// Saiu da pista 
	if(last_read < 0) return -50;

	return 50;
}

float Sensor_linha::mediaPond(int pos){
    float num;
	float den;

	if(pos == 0){
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos + 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos + 1].Read_Calibrado();
	}else if (pos == 7){
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos - 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos - 1].Read_Calibrado();
	}else{
		num =sensores[pos].Read_CalibradoPonderado() + sensores[pos - 1].Read_CalibradoPonderado() + sensores[pos + 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos - 1].Read_Calibrado() + sensores[pos + 1].Read_Calibrado();
	}
	return num / den;
}

void Sensor_linha::calibration_max(){
    for(unsigned i = 0; i < 8; i++) sensores[i].find_max();
}

void Sensor_linha::calibration_min(){
    for(unsigned i = 0; i < 8; i++) sensores[i].find_min();

}

void Sensor_linha::testeLeituraRaw(){

}