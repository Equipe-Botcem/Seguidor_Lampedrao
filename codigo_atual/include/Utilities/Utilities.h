#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

String command = "";

void initBluetooth(){
    SerialBT.begin("ESP32"); 
  	Serial.println("O dispositivo já pode ser pareado ou conectado!");
}
