#include "Sensor.h"

// Construtor padrão da classe Sensor
Sensor::Sensor(){}

// Construtor da classe Sensor que aceita um pino como argumento
Sensor::Sensor(unsigned char pin){
	pin_sensor = pin;
	Serial.print(pin);
}

// Método para inicializar o sensor
void Sensor::Init(){
	pinMode(pin_sensor, INPUT);
}

// Método para ler o valor do sensor
uint16_t Sensor::Read_sensor(){
	uint16_t leitura = RESOLUTION - analogRead(pin_sensor);
	return leitura;
}

// Método para ler o valor calibrado do sensor
int Sensor::Read_Calibrado(){
	uint16_t valor_descalibrado = Read_sensor();
	int c;
	c = (valor_descalibrado - Cmin) * (RESOLUTION / (Cmax - Cmin));

	if (c > RESOLUTION) c = RESOLUTION;

	if (c < 0) c = 0;

	return c;
}

// Método para encontrar o valor máximo que o sensor pode ler
void Sensor::find_max()
{
	int v_lido = 0;
	v_lido = Read_sensor();

	if (v_lido > Cmax) Cmax = v_lido;
}

// Método para ler o sensor com histerese
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

// Método para definir o ângulo do sensor
void Sensor::setAngle(float angulo){
	angle = angulo;
}

// Método para ler o valor calibrado e ponderado do sensor
float Sensor::Read_CalibradoPonderado(){
	return Read_Calibrado()*angle;
}

// Método para obter o valor máximo do sensor
float Sensor::GetMax(){
	return Cmax;
}
