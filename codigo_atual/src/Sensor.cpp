#include "Sensors/Sensor/Sensor.h"

Sensor::Sensor()
{
}

Sensor::Sensor(unsigned char pin)
{
	pin_sensor = pin;
}

Sensor::Sensor(unsigned char pin, double val_max, double val_min)
{
	pin_sensor = pin;
	calib_val_max = val_max;
	calib_val_min = val_min;
}

void Sensor::Init()
{

	pinMode(pin_sensor, INPUT);
}

int Sensor::Read_sensor()
{
	return analogRead(pin_sensor);
}

void Sensor::Rotina()
{
}

//! implementar
void Sensor::Calibracao()
{

	
}

int Sensor::find_min(){
	int v_lido=0;
	int v_min=1023;
	for(unsigned int i=0;i<20;i++){

		v_lido = analogRead(pin_sensor);
		if(v_lido<v_min) v_min = v_lido;
	}
	return v_min;
}

int Sensor::find_max(){
	int v_lido=0;
	int v_max=0;
	for(unsigned int i=0;i<20;i++){

		v_lido = analogRead(pin_sensor);
		if(v_lido>v_max) v_max = v_lido;
	}
}

void Sensor::setValorMed(double val_med){
	calib_val_med = val_med;
}

//! implementar
void Sensor_esq::Rotina()
{
}
//! implementar
void Sensor_dir::Rotina()
{
}