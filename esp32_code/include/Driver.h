#ifndef _DRIVER_
#define _DRIVER_

#include "motor_drive.h"

class Driver
{

public:

	Driver();
	Driver(unsigned char *pins_dir, unsigned char *pins_esq);

	void Init();

    void setVB_reta(int vb_reta);
    void setVB_curva(int vb_curva);

	void Enable_motors_drives();
	void Disable_motors_drives();
 
	void Set_speedRot(int speed);
    void Set_speedTrans(int rot);

private:

	int VB_reta, VB_curva;
    Motor_drive motor_esq;
	Motor_drive motor_dir;


};

#endif