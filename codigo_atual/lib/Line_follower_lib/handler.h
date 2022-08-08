#include "motor.h"

String command = "";

const char SET = 'S';
const char STOP = 'P';
const char RUN = 'R';
const char CALIBRACAO = 'C';

bool Estado_corrida = false;

// reescrever para tirar sobrecarga de tarefas
void set_handler()
{
	String VB = "", KP_str = "", KI_str = "", KD_str = "", K_str = "", vel_min_str = "";
	int pos = command.indexOf(',', 2);
	for (int i = 4; i < pos; i++)
		KP_str += command[i];

	int pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		KI_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		KD_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		VB += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		K_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		vel_min_str += command[i];

	Kp = KP_str.toDouble() / 1000;
	Kd = KD_str.toDouble() / 1000;
	Ki = KI_str.toDouble() / 1000;
	vel_base = VB.toInt();
	K = K_str.toDouble() / 1000;
	vel_min = vel_min_str.toInt();
	qnt_linhas = 2;
	stop_condition = false;

	// Serial.println(STOP_VALUE_LATERAL);
}

/**
 * @brief Essa função é responsavel por lidar com a solicitação de parada vinda do aplicativo
 *
 * * Os pinos dos motores são modificados para estado LOW e o Estado_corrida fica falso
 *
 */
void stop_handler()
{
	digitalWrite(pin_direcional_motor_dir_1, LOW); // direcional
	digitalWrite(pin_direcional_motor_dir_2, LOW); // direcional

	digitalWrite(pin_direcional_motor_esq_1, LOW); // direcional
	digitalWrite(pin_direcional_motor_esq_2, LOW); // direcional

	Estado_corrida = false;
}

/**
 * @brief Essa função é responsavel por lidar com a solicitação de corrida vinda do aplicativo
 *
 * * Os pinos dos motores são modificados para estado HIGH e o Estado_corrida fica true
 *
 */
void run_handler()
{
	digitalWrite(pin_direcional_motor_dir_1, HIGH); // direcional
	digitalWrite(pin_direcional_motor_dir_2, LOW);	// direcional

	digitalWrite(pin_direcional_motor_esq_1, HIGH); // direcional
	digitalWrite(pin_direcional_motor_esq_2, LOW);	// direcional

	Estado_corrida = true;
	cagaco = millis();
}

/**
 * @brief Função responsavel por encontrar os valores maximos lidos por cada sensor durante a calibração.
 *
 * * Vários valores são lidos por cada sensor, e são comparados para que o maior valor seja encontrado.
 * * Ao fim do primeiro for() o vetor valLeitura_max conterá todos os valores maximos de cada sensor.
 * ! O segundo for() é responsavel por definir qual o valor (STOP_VALUE_FRONTAL)
 * ! que indica que uma leitura de encruzilhada foi realizada.
 * 
 * ? mudança para correção percentual valor maximo
 */
void find_max()
{
	double valoresLidos[6] = {0, 0, 0, 0, 0, 0};

	for (unsigned j = 0; j <= 5000; j++)
	{
		// delay(100);
		for (unsigned i = 0; i < 6; i++)
		{
			valoresLidos[i] = 1023 - analogRead(nomes_sensores[i]);

			if (valLeitura_max[i] < valoresLidos[i])
				valLeitura_max[i] = valoresLidos[i];
		}
	}
	double soma = 0;
	for (unsigned int i = 0; i < 6; i++)
	{
		soma += valLeitura_max[i];
	}
	STOP_VALUE_FRONTAL = soma / (NUM_SENSORS * 2);

	//* mudança
	for (unsigned int i = 0; i < NUM_SENSORS; i++)
	{
		//!valor maximo diminuido pencentualmente
		valLeitura_max[i] = valLeitura_max[i] * (1 - valor_pencentual_sensores);
	}
	//* fim da mudança
}

/**
 * @brief Função responsavel por encontrar os valores minimos lidos por cada sensor durante a calibração
 *
 * * Vários valores são lidos por cada sensor, e são comparados para que o menor valor seja encontrado.
 * * Ao fim do primeiro for() o vetor valLeitura_min conterá todos os valores minimos de cada sensor.
 *
 * ? mudança para correção poercentual do valor minimo
 */
void find_min()
{
	double valoresLidos[NUM_SENSORS] = {0, 0, 0, 0, 0, 0};
	double valoresLidos_soma[NUM_SENSORS] = {0, 0, 0, 0, 0, 0};
	double valoresLidos_media[NUM_SENSORS] = {0, 0, 0, 0, 0, 0};

	for (unsigned j = 0; j <= 5000; j++)
	{
		// delay(100);
		for (unsigned i = 0; i < 6; i++)
		{
			valoresLidos[i] = 1023 - analogRead(nomes_sensores[i]);
			valoresLidos_soma[i]= 1023 - analogRead(nomes_sensores[i]);

			if (valLeitura_min[i] > valoresLidos[i])
				valLeitura_min[i] = valoresLidos[i];
		}
	}

	//* mudança
	for (unsigned int i = 0; i < NUM_SENSORS; i++)
	{
		//!aplicando a correção percentual na média, visto que zerar erros baixos é melhor q perder altos
		valoresLidos_media[i]= valoresLidos_soma[i]/5000.0000;
		valLeitura_min[i] = valoresLidos_media[i] * (1 + valor_pencentual_sensores);
	}
	//* fim da mudança
}

/**
 * @brief Função responsavel por encontrar o valor maximo lido sensor lateral lido durante a calibração.
 *
 * * Vários valores são lidos pelo sensor, e são comparados para que o maior valor seja encontrado.
 * * Ao fim do for() o vetor valLeitura_max conterá todos os valores maximos de cada sensor.
 *
 * ! O valor (STOP_VALUE_LATERAL) é definido com base no valor maximo lido indicando que
 * ! uma leitura lateral foi realizada.
 */
void find_max_lateral()
{
	double valor_lido = 0;
	for (unsigned int i = 0; i < 1000; i++)
	{
		valor_lido = 1023 - analogRead(A1);

		if (val_leitura_max_lateral < valor_lido)
			val_leitura_max_lateral = valor_lido;
		delay(2);
	}
	STOP_VALUE_LATERAL = val_leitura_max_lateral / 3;

	// Serial.println("Valor da calibracao lateral: ");
	// Serial.println(STOP_VALUE_LATERAL);
}

/**
 * @brief Essa função é responsavel por lidar com a solicitação de calibração vinda do aplicativo
 *
 * * Os valores maximos de cada sensor frontal são encontrados.
 * * O robo se mexe para tras.
 * * Os valores minimos de cada sensor frontal são encontrados.
 * * O valor maximo do sensor lateral é encontrado.
 */
void calibracao_handler()
{
	Serial.println("start calibracao");

	// Calibra linha branca
	find_max();

	// Serial.println("Branco Calibrado");

	// Vai pra trás pro preta
	motor_calibration(1);

	// Vai pra linha preta
	find_min();

	// Serial.println("Preto Calibrado");

	Serial.println("end calibracao frontal");

	delay(1000);

	Serial.println("start calibracao lateral");

	// Vai pra frente pra calibrar o lateral
	motor_calibration(0);

	find_max_lateral();

	Serial.println("end calibracao lateral");
}

void msg_handler()
{
	switch (command[0])
	{
	case SET:
		set_handler();
		break;
	case STOP:
		stop_handler();
		break;
	case RUN:
		run_handler();
		break;
	case CALIBRACAO:
		if (command[1] != 'l')
			calibracao_handler();
		break;
	}
}

void comunica_serial()
{
	if (Serial.available())
	{
		command = Serial.readStringUntil(';');
		Serial.println(command);
		msg_handler();
		command = "";
	}
}
