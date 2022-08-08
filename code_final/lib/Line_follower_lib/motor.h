#include "Arduino.h"

// Defines
#define NUM_SENSORS 6

// Constantes
int nomes_sensores[NUM_SENSORS] = {A0, A2, A3, A4, A5, A6};

int pin_PWM_motor_dir = 6;			 //   PWMB_PONTEH
int pin_direcional_motor_dir_1 = 10; //   BIN2_PONTEH
int pin_direcional_motor_dir_2 = 9;	 //   BIN1_PONTEH

int pin_PWM_motor_esq = 5;			//   PWMA_PONTEH
int pin_direcional_motor_esq_1 = 7; //   AIN2_PONTEH
int pin_direcional_motor_esq_2 = 8; //   AIN1_PONTEH

double Kp = 0.1;
double Ki = 0;
double Kd = 0;
double K = 0.05;

double P = 0;
// double I = 0;
// double D = 0;

double vel_motor_direito = 0;
double vel_motor_esquerdo = 0;

double vel_max = 255;
double vel_min = 15;
double vel_base = 120;
double STOP_VALUE_LATERAL = 300;
double STOP_VALUE_FRONTAL = 300;

double erro_antigo = 0;

bool stop_condition = false;
unsigned long start_time = 0;
int qnt_linhas = 2;

unsigned long second = 1000;

double valLeitura_min[6] = {1023, 1023, 1023, 1023, 1023, 1023};
double valLeitura_max[6] = {0, 0, 0, 0, 0, 0};
double val_leitura_max_lateral =0;

double valor_med_max = 0;
double valor_med_min = 1023;

unsigned long cagaco = 0;

//-------------------//

void pin_config()
{
	// Input
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);
	pinMode(A6, INPUT);
	pinMode(A7, INPUT);

	// Output - motor dir
	pinMode(pin_PWM_motor_dir, OUTPUT);
	pinMode(pin_direcional_motor_dir_1, OUTPUT);
	pinMode(pin_direcional_motor_dir_2, OUTPUT);
	digitalWrite(pin_direcional_motor_dir_1, HIGH); // direcional
	digitalWrite(pin_direcional_motor_dir_2, LOW);	// direcional

	// Output - motor esq
	pinMode(pin_PWM_motor_esq, OUTPUT);
	pinMode(pin_direcional_motor_esq_1, OUTPUT);
	pinMode(pin_direcional_motor_esq_2, OUTPUT);
	digitalWrite(pin_direcional_motor_esq_1, HIGH); // direcional
	digitalWrite(pin_direcional_motor_esq_2, LOW);	// direcional
}

bool timer_cagaco(){
	// if(millis() - cagaco >= 83*second) return true;
	return false;
}

bool ignorar_leitura()
{
	if ((millis() - start_time) <= (0.3 * second))
		return true;
	return false;
}

void sensor_stop_condition(int value)
{
	
	if (value >= STOP_VALUE_LATERAL)
	{
		//Serial.println(value);
		if (!ignorar_leitura())
		{
			qnt_linhas--;
			//Serial.println("qnt_linha: ");
			//Serial.println(qnt_linhas);
			start_time = millis();
			if (qnt_linhas == 0)
				stop_condition = true;
		}
	}
}

void sensor_encruzilhada_condition(double soma)
{
	if (soma >= (STOP_VALUE_FRONTAL) * (NUM_SENSORS))
	{
		//Serial.println("encruzilhada");
		start_time = millis();
	}
}

double calibracao_frontal(double valor_descalibrado, double valLeitura_max, double valLeitura_min)
{
	double c = 0;

	c = (valor_descalibrado - valLeitura_min) * (1023 / (valLeitura_max - valLeitura_min));

	if(c>1023) c=1023;
	if(c<0)	c=0;
	/*Serial.println("-------------------------------------");
	Serial.print("valor descalibrado = ");
	Serial.println(valor_descalibrado);
	Serial.print("valor_max = ");
	Serial.println(valLeitura_max);
	Serial.print("valor min = ");
	Serial.println(valLeitura_min);
	*/

	return c;
}

/**
 * @brief
 *
 * @return double
 */
double calc_erro()
{

	double valoresLidos[6];
	double soma = 0;
	double erro_calc = 0;

	for (unsigned int i = 0; i < NUM_SENSORS; i++)
	{
		valoresLidos[i] = 1023 - analogRead(nomes_sensores[i]);

		soma += valoresLidos[i];
	}

	sensor_encruzilhada_condition(soma);
	sensor_stop_condition(1023 - analogRead(A1));

	double pesos[6] = {-4, -2, -1, 1, 2, 4};
	int valor_calibrado;

	for (unsigned int i = 0; i < NUM_SENSORS; i++)
	{
		valor_calibrado = calibracao_frontal(valoresLidos[i], valLeitura_max[i], valLeitura_min[i]);
		erro_calc += valor_calibrado * pesos[i];
		//Serial.print(i);
		//Serial.print("->");
		//Serial.print(valor_calibrado);
		//Serial.println();
	}
	//Serial.println();
	//delay(1000);

	return erro_calc;
}

/**
 * @brief Calcula a resposta angular para a velocidades dos motores
 *
 * @param erro erro da leitura dos sensores
 * @return Kp * erro + Ki * a soma de todos os erros
 */
double PID_calc(double erro)
{
	double value = 0;
	P = erro;
	// I = I + erro;
	//  D = erro_antigo - erro;
	//  erro_antigo = erro;

	value = (Kp * P);
	// double valor = Kp * P + Ki * I + Kd * D;
	return value;
}

int motor_check(double vel_motor)
{
	if (vel_motor > 255)
	{
		vel_motor = 255;
	}
	else if (vel_motor < vel_min)
	{
		vel_motor = vel_min;
	}
	return vel_motor;
}

/**
 * @brief Quanto maior o erro menor o retorno
 *
 * @param erro erro da leitura dos sensores
 * @return velocidade maxima - velocidade base - K * erro
 */
double Vel_direcional(double erro)
{
	double value = (vel_max - vel_base) - K * abs(erro);
	if (value <= 0)
		return 0;
	return value;
}

double motor_controler_esq(double erro)
{
	vel_motor_esquerdo = vel_base + Vel_direcional(erro) - PID_calc(erro);
	return motor_check(vel_motor_esquerdo);
}

double motor_controler_dir(double erro)
{
	vel_motor_direito = vel_base + Vel_direcional(erro) + PID_calc(erro);
	return motor_check(vel_motor_direito);
}

void motor_controler(double erro)
{
	analogWrite(pin_PWM_motor_dir, motor_controler_dir(erro));
	analogWrite(pin_PWM_motor_esq, motor_controler_esq(erro));
}

/**
 * @brief Resposavel por controlar o robô durante a calibração 
 * 
 */
void motor_calibration(int dir){

	// Se dir for 1 joga pra tras 
	if (dir){

		// Configura o motor pra tras 
		// Sentido do motor direito 
		digitalWrite(pin_direcional_motor_dir_1, LOW); // direcional
		digitalWrite(pin_direcional_motor_dir_2, HIGH);	// direcional

		// Sentido do motor esquerdo 
		digitalWrite(pin_direcional_motor_esq_1, LOW); // direcional
		digitalWrite(pin_direcional_motor_esq_2, HIGH);	// direcional

		analogWrite(pin_PWM_motor_dir, 120);
		analogWrite(pin_PWM_motor_esq, 120);

		delay(200);

		analogWrite(pin_PWM_motor_dir, 0);
		analogWrite(pin_PWM_motor_esq, 0);

		// Reconfigura pra frente 
		// Sentido do motor direito 
		digitalWrite(pin_direcional_motor_dir_1, HIGH); // direcional
		digitalWrite(pin_direcional_motor_dir_2, LOW);	// direcional

		// Sentido do motor esquerdo 
		digitalWrite(pin_direcional_motor_esq_1, HIGH); // direcional
		digitalWrite(pin_direcional_motor_esq_2, LOW);	// direcional
	
	}else{

		analogWrite(pin_PWM_motor_dir, 120);
		analogWrite(pin_PWM_motor_esq, 120);

		delay(550);

		analogWrite(pin_PWM_motor_dir, 0);
		analogWrite(pin_PWM_motor_esq, 0);
			
	}

}