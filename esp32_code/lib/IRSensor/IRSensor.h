#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <Arduino.h>

#define RECEIVER_PIN 12

class IRSensor {
private:

public:
    IRSensor();     // Constructor
    ~IRSensor();    // Destructor
    void setup();
    int readIRSignal();
    void printIRSignal();
};



#endif