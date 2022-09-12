#ifndef _CONTROLADOR_
#define _CONTROLADOR_

#include "Motor_drive/motor_drive.h"
#include "Sensors/Sensor_linha/Sensor_linha.h"
#include "Encoder/Encoder.h"

class Controlador
{

public:
	Controlador();
	Controlador(double kp, double ki, double kd);

	void Config_motor_esq(unsigned char *pins);
	void Config_motor_dir(unsigned char *pins);
	void Config_encoder_dir(unsigned char pin_interrupt);
	void Config_encoder_esq(unsigned char pin_interrupt);
	void Config_sensor_linha(unsigned char *pins);
	void Config_sensor_esq(unsigned char pin);
	void Config_sensor_dir(unsigned char pin);

	void Init();

	void Set_kp(double kp);
	void Set_ki(double ki);
	void Set_kd(double kd);

	void Enable_motors_drives();
	void Disable_motors_drives();

	void Detec_motors_drives_fault();

	void Set_direction_forward();
	void Set_direction_reverse();

	void Set_motor_esq_speed(int speed);
	void Set_motor_dir_speed(int speed);

	void Check_current_motors_drives();

	double calc_erro();

private:

	double _kp;
	double _ki;
	double _kd;

	double pesos[8] = {-8, -4, -2, -1, 1, 2, 4, 8};

	Motor_drive motor_esq;
	Motor_drive motor_dir;

	Encoder encoder_esq;
	Encoder encoder_dir;

	// testar para ver criação correta do objeto
	Sensor_linha sensor_linha;
	Sensor_esq sensor_esq;
	Sensor_dir sensor_dir;
};

#endif