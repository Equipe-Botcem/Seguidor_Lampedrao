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

int Sensor_linha::Read_line(int i){
	return sensor_linha[i].Read_sensor(); 
}