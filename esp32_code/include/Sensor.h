#ifndef _SENSOR_
#define _SENSOR_

#include "include.h"

class Sensor
{
public:
	Sensor();
	Sensor(unsigned char pin);

	void Init();

	uint16_t Read_sensor();
	
	uint16_t Read_Calibrado(uint16_t valor_descalibrado);
	void find_min();	
	void find_max();	

	double Vmax = 0;
	double Vmin = 255;
	

private:
	unsigned char pin_sensor;
	

	enum estado{
		Preto = 0,
		Branco
	};
	estado ant;
};

#endif