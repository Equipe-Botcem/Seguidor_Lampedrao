#include "Driver.h"

Driver::Driver(){}


Driver::Driver(unsigned char *pins_dir, unsigned char *pins_esq){
    motor_esq = Motor(pins_esq[0], pins_esq[1], pins_esq[2], pins_esq[3]);
	motor_dir = Motor(pins_dir[0], pins_dir[1], pins_dir[2], pins_dir[3]);
}

void Driver::Init(){
    motor_dir.Init();
    motor_esq.Init();

    VB = 100;
}

void Driver::setVB(int vb){
    VB = vb;
}

void Driver::setMotors(int speed_esq, int speed_dir){
    motor_dir.Set_speed(speed_dir);
    motor_esq.Set_speed(speed_esq);
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

void Driver::Set_speedRot(int rot)
{
	motor_dir.Set_speed(VB + rot);
    motor_esq.Set_speed(VB - rot);
}

