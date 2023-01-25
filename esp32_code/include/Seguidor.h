#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include "motor_drive.h"
#include "Sensor.h"
#include "Encoder.h"
#include "BluetoothSerial.h"


class Seguidor{

public:

	Seguidor();
	Seguidor(double kp, double ki, double kd);

	void Init();

	void Behavior();

	void Set_parametros(double k, double kp, double kd, double vb, int vmin);
	void Stop();
	void Run();

	void Config_motor_esq(unsigned char *pins);
	void Config_motor_dir(unsigned char *pins);
	void Config_encoder_dir(unsigned char pin_interrupt);
	void Config_encoder_esq(unsigned char pin_interrupt);
	void Config_sensor_linha(unsigned char *pins);
	void Config_sensor_esq(unsigned char pin);
	void Config_sensor_dir(unsigned char pin);

	// Sets parametros
	void Set_K(double k);
	void Set_Kp(double kp);
	void Set_kd(double kd);
	void Set_VB(int vb);
	void Set_VM(int vmin);

	void Enable_motors_drives();
	void Disable_motors_drives();

	void Detec_motors_drives_fault();

	void Set_direction_forward();
	void Set_direction_reverse();

	int check_speed_esq(int speed);

	int check_speed_dir(int speed);

	void Set_motor_esq_speed(int speed);
	void Set_motor_dir_speed(int speed);

	void Check_current_motors_drives();

	double calc_erro();

	void calibration();

	void controle();

	int calc_rotacional(double erro);
	int calc_translacional(double erro);


	void testeSensores();

	void initBluetooth();

	void comunica_serial();

	void set_handler();

	void Check_stop();

	void testeMotores();

	//-----------Atributos-----------//
	
	String command = "";

	Sensor sensor_linha[8];
	Sensor sensor_esq;
	Sensor sensor_dir;

	BluetoothSerial SerialBT;


	bool stop_condition = false;
	bool start_condition = false;

	int stop_count = 0;

	unsigned long time_stop;

	double K;

	
private:

	enum Comando {SET = 0, STOP, RUN, CALIBRACAO};

	//-----------Atributos-----------//

	
	double Kp;
	double Kd;
	int VB;
	int VM;

	double erro =0;



	unsigned long control_time = 5;
	unsigned long last_control =0;

	double pesos[8] = {-4.2, -4, -2, -1, 1, 2, 4, 4.2};

	Motor_drive motor_esq;
	Motor_drive motor_dir;

	Encoder encoder_esq;
	Encoder encoder_dir;

	//-----------Pinos-----------//
	unsigned char pins_motor_drive_esq[5] = {15,2,18,21};
	unsigned char pins_motor_drive_dir[5] = {5,22,4,19};
	unsigned char pin_encoder_esq = 16;
	unsigned char pin_encoder_dir = 17;
	unsigned char pins_sensor_linha[8] = {36,13,14,35,32,33,25,26};
	unsigned char pin_sensor_esq = 39;
	unsigned char pin_sensor_dir = 27;

	void Config_pins();
	
	
};

#endif