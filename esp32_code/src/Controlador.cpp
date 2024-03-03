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

float Controlador::calcPID(float erro){

    float temp = atuation_k1 + (Kp + Ki + Kd)*erro + (Ki - Kp - 2*Kd)*erro_k1 + Kd*erro_k2;

    atuation_k1 = temp;

    erro_k2 = erro_k1;
    erro_k1 = erro;
    return temp;
}

float Controlador::PID(float _kp, float _ki, float _kd, float erro)
{
    float temp = _atuation_k1 + (_kp + _ki + _kd)*erro + (_ki - _kp - 2*_kd)*_erro_k1 + _kd*_erro_k2;

    _atuation_k1 = temp;

    _erro_k2 = _erro_k1;
    _erro_k1 = erro;
    return temp;
}

 
float Controlador::getAmostragem(){
    return tempo_amostragem;
}


void Controlador::resetConditions(){
    erro_k1 = 0;
    erro_k2 = 0;
    atuation_k1 = 0;
}

void Controlador::teste(float erro){
    Serial.print("PID: ");
    Serial.println(calcPID(erro));
}