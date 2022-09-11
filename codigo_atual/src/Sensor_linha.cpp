#include "Sensors/Sensor_linha/Sensor_linha.h"

Sensor_linha::Sensor_linha()
{
}

Sensor_linha::Sensor_linha(unsigned char *pins)
{

	for (unsigned int i = 0; i < num_sensores_linha; i++)
	{
		pins_sensores[i] = pins[i];
	}
}

void Sensor_linha::Init()
{
	for (unsigned int i = 0; i < num_sensores_linha; i++)
	{
		Sensor my_sensor(pins_sensores[i]);
		my_sensor.Init();
		sensor_linha[i] = my_sensor;
	}
}

double *Sensor_linha::Read_line()
{
	double Leituras[num_sensores_linha];

	for (unsigned int i = 0; i < num_sensores_linha; i++)
	{
		Leituras[i] = sensor_linha[i].Read_sensor();
	}

	return Leituras;
}