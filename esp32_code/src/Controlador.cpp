#include "Controlador.h"
#include "Arduino.h"

// Construtor padrão
Controlador::Controlador(){}

// Função para ajustar o coeficiente proporcional
void Controlador::setKp(float kp){
    Kp = kp;
}

// Função para ajustar o coeficiente diferencial
void Controlador::setKd(float kd){
    Kd = kd;
}

// Função para ajustar o coeficiente integral
void Controlador::setKi(float ki){
    Ki = ki;
}

// Função para ajustar todos os coeficientes do controlador
void Controlador::setControlador(float kp, float kd, float ki){
    setKp(kp); // Ajusta o coeficiente proporcional
    setKd(kd); // Ajusta o coeficiente diferencial
    setKi(ki); // Ajusta o coeficiente integral
}   

// Função para calcular o PID
float Controlador::calcPID(float erro){
    // Calcula a atuação do controlador com base nos coeficientes e no erro
    float temp = atuation_k1 + (Kp + Ki + Kd)*erro + (Ki - Kp - 2*Kd)*erro_k1 + Kd*erro_k2;
    atuation_k1 = temp; // Atualiza a atuação anterior
    erro_k2 = erro_k1; // Atualiza o erro anterior
    erro_k1 = erro; // Atualiza o erro atual

    return temp; // Retorna a atuação do controlador
}

// Função para obter o tempo de amostragem
float Controlador::getAmostragem(){
    return tempo_amostragem;
}

// Função para resetar as condições do controlador
void Controlador::resetConditions(){
    erro_k1 = 0; // Reseta o erro anterior
    erro_k2 = 0; // Reseta o erro anterior ao anterior
    atuation_k1 = 0; // Reseta a atuação anterior
}

// Função de teste para o controlador
void Controlador::teste(float erro){
    Serial.print("PID: "); // Imprime "PID: "
    Serial.println(calcPID(erro)); // Imprime o valor calculado pelo PID
}
