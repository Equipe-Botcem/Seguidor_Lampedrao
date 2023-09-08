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

	void testeLeitura(ReadType tipo);
	
private:
    Sensor sensores[8];
	double angulos[8] = {-42.855, -31.430, -17.571, -4.538, 4.538, 17.571, 31.430, 41.855};
    float last_read = 0;
    int limit_buraco = 0;
    unsigned long buraco_timer = 0;
    bool gate_buraco = false;
    int tam_sensores = 8;
    float read;
    bool in = true;

    int c_max[8] = {890, 640, 790, 880, 600, 880, 820, 1280};

    // Métodos auxiliares
    float mediaPond(int pos);
};

#endif