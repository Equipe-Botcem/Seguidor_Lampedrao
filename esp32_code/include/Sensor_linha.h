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
    Sensor sensores[6];
	double angulos[6] = {-31.430, -17.571, -4.538, 4.538, 17.571, 31.430};
    float last_read;
    int limit_buraco = 0;
    unsigned long buraco_timer = 0;
    bool gate_buraco = false;
    int tam_sensores = 6;

    int c_max[8] = {1300, 1230, 1500, 1460, 980, 1400, 800, 1400};

    // Métodos auxiliares
    float mediaPond(int pos);
};

#endif