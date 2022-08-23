#include "Controlador_test.h"

void Controlador_teste_config()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
}

void Controlador_teste_init()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
	controlador_teste.Init();
}

void Controlador_teste_enable_motors_drives()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
	controlador_teste.Init();
	controlador_teste.Enable_motors_drives();
}

void Controlador_teste_disable_motors_drives()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
	controlador_teste.Init();
	controlador_teste.Disable_motors_drives();
}

void Controlador_teste_set_direction_forward()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
	controlador_teste.Init();
	controlador_teste.Enable_motors_drives();
	controlador_teste.Set_direction_forward();
}

void Controlador_teste_set_direction_reverse()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
	controlador_teste.Init();
	controlador_teste.Enable_motors_drives();
	controlador_teste.Set_direction_reverse();
}

void Controlador_teste_command_1()
{
	Controlador controlador_teste(100, 100, 100);
	controlador_teste.Config_motor_esq(pins_motores);
	controlador_teste.Init();
	controlador_teste.Enable_motors_drives();
	controlador_teste.Set_direction_forward();
	controlador_teste.Set_motor_esq_speed(100);
	delay(5000);
	controlador_teste.Disable_motors_drives();
}