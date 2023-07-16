#include "Driver.h"

Driver::Driver(){}


Driver::Driver(unsigned char *pins_dir, unsigned char *pins_esq){
    motor_esq = Motor_drive(pins_esq[0], pins_esq[1], pins_esq[2], pins_esq[3]);
	motor_dir = Motor_drive(pins_dir[0], pins_dir[1], pins_dir[2], pins_dir[3]);
}

void Driver::setVB_reta(int vb_reta){
    VB_reta = vb_reta;
}

void Driver::setVB_curva(int vb_curva){
    VB_curva = vb_curva;
}


void Driver::Init(){
    motor_dir.Init();
    motor_esq.Init();
}

void Driver::Enable_motors_drives()
{
	motor_esq.Enable_drive();
	motor_dir.Enable_drive();
}

void Driver::Disable_motors_drives()
{
	motor_esq.Disable_drive();
	motor_dir.Disable_drive();
}

void Driver::Set_speedTrans(int speed)
{
	motor_dir.Set_speed(speed);
    motor_esq.Set_speed(speed);
}

void Driver::Set_speedRot(int rot)
{
	motor_dir.Set_speed(VB_reta + rot);
    motor_esq.Set_speed(VB_reta - rot);
}

