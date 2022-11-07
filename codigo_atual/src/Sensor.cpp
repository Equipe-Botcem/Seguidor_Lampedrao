#include "Sensors/Sensor/Sensor.h"

Sensor::Sensor()
{
}

Sensor::Sensor(unsigned char pin)
{
	pin_sensor = pin;
}

Sensor::Sensor(unsigned char pin, double val_med)
{
	pin_sensor = pin;
	calib_val_med = val_med;
}

void Sensor::Init()
{

	pinMode(pin_sensor, INPUT);
}

uint16_t Sensor::Read_sensor()
{
	return analogRead(pin_sensor);
}

void Sensor::Rotina()
{
}

// TODO testar
int Sensor::Read_Calibrado()
{
	int valor = analogRead(pin_sensor);
	if (valor <= calib_val_med * 0.9)
	{
		ant = Preto;
		return 0;
	}
	else if (valor >= calib_val_med * 1.1)
	{
		ant = Branco;
		return 1023;
	}
	else
	{
		if (ant == Preto)
			return 0;
		else
			return 1023;
	}
}

int Sensor::find_min()
{
	int v_lido = 0;
	int v_min = 1023;
	for (unsigned int i = 0; i < 20; i++)
	{

		v_lido = analogRead(pin_sensor);
		if (v_lido < v_min)
			v_min = v_lido;
	}
	return v_min;
}

int Sensor::find_max()
{
	int v_lido = 0;
	int v_max = 0;
	for (unsigned int i = 0; i < 20; i++)
	{

		v_lido = analogRead(pin_sensor);
		if (v_lido > v_max)
			v_max = v_lido;
	}

	return v_max;
}

void Sensor::setValorMed(double val_med)
{
	calib_val_med = val_med;
}
