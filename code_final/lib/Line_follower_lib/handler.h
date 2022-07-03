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

void stop_handler()
{
	digitalWrite(pin_direcional_motor_dir_1, LOW); // direcional
	digitalWrite(pin_direcional_motor_dir_2, LOW); // direcional

	digitalWrite(pin_direcional_motor_esq_1, LOW); // direcional
	digitalWrite(pin_direcional_motor_esq_2, LOW); // direcional

	Estado_corrida = false;
}

void run_handler()
{
	digitalWrite(pin_direcional_motor_dir_1, HIGH); // direcional
	digitalWrite(pin_direcional_motor_dir_2, LOW);	// direcional

	digitalWrite(pin_direcional_motor_esq_1, HIGH); // direcional
	digitalWrite(pin_direcional_motor_esq_2, LOW);	// direcional

	Estado_corrida = true;
	cagaco = millis();
}

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
	STOP_VALUE_FRONTAL = soma /( NUM_SENSORS * 2);
}

void find_min()
{
	double valoresLidos[6] = {0, 0, 0, 0, 0, 0};

	for (unsigned j = 0; j <= 5000; j++)
	{
		// delay(100);
		for (unsigned i = 0; i < 6; i++)
		{
			valoresLidos[i] = 1023 - analogRead(nomes_sensores[i]);

			if (valLeitura_min[i] > valoresLidos[i])
				valLeitura_min[i] = valoresLidos[i];
		}
	}
}

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
	STOP_VALUE_LATERAL = val_leitura_max_lateral/3;

	//Serial.println("Valor da calibracao lateral: ");
	//Serial.println(STOP_VALUE_LATERAL);

	
}

void calibracao_handler()
{
	Serial.println("start calibracao");

	// Calibra linha branca
	find_max();

	//Serial.println("Branco Calibrado");

	// Vai pra trás pro preta 
	motor_calibration(1);

	// Vai pra linha preta 
	find_min();

	//Serial.println("Preto Calibrado");
	
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
		if(command[1] != 'l')
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


