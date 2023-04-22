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
    float calcTrans(float erro);

    // True - direita
    // False - esquerda

    bool getLastDir();
    float getAmostragem();

    
private:
	
    float tempo_amostragem = 0.1;
    double I, D, erro_antigo;
    float Kp;
	float Kd;
    float Ki;


};

#endif