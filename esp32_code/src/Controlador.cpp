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

    // Calculo do I
    if(erro < 100){
	    I = 0;
    }else{
        I = erro + I;
    }

    // Calculo do D
	double D = (erro - erro_antigo);
	erro_antigo = erro;

	double valor = Kp * erro + Ki * I + Kd * D;
	return valor;
}