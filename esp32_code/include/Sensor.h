#ifndef _SENSOR_
#define _SENSOR_
#define RESOLUTION 4095


#define READ_SENSOR

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

	double Cmax = 0;
	double Cmin = RESOLUTION;
#if defined(DIAG_MODE)
	uint16_t telemetria();
#endif
	
private:
	unsigned char pin_sensor;
	
	enum estado{
		Preto = 0,
		Branco
	};

	estado ant;
};

#endif