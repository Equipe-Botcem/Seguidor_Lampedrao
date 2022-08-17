#include "Arduino.h"

class Motor_drive
{

public:

	//arrumar pinos iniciais;
	Motor_drive();
	Motor_drive(unsigned char PWM_1, unsigned char PWM_2, unsigned char EN, unsigned char DIAG, unsigned char OCM);

	void init();
	void enable_drive();
	void disable_drive();

	unsigned char get_fault();

	void set_motor_forward();
	void set_motor_reverse();
	void set_speed(int speed);

	double get_current_milliamps();

private:

	unsigned char pin_PWM_1;
	unsigned char pin_PWM_2;
	unsigned char pin_EN;
	unsigned char pin_DIAG;
	unsigned char pin_OCM;
	
	// 1 para forward ; 0 para reverse;
	bool motor_direction;
};