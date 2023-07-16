#ifndef _SENSOR_
#define _SENSOR_
#define RESOLUTION 4095
#include "Arduino.h"

class Sensor
{
public:
	Sensor();
	Sensor(unsigned char pin);

	void Init();

	uint16_t Read_sensor();
	int Read_Calibrado();
	bool Read_histerese();

	void setAngle(float angulo);
	void find_min();	
	void find_max();	
	float Read_CalibradoPonderado();


	double Cmax = 0;
	double Cmin = 4095;
	
	
private:
	unsigned char pin_sensor;
	float angle;

	enum estado{
		Preto = 0,
		Branco
	};

	estado ant = Preto;
};

#endif