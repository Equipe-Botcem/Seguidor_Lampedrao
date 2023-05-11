#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include "motor_drive.h"
#include "Sensor.h"
#include "Encoder.h"
#include "BluetoothSerial.h"
#include "Controlador.h"


class Seguidor{

public:

	// Constrtutores
	Seguidor();

	// Configs e inits
	void Config_led_esq(unsigned char pin);
	void Config_led_dir(unsigned char pin);
	void Config_motor_esq(unsigned char *pins);
	void Config_motor_dir(unsigned char *pins);
	void Config_encoder_dir(unsigned char pin_interrupt);
	void Config_encoder_esq(unsigned char pin_interrupt);
	void Config_sensor_linha(unsigned char *pins);
	void Config_sensor_esq(unsigned char pin);
	void Config_sensor_dir(unsigned char pin);
	void Config_pins();
	void initBluetooth();
	void Init();

	// Sets parametros
	void Set_VB(int vb);
	void Set_VM(int vmin);
	void set_handler();
	void Set_parametros(float kp, float kd, float ki, float vb, int vmin);

	// Other functions 
	void Behavior();
	void Stop();
	void Run();
	void Enable_motors_drives();
	void Disable_motors_drives();
	float calc_erro();
	void calibration();
	void controle();
	void comunica_serial();
	bool Check_stop();
	void returnToLine(float);
	bool isEnd();
	bool isStar();
	void stopRoutine();

	void testeSensores();
	void testeMotores();
	void habiliteiStop();


	String command = "";
	BluetoothSerial SerialBT;

private:

	enum Comando {SET = 0, STOP, RUN, CALIBRACAO};

	//-----------Atributos-----------//

	double pesos[8] = {-5, -4, -2, -1, 1, 2, 4, 5};

	int VB;
	int VM;
	bool end = false;
	bool start = false;
	unsigned long startTime = 0;
	unsigned long stopTime = 0;
	unsigned long samplingTime = 0;


	//-----------Objetos-----------//
	Motor_drive motor_esq;
	Motor_drive motor_dir;
	Sensor sensor_linha[8];
	Sensor sensor_esq;
	Sensor sensor_dir;
	Encoder encoder_esq;
	Encoder encoder_dir;
	Controlador controlador;
	

	//-----------Pinos-----------//
	// Motor 2
	unsigned char pins_motor_drive_esq[4] = {2,15,18,21};
	unsigned char pin_encoder_esq = 16;
	unsigned char led_dir = 12;

	// Motor 1
	unsigned char pins_motor_drive_dir[4] = {22,5,4,19};
	unsigned char pin_encoder_dir = 17;
	unsigned char led_esq = 23;

	// Sensores 
	unsigned char pins_sensor_linha[8] = {39,35,36,14,25,26,27,32};
	unsigned char pin_sensor_esq = 13;
	unsigned char pin_sensor_dir = 33;

	// Outros
	unsigned char bateria = 34;
	unsigned char led1 = 23;
	unsigned char led2 = 12;

	

};

#endif