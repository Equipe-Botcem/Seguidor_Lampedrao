#include "Motor.h"
#include "Arduino.h"

// Construtor padrão
Motor::Motor()
{
}

// Construtor que configura os pinos do motor
Motor::Motor(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char OCM)
{
	pin_PWM_1 = PWM_1; // Pino para o PWM 1
	pin_PWM_2 = PWM_2; // Pino para o PWM 2
	pin_EN = EN; // Pino para habilitar o motor
	pin_OCM = OCM; // Pino para o OCM (Over-Current Monitoring)
}

// Função para inicializar os pinos do motor
void Motor::Init()
{
	pinMode(pin_PWM_1, OUTPUT); // Configura o pino do PWM 1 como saída
	pinMode(pin_PWM_2, OUTPUT); // Configura o pino do PWM 2 como saída
	pinMode(pin_EN, OUTPUT); // Configura o pino de habilitação como saída
	pinMode(pin_OCM, INPUT); // Configura o pino do OCM como entrada
}

// Função para habilitar o driver do motor
void Motor::Enable_drive()
{
	digitalWrite(pin_EN, HIGH); // Ajusta o pino de habilitação para alto
}

// Função para desabilitar o driver do motor
void Motor::Disable_drive()
{
	Set_speed(0); // Define a velocidade do motor para 0
	digitalWrite(pin_EN, LOW); // Ajusta o pino de habilitação para baixo
}

// Função para definir a velocidade do motor
void Motor::Set_speed(int speed)
{
	// Verifica se a velocidade está dentro do intervalo permitido
	if(speed > 255) speed = 255; // Se a velocidade é maior que 255, ajusta para 255
	if(speed < -255) speed = -255; // Se a velocidade é menor que -255, ajusta para -255

	// Se a velocidade é positiva, ajusta o PWM 1 para a velocidade e o PWM 2 para 0
	if (speed >= 0)
	{
		analogWrite(pin_PWM_1, speed); 
		analogWrite(pin_PWM_2, LOW);
	}
	else // Se a velocidade é negativa, ajusta o PWM 2 para a velocidade (valor absoluto) e o PWM 1 para 0
	{
		speed = -1*speed; 
		analogWrite(pin_PWM_2, speed);
		analogWrite(pin_PWM_1, LOW);
	}
}
