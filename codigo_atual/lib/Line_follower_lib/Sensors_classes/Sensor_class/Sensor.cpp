#include "Sensor.h"

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
//! implementar
void Sensor_esq::Rotina()
{
}
//! implementar
void Sensor_dir::Rotina()
{
}