#ifndef US_H
#define US_H

#include <Arduino.h>

#define ECHO_PIN 7
#define TRIG_PIN_RX 8
#define TRIG_PIN_TX 9

extern volatile long duration;

int receive_us();

void send_us();

void initialize_us();


#endif
