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
	int behavior = msg_handler();
	command = "";
	return behavior;
}