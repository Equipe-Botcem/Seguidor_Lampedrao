#ifndef _CONTROLADOR_
#define _CONTROLADOR_

class Controlador{

public:

	// Constrtutores
	Controlador();

    void setControlador(float kp, float kd, float ki);

    void setKp(float kp);
    void setKd(float kd);
    void setKi(float ki);
    float calcPID(float erro);
    float PID(float _kp, float _ki, float _kd, float erro);
    float getAmostragem();
    void resetConditions();
    void teste(float erro);

    
private:
	
    float tempo_amostragem = 0.1;
    float atuation_k1;
    float erro_k1, erro_k2;
    float Kp, Kd, Ki;

    float Ts = 0.1;
    float _atuation_k1;
    float _erro_k1, _erro_k2;


};

#endif