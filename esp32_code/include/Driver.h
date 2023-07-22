#ifndef _DRIVER_
#define _DRIVER_

#include "Motor.h"

class Driver
{

public:

	Driver();
	Driver(unsigned char *pins_dir, unsigned char *pins_esq);

	void Init();

	void Break();

	void Enable_motors_drives();
	void Disable_motors_drives();
	
	void setMotors(int speed_esq, int speed_dir);
	void setVB(int vb);
	void Set_speedRot(int rot);
	void teste();

private:

	int VB = 100;
	
    Motor motor_esq;
	Motor motor_dir;


};

#endif