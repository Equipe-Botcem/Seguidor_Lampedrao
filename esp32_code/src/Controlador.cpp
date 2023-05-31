#include "Controlador.h"
#include "Arduino.h"


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

float Controlador::calcPID(float angulo){

    erro = setpoint - angulo;
    if(abs(angulo) > 20){
        I += erro * 0.1;   
    }else{
        I = 0;
        D = (erro - erro_antigo) / tempo_amostragem;
    } 
    erro_antigo = erro;

	return Kp * erro + Ki * I + Kd * D;
}

float Controlador::getAmostragem(){
    return tempo_amostragem;
}

int Controlador::getLastError(){
    if(erro_antigo < 0) return 1;

    return 0;
}