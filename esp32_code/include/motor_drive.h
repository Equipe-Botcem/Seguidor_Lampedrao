#ifndef _MOTOR_DRIVE_
#define _MOTOR_DRIVE_

//#define ON

class Motor_drive
{

public:

	//arrumar pinos iniciais;
	Motor_drive();
	Motor_drive(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char OCM);

	void Init();
	void Enable_drive();
	void Disable_drive();
 
	void Set_speed(int speed);

private:

	unsigned char pin_PWM_1;
	unsigned char pin_PWM_2;
	unsigned char pin_EN;
	unsigned char pin_OCM;
	

};

#endif