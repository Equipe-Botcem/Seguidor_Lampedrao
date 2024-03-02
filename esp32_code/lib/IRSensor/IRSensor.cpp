#include "IRSensor.h"
#include <Arduino.h>
#include <IRremote.h>

// command = ir_receiver.read
IRrecv receiver(RECEIVER_PIN);
decode_results results;

IRSensor::IRSensor() {}

IRSensor::~IRSensor() {}

void IRSensor::setup() {
    receiver.enableIRIn();
}

int IRSensor::readIRSignal() {

    // if (receiver.decode()) {
    //     unsigned long receivedHexValue = receiver.decodedIRData.decodedRawData;
    //     receiver.resume();

    //     if(receivedHexValue == 0xBA45FF00 || receivedHexValue == 0x880) {
    //         return 1;
    //     }
    //     if(receivedHexValue == 0xB946FF00  || receivedHexValue == 0x8b5) {
    //         return 2;
    //     }
    //     if(receivedHexValue == 0xB847FF00 || receivedHexValue == 0x882) {
    //         return 3;
    //     } 
    //     if(receivedHexValue == 0xA55AFF00) {
    //         return 4;
    //     }
    //     if(receivedHexValue == 0xF708FF00) {
    //         return 5;
    //     }
    // }
    if (receiver.decode()) {
    unsigned long receivedHexValue = receiver.decodedIRData.decodedRawData;
    receiver.resume();
    return receiver.decodedIRData.decodedRawData;
    }return 0;
}

void IRSensor::printIRSignal() {
    if (receiver.decode()) {
        unsigned long receivedHexValue = receiver.decodedIRData.decodedRawData;
        receiver.resume();
        Serial.println(receiver.decodedIRData.decodedRawData);
    }
}
