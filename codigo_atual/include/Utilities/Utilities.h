#ifndef _UTILITIES_
#define _UTILITIES_
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

String command = "";

const char SET = 'S';
const char STOP = 'P';
const char RUN = 'R';
const char CALIBRACAO = 'C';


void initBluetooth(){
    SerialBT.begin("ESP32");
  	Serial.println("O dispositivo já pode ser pareado ou conectado!");
}

int msg_handler()
{
	switch (command[0])
	{
	case SET:
		return 0;
		break;
	case STOP:
		return 1;
		break;
	case RUN:
		return 2;
		break;
	case CALIBRACAO:
		return 3;
		break;
	default:
		return -1;
		break;
	}
}

int comunica_serial(){
	command = SerialBT.readStringUntil(';');
	SerialBT.println(command);
	int behavior = msg_handler();
	command = "";
	return behavior;
}

#endif

/*
void set_handler()
{
	String VB = "", KP_str = "", KI_str = "", KD_str = "", K_str = "", vel_min_str = "";
	int pos = command.indexOf(',', 2);
	for (int i = 4; i < pos; i++)
		KP_str += command[i];

	int pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		KI_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		KD_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		VB += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		K_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		vel_min_str += command[i];

	Kp = KP_str.toDouble() / 1000;
	Kd = KD_str.toDouble() / 1000;
	Ki = KI_str.toDouble() / 1000;
	vel_base = VB.toInt();
	K = K_str.toDouble() / 1000;
	vel_min = vel_min_str.toInt();
	qnt_linhas = 2;
	stop_condition = false;

	// Serial.println(STOP_VALUE_LATERAL);
}
*/