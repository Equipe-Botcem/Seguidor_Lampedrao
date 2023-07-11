#ifndef _SENSOR_LINHA_
#define _SENSOR_LINHA_
#include "Arduino.h"
#include "Sensor.h"

class Sensor_linha
{
public:
	Sensor_linha();
	Sensor_linha(unsigned char* pins);
	void Init();

    void calibration_max(); 
    void calibration_min(); 
	void testeLeituraRaw();
    float getAngle();
	
private:
    Sensor sensores[8];
	double angulos[8] = {-42.855, -31.430, -17.571, -4.538, 4.538, 17.571, 31.430, 41.855};
    float last_read;

    // Métodos auxiliares
    float mediaPond(int pos);
};

#endif