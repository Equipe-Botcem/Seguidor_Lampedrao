#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include <Controlador/Controlador.h>

class Seguidor{

public:

	Seguidor();
	Seguidor(double kp, double ki, double kd);

	void Init();

	void Behavior(int bh);

	void Set_parametros(double kp, double ki, double kd);

	void Auto_calibrate();
	void Run();

private:

	enum Comando {SET = 0, STOP, RUN, CALIBRACAO};

	Controlador u_c;

	unsigned char pins_motor_drive_esq[5] = {2,15,18,21};
	unsigned char pins_motor_drive_dir[5] = {5,22,4,21};
	unsigned char pin_encoder_esq = 16;
	unsigned char pin_encoder_dir = 17;
	unsigned char pins_sensor_linha[8] = {36,13,14,35,32,33,25,28};
	unsigned char pin_sensor_esq = 39;
	unsigned char pin_sensor_dir = 27;

	void Config_pins();
	bool stop_condicion = false;
	
};

#endif