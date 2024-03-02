#include "Driver.h"
#include "Arduino.h"

Driver::Driver(){}


Driver::Driver(unsigned char *pins_dir, unsigned char *pins_esq){
    motor_esq = Motor(pins_esq[0], pins_esq[1], pins_esq[2], pins_esq[3]);
	motor_dir = Motor(pins_dir[0], pins_dir[1], pins_dir[2], pins_dir[3]);
}

void Driver::Init(){
    motor_dir.Init();
    motor_esq.Init();
}

void Driver::setVB(int vb){
    VB = vb;
}

void Driver::setMotors(int speed_esq, int speed_dir){
    motor_esq.Set_speed(speed_esq);
    motor_dir.Set_speed(speed_dir);
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

void Driver::Break(){
    motor_dir.Set_speed(0);
    motor_esq.Set_speed(0);
}

void Driver::Set_speedRot(int rot)
{
	motor_dir.Set_speed(VB + rot);
    motor_esq.Set_speed(VB - rot);
}

void Driver::Set_highspeedRot(int rot, int VBase)
{
	motor_dir.Set_speed(VBase + rot);
    motor_esq.Set_speed(VBase - rot);
}
void Driver::teste(){
    // Move o motor para frente
    setMotors(100, 100);
    delay(5000);
    // Move o motor para trás
    setMotors(-100,-100);
    delay(5000);

    // Para o motor
    Break();
    delay(5000);
}