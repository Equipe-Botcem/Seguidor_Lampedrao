#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include <Controlador.h>

class Seguidor{

public:

	Seguidor();
	Seguidor(double kp, double ki, double kd);

	void Init();

	void Behavior(int bh);

	void Set_parametros(double k, double kp, double kd);
	void Stop();
	void Auto_calibrate();
	void Run();

	bool stop_condition = false;
	bool start_condition = false;

	Controlador u_c;
private:

	enum Comando {SET = 0, STOP, RUN, CALIBRACAO};


	unsigned char pins_motor_drive_esq[5] = {15,2,18,21};
	unsigned char pins_motor_drive_dir[5] = {5,22,4,21};
	unsigned char pin_encoder_esq = 16;
	unsigned char pin_encoder_dir = 17;
	unsigned char pins_sensor_linha[8] = {36,13,14,35,32,33,25,28};
	unsigned char pin_sensor_esq = 39;
	unsigned char pin_sensor_dir = 27;

	void Config_pins();
	
	
};

#endif