#ifndef _SENSOR_
#define _SENSOR_
#define RESOLUTION 4095



#include "include.h"

class Sensor
{
public:
	Sensor();
	Sensor(unsigned char pin);

	void Init();

	uint16_t Read_sensor();
	
	int Read_Calibrado();
	bool Read_histerese();
	void find_min();	
	void find_max();	

	//! Trocar quando for testar na pista
	double Cmax = 1200;
	double Cmin = 0;
	
	float angle;
private:
	unsigned char pin_sensor;

	enum estado{
		Preto = 0,
		Branco
	};

	estado ant = Preto;
};

#endif