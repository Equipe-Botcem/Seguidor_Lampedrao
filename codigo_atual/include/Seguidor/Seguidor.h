#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include <Controlador/Controlador.h>

class Seguidor{

public:

	Seguidor();
	Seguidor(double kp, double ki, double kd);

	void Init();

	void Set_parametros(double kp, double ki, double kd);

	void Auto_calibrate();
	void Run();

private:

	Controlador u_c;

	unsigned char pins_motor_drive_esq[5] = {0,1,2,3,4};
	unsigned char pins_motor_drive_dir[5] = {0,1,2,3,4};
	unsigned char pin_encoder_esq = 0;
	unsigned char pin_encoder_dir = 0;
	unsigned char pins_sensor_linha[8] = {0,1,2,3,4,5,6,7};
	unsigned char pin_sensor_esq = 0;
	unsigned char pin_sensor_dir = 0;

	void Config_pins();


};

#endif