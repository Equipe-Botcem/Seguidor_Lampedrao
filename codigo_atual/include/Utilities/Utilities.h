#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#include "BluetoothSerial.h"

AsyncWebServer server(80);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const char SET = 'S';
const char STOP = 'P';
const char RUN = 'R';
const char CALIBRACAO = 'C';

// Comportamento que msg enviada desempenhará 
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
}

void initWifi(const char * WIFI_SSID, const char * WIFI_PASSWORD){
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.printf("WiFi Failed!\n");
		return;
	}
	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());
	// WebSerial is accessible at "<IP Address>/webserial" in browser
	WebSerial.begin(&server);
	WebSerial.msgCallback(recvMsg);
	server.begin();
}

void initBluetooth(){
    SerialBT.begin("ESP32");
  	Serial.println("O dispositivo já pode ser pareado ou conectado!");
}

int comunica_serial(){
	command = SerialBT.readStringUntil(';');
	int behavior = msg_handler();
	command = "";
	return behavior;
}


int msg_handler()
{
	switch (command[0])
	{
	case SET:
		//set_handler();
		break;
	case STOP:
		//stop_handler();
		break;
	case RUN:
		//run_handler();
		break;
	case CALIBRACAO:
		return 3;
		break;
	}
}
