#include "Sensor_linha.h"
#include "SimpleKalmanFilter.h"

SimpleKalmanFilter angleKalmanFilter(0.01, 0.01, 0.005);

Sensor_linha::Sensor_linha(){}

Sensor_linha::Sensor_linha(unsigned char* pins){
    for(unsigned i = 0; i < 4; i++){
		sensores[i] = Sensor(pins[i]);
		sensores[i].setAngle(angulos[i]);
	}
    last_read = 0;
}

void Sensor_linha::Init(){
    for(unsigned i = 0; i < tam_sensores; i++){
		sensores[i].Init();
	}
}

float Sensor_linha::getAngleRaw(){
  int j = 2;

	in = false;

	for(int i = 1; i >= 0; i--){
    if(sensores[j].Read_histerese()){
      read = mediaPond(j);
			in = true;
      break;
    } 
		if(sensores[i].Read_histerese()){
      read = mediaPond(i);
			in = true;
      break;  
    } 
		j++;
	}

	// Saiu da pista 
	if(read < 0 and in == false){
		read = -23;
	}else if(in == false)	read = 23;

	int variacao = read - last_read;

	if(abs(variacao) > 7){
		read = last_read;
	}else{
		last_read = read;
	}

	return read;
}

float Sensor_linha::getAngle(){
	return angleKalmanFilter.updateEstimate(getAngleRaw());
}

float Sensor_linha::mediaPond(int pos){
	float num;
	float den;

	if(sensores[1].Read_histerese() == HIGH and sensores[2].Read_histerese() == HIGH){
		num = sensores[1].Read_CalibradoPonderado() + sensores[2].Read_CalibradoPonderado();
		den = sensores[1].Read_Calibrado() + sensores[2].Read_Calibrado();
		return num / den;
	}

	if(pos == 0){
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos + 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos + 1].Read_Calibrado();
	}else if (pos == tam_sensores-1){
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos - 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos - 1].Read_Calibrado();
	}else{
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos - 1].Read_CalibradoPonderado() + sensores[pos + 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos - 1].Read_Calibrado() + sensores[pos + 1].Read_Calibrado();
	}
	return num / den;
}

void Sensor_linha::calibration_max(){
	for(unsigned i = 0; i < tam_sensores; i++) sensores[i].find_max();
}

void Sensor_linha::calibation_manual(){
	for(unsigned i = 0; i < tam_sensores; i++){
		sensores[i].Cmax = c_max[i];
	}
}

void Sensor_linha::testeLeitura(ReadType tipo){

	switch (tipo){
	case RAW:
		for (int i = 0; i < tam_sensores; i++){
			Serial.print("S");
			Serial.print(String(i));
			Serial.print(": ");
			Serial.print(sensores[i].Read_sensor());
			Serial.print("       ");
    }
		Serial.println();
		break;
	case CALIB:
		calibation_manual();
		for (int i = 0; i < tam_sensores; i++){
			Serial.print("S");
			Serial.print(String(i));
			Serial.print(": ");
			Serial.print(sensores[i].Read_Calibrado());
			Serial.print("       ");
		}
		Serial.println();
		break;
	case HIST:
		calibation_manual();
		for (int i = 0; i < tam_sensores; i++){
			Serial.print("S");
			Serial.print(String(i));
			Serial.print(": ");
			Serial.print(sensores[i].Read_histerese());
			Serial.print("       ");
    }
		Serial.println();
		break;

		case KALMAN:
			calibation_manual();
			Serial.print("Angle:");
			Serial.println(getAngleRaw());
			Serial.print(",");
			Serial.print("Kalman_filter:");
			Serial.println(getAngle());
		break;
	}
}

bool Sensor_linha::CheckCalibration(){
	for(unsigned i = 0; i < tam_sensores; i++){
		if(sensores[i].GetMax() == 0) return false;
	}
	return true;
}

bool Sensor_linha::CheckBuraco(){
  int talvez_buraco = false;
  for(int i = 0; i <= 4; i++){
     if(sensores[i].Read_histerese() == HIGH){
      if(talvez_buraco == true){
        return true;
      }else if(sensores[i+1].Read_histerese() == LOW and i < tam_sensores){
        talvez_buraco = true;
      }
    }
  }

  return false;

}
