#include "handler.h"

double erro = 0;

void setup()
{

	Serial.begin(115200);
	pin_config(); 
	//calibracao_handler();
}

void loop()
{
	// Serial.println("leituras dos sensores");
	// erro = calc_erro();
	// Serial.print("erro =");
	// Serial.println(erro);
	// delay(1000);

	comunica_serial();
	//calc_erro();
	if (Estado_corrida)
	{
		if (!stop_condition && !timer_cagaco())
		{
			erro = 0;
			erro = calc_erro();
			motor_controler(erro);
		}
		else
		{
			delay(100);
			digitalWrite(pin_direcional_motor_dir_1, LOW); // direcional
			digitalWrite(pin_direcional_motor_dir_2, LOW); // direcional

			digitalWrite(pin_direcional_motor_esq_1, LOW); // direcional
			digitalWrite(pin_direcional_motor_esq_2, LOW); // direcional
			Estado_corrida = false;
		}
	}
}