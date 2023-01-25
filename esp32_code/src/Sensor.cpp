#include "Sensor.h"

Sensor::Sensor()
{
}

Sensor::Sensor(unsigned char pin)
{
	pin_sensor = pin;
}

void Sensor::Init()
{

	pinMode(pin_sensor, INPUT);
}

uint16_t Sensor::Read_sensor()
{
	uint16_t leitura = 255 - analogRead(pin_sensor);

	if(leitura < 40) return 0;

	return leitura;
}

uint16_t Sensor::Read_Calibrado(uint16_t valor_descalibrado)
{
	uint16_t c = 0;
	c = (valor_descalibrado - Vmin) * (255 / (Vmax - Vmin));


	if (c > 255)
		c = 255;
	if (c < 0)
		c = 0;

	return c;
}

void Sensor::find_min()
{
	int v_lido = 0;

	
	v_lido = Read_sensor();

	if (v_lido < Vmin)	Vmin = v_lido;

}

void Sensor::find_max()
{
	int v_lido = 0;
	
	v_lido = Read_sensor();

	if (v_lido > Vmax)	Vmax = v_lido;
}
