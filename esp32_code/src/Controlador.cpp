#include "Controlador.h"


Controlador::Controlador(){

}

void Controlador::setKp(float kp){
    Kp = kp;
}


void Controlador::setKd(float kd){
    Kd = kd;
}

void Controlador::setKi(float ki){
    Ki = ki;
}


void Controlador::setControlador(float kp, float kd, float ki){
    setKp(kp);
    setKd(kd);
    setKi(ki);
}   

int Controlador::calcRot(float erro){
    double value = 0;
	// I = I + erro;
	//double D = (erro - erro_antigo);
	//  erro_antigo = erro;

	value = (Kp * erro);
	// double valor = Kp * P + Ki * I + Kd * D;
	return value;
}