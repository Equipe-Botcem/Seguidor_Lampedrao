#include "Sensor.h"
// #include "vector"

#if defined(DIAG_MODE)
uint16_t Sensor::telemetria()
{
	Serial.println("|===============TELEMETRIA DO SENSOR DO ROBO===============|");
	Serial.print("\tSENSOR VERIFICADO: ");
	Serial.println(pin_sensor);
	Serial.print("\tTIME: ");
	Serial.print((millis() - timer_exec) / 1000);
	Serial.println(" sec");
	Serial.println("---------------------------------------------------------------");
	uint16_t leitura = RESOLUTION - analogRead(pin_sensor);
	Serial.print("\tLEITURA: ");
	Serial.println(leitura);
	Serial.println("|==============================================================|");
	return leitura;
}
#endif

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
#if (!defined(DIAG_MODE) || !defined(READ_SENSOR))
	uint16_t leitura = RESOLUTION - analogRead(pin_sensor);

	return leitura;

#else
	return telemetria();
#endif
}

// TODO Validar calibracao e desenvolver telemetria
uint16_t Sensor::Read_Calibrado(uint16_t valor_descalibrado)
{
	uint16_t c = 0;
	c = (valor_descalibrado - Cmin) * (RESOLUTION / (Cmax - Cmin));

	if (c > RESOLUTION)
		c = RESOLUTION;
	if (c < RESOLUTION)
		c = 0;

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
