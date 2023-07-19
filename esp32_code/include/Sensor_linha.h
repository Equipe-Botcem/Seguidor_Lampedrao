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
    void calibration_min(); 
    float getAngleRaw();
    float getAngle();
    void calibation_manual();

	void testeLeitura(ReadType tipo);
	
private:
    Sensor sensores[8];
	double angulos[8] = {-42.855, -31.430, -17.571, -4.538, 4.538, 17.571, 31.430, 41.855};
    float last_read;

    int c_max[8] = {1300, 1230, 1500, 1460, 980, 1400, 1500, 1400};
    int c_min[8] = {0, 0, 0, 0, 0, 0, 0, 0};


    // Métodos auxiliares
    float mediaPond(int pos);
};

#endif