#ifndef _SENSOR_
#define _SENSOR_

#include "include.h"

class Sensor
{
public:
	Sensor();
	Sensor(unsigned char pin);
	Sensor(unsigned char pin, double val_med);

	void Init();

	int Read_sensor();
	
	int Read_Calibrado();
	virtual void Rotina();
	int find_min();	
	int find_max();	

	void setValorMed(double val_med);

private:
	unsigned char pin_sensor;
	//TODO inicializar valor médio caso não seja feita calibração 
	double calib_val_med;
	enum estado{
		Preto = 0,
		Branco
	};
	estado ant;
};

#endif