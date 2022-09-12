#ifndef _SENSOR_TEST_
#define _SENSOR_TEST_

#include "Sensors/Sensor/Sensor.h"
#include "Sensors/Sensor_linha/Sensor_linha.h"

unsigned char pin = 0;
unsigned char pins[2] = {0,1};
double calib_val_max = 800;
double calib_val_min = 30;

void Sensor_teste_init();

double Sensor_teste_read();

void Sensor_linha_test_init();

void Sensor_linha_test_read();

#endif