#ifndef _CONTROLADOR_
#define _CONTROLADOR_

#include "motor_drive.h"
#include "Sensor.h"
#include "Encoder.h"
#include "BluetoothSerial.h"


class Controlador{

public:

	// Constrtutores
	Controlador();

    void setControlador(float kp, float kd, float ki);

    void setKp(float kp);
    void setKd(float kd);
    void setKi(float ki);
    float calcPID(float erro);
    // 1 Direita 
    // 0 Esquerda
    int getLastError();

    float getAmostragem();

    
private:
	
    float tempo_amostragem = 0.1;
    float setpoint = 0.0;
    float I, D, erro_antigo;
    float erro;
    float Kp;
	float Kd;
    float Ki;


};

#endif