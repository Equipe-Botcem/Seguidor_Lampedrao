#include "Sensor.h"

Sensor::Sensor()
{
}

Sensor::Sensor(unsigned char pin)
{
	pin_sensor = pin;
	Serial.print(pin);
}

void Sensor::Init()
{
	pinMode(pin_sensor, INPUT);
}

uint16_t Sensor::Read_sensor()
{
	uint16_t leitura = RESOLUTION - analogRead(pin_sensor);

	return leitura;
}

int Sensor::Read_Calibrado()
{
	uint16_t valor_descalibrado = Read_sensor();
	int c;
	c = (valor_descalibrado - Cmin) * (RESOLUTION / (Cmax - Cmin));

	if (c > RESOLUTION) c = RESOLUTION;

	if (c < 0) c = 0;

	return c;
}

void Sensor::find_min()
{
	int v_lido = 0;

	v_lido = Read_sensor();

	if (v_lido < Cmin) Cmin = v_lido;
}

void Sensor::find_max()
{
	int v_lido = 0;

	v_lido = Read_sensor();

	if (v_lido > Cmax) Cmax = v_lido;
}

bool Sensor::Read_histerese(){
	uint16_t v = Read_sensor();
	int x = (Cmax / 2);
	
	if (v < (x - 100)){
		ant = Preto;
		return Preto;
	}	
	else if (v > (x + 100)){
		ant = Branco;
		return Branco; 
	} 

	return ant;
	
}

void Sensor::setAngle(float angulo){
	angle = angulo;
}

float Sensor::Read_CalibradoPonderado(){
	return Read_Calibrado()*angle;
}