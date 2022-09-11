#ifndef _SENSOR_TEST_
#define _SENSOR_TEST_

#include "Sensors/Sensor/Sensor.h"

unsigned char pin = 0;
double calib_val_max = 800;
double calib_val_min = 30;

void Sensor_teste_init();

double Sensor_teste_read();

#endif