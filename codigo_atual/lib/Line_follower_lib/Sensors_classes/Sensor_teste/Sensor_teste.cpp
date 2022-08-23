#include "Sensor_teste.h"

void Sensor_teste_init()
{
	Sensor sensor_teste(pin);
	sensor_teste.Init();
}

double Sensor_teste_read()
{
	Sensor sensor_teste(pin);
	sensor_teste.Init();
	return sensor_teste.Read_sensor();
}

//! Implementar os testes futuros para calibração() e Rotina() e classe