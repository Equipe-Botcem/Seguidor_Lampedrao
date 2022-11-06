#include "Seguidor/Seguidor.h"

Seguidor::Seguidor()
{
}

Seguidor::Seguidor(double kp, double ki, double kd)
{
	u_c = Controlador(kp, ki, kd);
}

void Seguidor::Config_pins()
{

	u_c.Config_motor_esq(pins_motor_drive_esq);
	u_c.Config_motor_dir(pins_motor_drive_dir);
	u_c.Config_encoder_esq(pin_encoder_esq);
	u_c.Config_encoder_dir(pin_encoder_dir);
	u_c.Config_sensor_linha(pins_sensor_linha);
	u_c.Config_sensor_esq(pin_sensor_esq);
	u_c.Config_sensor_dir(pin_sensor_dir);
}

void Seguidor::Init()
{

	Config_pins();
	u_c.Init();
}

void Seguidor::Set_parametros(double kp, double ki, double kd)
{
	u_c.Set_kp(kp);
	u_c.Set_ki(ki);
	u_c.Set_kd(kd);
}

void Seguidor::Auto_calibrate()
{
	u_c.calibration();
}

void Seguidor::Run()
{
	while (!stop_condition)
	{
		u_c.controle();
	}
}

void Seguidor::Behavior(int bh)
{
	switch (bh)
	{
	case RUN:
		Run();
		break;
	case CALIBRACAO:
		Auto_calibrate();
		break;

	default:
		break;
	}
}