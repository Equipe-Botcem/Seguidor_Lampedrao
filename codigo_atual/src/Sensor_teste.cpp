#include "Sensors/test/Sensor_teste.h"

void Sensor_teste_init()
{
	Sensor sensor_teste(pin);
	sensor_teste.Init();
	Serial.println("Sensor iniciado");
}

double Sensor_teste_read()
{
	Sensor sensor_teste(pin);
	sensor_teste.Init();
	return sensor_teste.Read_sensor();
}

void Sensor_linha_test_init()
{
	Sensor_linha s_l(pins);
	s_l.Init();
	Serial.println("Sensor iniciado");
}

void Sensor_linha_test_read(){
	Sensor_linha s_l(pins);
	s_l.Init();
	double* vector = s_l.Read_line();
	for(unsigned int i=0; i< 2;i++){
		Serial.print(vector[i]);
		Serial.print("\t");
	}
}


//! Implementar os testes futuros para calibração() e Rotina() e classe