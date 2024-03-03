#include "Encoder.h"
#include "Arduino.h"

volatile int Encoder::contador_a = 0;
volatile int Encoder::contador_b = 0;

Encoder::Encoder()
{
	contador_a = 0;
	contador_b = 0;
}

Encoder::Encoder(unsigned char a)
{
	pin_a = a;
	contador_a = 0;
	contador_b = 0;
}

Encoder::~Encoder()
{
}

void Encoder::Init_dir()
{
	pinMode(pin_a, INPUT);
	attachInterrupt(digitalPinToInterrupt(pin_a), Rotina_dir, RISING);
	Reset_dir();
}
void Encoder::Init_esq()
{
	pinMode(pin_a, INPUT);
	attachInterrupt(digitalPinToInterrupt(pin_a), Rotina_esq, RISING);
	Reset_esq();
}

void Encoder::Rotina_dir()
{
	contador_a++;
	//Serial.println("valor atualizado");
}
void Encoder::Rotina_esq()
{
	contador_b++;
}

int Encoder::Get_contador_dir()
{
	return contador_a;
}
int Encoder::Get_contador_esq()
{
	return contador_b;
}

void Encoder::Reset_dir()
{
	contador_a = 0;
}
void Encoder::Reset_esq()
{
	contador_b = 0;
}