#include "include.h"

class Motor_drive
{

public:

	//arrumar pinos iniciais;
	Motor_drive();
	Motor_drive(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char DIAG, unsigned char OCM);

	void Init();
	void Enable_drive();
	void Disable_drive();

	unsigned char Get_fault();

	void Set_motor_forward();
	void Set_motor_reverse();
	void Set_speed(int speed);
	

	double Get_current_milliamps();

private:

	unsigned char pin_PWM_1;
	unsigned char pin_PWM_2;
	unsigned char pin_EN;
	unsigned char pin_DIAG;
	unsigned char pin_OCM;
	
	// 1 para forward ; 0 para reverse;
	bool motor_direction;
	int motor_speed=0;

	void On();

};