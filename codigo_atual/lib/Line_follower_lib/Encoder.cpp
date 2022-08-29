#include "Encoder.h"

int Encoder::contador_a = 0;
unsigned char Encoder::pin_a = 1;

Encoder::Encoder()
{
	contador_a = 0;
}

Encoder::Encoder(unsigned char a)
{
	pin_a = a;
	contador_a = 0;
}

Encoder::~Encoder()
{
}

void Encoder::Init()
{
	pinMode(pin_a, INPUT);
	attachInterrupt(digitalPinToInterrupt(pin_a), Rotina, RISING);
}

void Encoder::Rotina()
{
	contador_a++;
}

int Encoder::Get_contador()
{
	return contador_a;
}

void Encoder::Reset()
{
	contador_a = 0;
}