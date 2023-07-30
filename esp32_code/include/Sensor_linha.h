#ifndef _SENSOR_LINHA_
#define _SENSOR_LINHA_
#include "Arduino.h"
#include "Sensor.h"


class Sensor_linha
{
public:
    enum ReadType {RAW = 0, CALIB, HIST, KALMAN};

	Sensor_linha();
	Sensor_linha(unsigned char* pins);
	void Init();

    void calibration_max(); 
    float getAngleRaw();
    float getAngle();
    void calibation_manual();
    bool CheckCalibration();
    bool CheckBuraco();

	void testeLeitura(ReadType tipo);
	
private:
    Sensor sensores[4];
	double angulos[4] = {-17.571, -4.538, 4.538, 17.571};
    float last_read = 0;
    int limit_buraco = 0;
    unsigned long buraco_timer = 0;
    bool gate_buraco = false;
    int tam_sensores = 4;
    float read;
    bool in = true;

    int c_max[4] = {1100, 1176, 850, 1100};

    // Métodos auxiliares
    float mediaPond(int pos);
};

#endif