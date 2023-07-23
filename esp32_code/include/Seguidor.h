#ifndef _SEGUIDOR_
#define _SEGUIDOR_

#include "Driver.h"
#include "Sensor.h"
#include "BluetoothSerial.h"
#include "Controlador.h"
#include "Sensor_linha.h"


class Seguidor{

public:

	// Constrtutores
	Seguidor();

	// Configs e inits
	void Config_led_esq(unsigned char pin);
	void Config_led_dir(unsigned char pin);
	void Config_motors(unsigned char *pins_dir, unsigned char *pins_esq);
	void Config_sensor_linha(unsigned char *pins);
	void Config_sensor_esq(unsigned char pin);
	void Config_sensor_dir(unsigned char pin);
	void Config_pins();
	void initBluetooth();
	void Init();

	// Sets parametros
	void set_handler();

	// Other functions 
	void Behavior();
	void Stop();
	void Run();
	void calibration();
	void controle();
	void comunica_serial();
	bool CheckLateralDir();
	bool CheckLateralEsq();
	bool isStart();
	void stopRoutine();
	void bateryCheck();
	void CheckLed();
	void LigaLed();
	void mapeamento();
	bool IsOut();


	// Funções de teste
	void teste();
	void TesteSensoresLat();


	String command = "";
	BluetoothSerial SerialBT;

private:

	enum Comando {SET = 0, STOP, RUN, CALIBRACAO};

//-----------Condicões iniciais-----------//
	bool end = false;
	bool start = false;
	bool isReta = true;
	bool isCalibrado = false;
	bool is_led_on = false;
	bool checking_encruzilhada_dir = false;
	bool gate_sensor = false;

	//-----------Variáveis-----------//
	int Vbc = 60;
	int Vbr = 100;
	int k = 1;
	int out = 40;
	float erro = 0;
	float trans = 0;
	int rot = 0;
	//-----------Timers-----------//
	unsigned long samplingTime = 2;
	unsigned long startTime = 0;
	unsigned long stopTime = 0;
	unsigned long execTime = 0;
	unsigned long latEsqTime = 0;
	unsigned long ledTimer = 0;
	unsigned long encruzilhada_timer = 0;


	//-----------Objetos-----------//
	Driver driver;
	Sensor_linha sensor_linha;
	Sensor sensor_esq;
	Sensor sensor_dir;
	Controlador controlador;
	

	//-----------Pinos-----------//
	// Motor 2
	unsigned char pins_motor_drive_esq[4] = {15,2,18,21};
	unsigned char led_dir_pin = 12;

	// Motor 1
	unsigned char pins_motor_drive_dir[4] = {5,22,4,19};
	unsigned char led_esq_pin = 23;

	// Sensores 
	unsigned char pins_sensor_linha[8] = {39,35,36,14,25,26,27,32};
	unsigned char pin_sensor_esq = 13;
	unsigned char pin_sensor_dir = 33;

	// Outros
	unsigned char bateria = 34;


};

#endif