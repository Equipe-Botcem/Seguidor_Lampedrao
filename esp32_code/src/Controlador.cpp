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

float Controlador::calcPID(float erro, bool outside){

    if(!outside) I += erro * 0.1;
    else D = (erro - erro_antigo) / 0.1;

    erro_antigo = erro;


	return Kp * erro + Ki * I + Kd * D;
}