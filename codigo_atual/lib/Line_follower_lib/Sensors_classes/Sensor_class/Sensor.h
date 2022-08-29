#include "include.h"

class Sensor
{
public:
	Sensor();
	Sensor(unsigned char pin);
	Sensor(unsigned char pin, double val_max, double val_min);

	void Init();

	int Read_sensor();
	
	void Calibracao();
	virtual void Rotina();

private:
	unsigned char pin_sensor;
	double calib_val_max;
	double calib_val_min;
};

class Sensor_esq : public Sensor
{

public:
	Sensor_esq();
	Sensor_esq(unsigned char pin);
	void Rotina();

private:
};

class Sensor_dir : public Sensor
{

public:
	Sensor_dir();
	Sensor_dir(unsigned char pin);
	void Rotina();

private:
};