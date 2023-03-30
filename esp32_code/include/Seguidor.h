#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include "motor_drive.h"
#include "Sensor.h"
#include "Encoder.h"
#include "BluetoothSerial.h"


class Seguidor{

public:

	// Constrtutores
	Seguidor();
	Seguidor(double kp, double ki, double kd);

	// Configs e inits
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
	void Set_K(double k);
	void Set_Kp(double kp);
	void Set_kd(double kd);
	void Set_VB(int vb);
	void Set_VM(int vmin);
	void set_handler();
	void Set_parametros(double k, double kp, double kd, double vb, int vmin);

	// Other functions 
	void Behavior();
	void Stop();
	void Run();
	void Enable_motors_drives();
	void Disable_motors_drives();
	double calc_erro();
	void calibration();
	void controle();
	int calc_rotacional(double erro);
	int calc_translacional(double erro);
	void comunica_serial();
	bool Check_stop();

	void testeSensores();
	void testeMotores();


	String command = "";
	BluetoothSerial SerialBT;
	bool start_condition = false;
private:

	enum Comando {SET = 0, STOP, RUN, CALIBRACAO};

	//-----------Atributos-----------//

	double pesos[8] = {-8, -4, -2, -1, 1, 2, 4, 8};
	
	double Kp;
	double Kd;
	int VB;
	int VM;
	double erro;
	double K;	


	//-----------Objetos-----------//
	Motor_drive motor_esq;
	Motor_drive motor_dir;
	Sensor sensor_linha[8];
	Sensor sensor_esq;
	Sensor sensor_dir;
	Encoder encoder_esq;
	Encoder encoder_dir;

	//-----------Pinos-----------//
	unsigned char pins_motor_drive_esq[5] = {15,2,18,21};
	unsigned char pins_motor_drive_dir[5] = {5,22,4,19};
	unsigned char pin_encoder_esq = 16;
	unsigned char pin_encoder_dir = 17;
	unsigned char pins_sensor_linha[8] = {39,35,36,14,25,26,27,32};
	unsigned char pin_sensor_esq = 13;
	unsigned char pin_sensor_dir = 33;

	

};

#endif