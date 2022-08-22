#include "Sensors_classes/Sensor_class/Sensor.h"

//!estudar como fazer
class Sensor_linha{

public:

	Sensor_linha();
	Sensor_linha(unsigned char *pins);
	void Init();
	double* Read_line(); 


private:
	static const unsigned int num_sensores_linha = 8 ;
	unsigned char pins_sensores[num_sensores_linha];
	Sensor sensor_linha[num_sensores_linha];

};