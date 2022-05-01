#ifndef _LINEFOLLOWER_HPP
#define _LINEFOLLOWER_HPP

#include <BLEDevice.h>
#include <BLEServer.h>
#include <esBLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *characteristicTX;

#define SERVICE_UUID "ab0828b1-198e-4351-b779-901fa0e0371e”
#define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91”

bool deviceConnected = false;

const char MOTORES = 'M';
const char PID = 'P';
const char BASE = 'B';
const char RUN = 'R';
const char STOP = 'S';
const char CALIBRAR = 'C';

const int input_sens_lat_1 = 36; //ver qual vai ser direita e esquerda
const int input_sens_lat_2 = 39; //ver qual vai ser direita e esquerda

const int ir_sens_front = 34;
const int d1_sens_front = 35;
const int d2_sens_front = 32;
const int d3_sens_front = 33;
const int d4_sens_front = 25;
const int d5_sens_front = 26;
const int d6_sens_front = 27;
const int d7_sens_front = 14;
const int d8_sens_front = 13;

const int ocm_pont_h_1 = 12; // ver qual motor é
const int diag_pont_h_1 = 21;
const int en_pont_h_1 = 17;
const int enb_pont_h_1 = 16;
const int pw1_pont_h_1 = 15;
const int pw2_pont_h_1 = 2;

const int ocm_pont_h_2 = 19; // ver qual motor é
const int diag_pont_h_2 = 18;
const int en_pont_h_2 = 1;
const int enb_pont_h_2 = 3;
const int pw1_pont_h_2 = 4;
const int pw2_pont_h_2 = 5;

const int div_tensao = 23;

void pin_mode_initiator()
{
	pinMode(input_sens_lat_1, INPUT);
	pinMode(input_sens_lat_2, INPUT);

	pinMode(ir_sens_front, OUTPUT);
	pinMode(d1_sens_front, INPUT);
	pinMode(d2_sens_front, INPUT);
	pinMode(d3_sens_front, INPUT);
	pinMode(d4_sens_front, INPUT);
	pinMode(d5_sens_front, INPUT);
	pinMode(d6_sens_front, INPUT);
	pinMode(d7_sens_front, INPUT);
	pinMode(d8_sens_front, INPUT);

	pinMode(ocm_pont_h_1, INPUT);
	pinMode(diag_pont_h_1, INPUT);
	pinMode(en_pont_h_1, OUTPUT);
	pinMode(enb_pont_h_1, OUTPUT);
	pinMode(pw1_pont_h_1, OUTPUT);
	pinMode(pw2_pont_h_1, OUTPUT);

	pinMode(ocm_pont_h_2, INPUT);
	pinMode(diag_pont_h_2, OUTPUT);
	pinMode(en_pont_h_2, OUTPUT);
	pinMode(enb_pont_h_2, OUTPUT);
	pinMode(pw1_pont_h_2, OUTPUT);
	pinMode(pw2_pont_h_2, OUTPUT);

	pinMode(div_tensao, OUTPUT);
}

void BLE_initiator()
{
	// Create the BLE Device
	BLEDevice::init("Russi_Rizzi_Seguidor"); // nome do dispositivo bluetooth

	// Create the BLE Server
	BLEServer *server = BLEDevice::createServer(); // cria um BLE server

	server->setCallbacks(new ServerCallbacks()); // seta o callback do server

	// Create the BLE Service
	BLEService *service = server->createService(SERVICE_UUID);

	// Create a BLE Characteristic para envio de dados
	characteristicTX = service->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

	characteristicTX->addDescriptor(new BLE2902());

	// Create a BLE Characteristic para recebimento de dados
	BLECharacteristic *characteristic = service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

	characteristic->setCallbacks(new CharacteristicCallbacks());

	// Start the service
	service->start();

	// Start advertising (descoberta do ESP32)
	server->getAdvertising()->start();
}

// callback para receber os eventos de conexão de dispositivos
class ServerCallbacks : public BLEServerCallbacks
{
	void onConnect(BLEServer *pServer)
	{
		deviceConnected = true;
	};

	void onDisconnect(BLEServer *pServer)
	{
		deviceConnected = false;
	};
};

// callback para eventos das características
class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
	void onWrite(BLECharacteristic *characteristic)
	{
		std::string rxValue = characteristic->getValue();
		if (rxValue.length() > 0)
		{
			define_estado(rxValue);
		}
	} // onWrite
};

void define_estado(std::string rxValue)
{

	switch (rxValue[0])
	{
	case MOTORES:
		motors_handle(rxValue);
		break;

	case PID:
		pid_handle(rxValue);
		break;

	case BASE:
		base_handle(rxValue);
		break;

	case RUN:
		run_handle(rxValue);
		break;

	case STOP:
		stop_handle(rxValue);
		break;

	case CALIBRAR:
		calibrar_handle(rxValue);
		break;

	default:
		break;
	}
}

void motors_handle(rxValue){

}

#endif