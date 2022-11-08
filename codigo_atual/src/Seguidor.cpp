#include "Seguidor.h"

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
	Set_parametros(0.1,0.005,0);
}


void Seguidor::Set_parametros(double k, double kp, double kd)
{
	u_c.Set_K(k);
	u_c.Set_Kp(kp);
	u_c.Set_kd(kd);
}

void Seguidor::Auto_calibrate()
{
	u_c.calibration();
}

void Seguidor::Run()
{
	u_c.Enable_motors_drives();
	//u_c.controle();
	//u_c.Disable_motors_drives();
	start_condition = true;
}

void Seguidor::Stop(){
	u_c.Disable_motors_drives();
}

void Seguidor::Behavior(int bh)
{
	switch (bh)
	{
	case SET:
		//Set_parametros();
		break;
	case STOP:
		Stop();
		break;
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