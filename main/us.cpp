#include "us.h"

void initialize_us() {
//Pin Modes in der main-Datei festlegen
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIG_PIN_RX, OUTPUT);
    pinMode(TRIG_PIN_TX, OUTPUT);
}

//int distance= duration*0.034;

int receive_us() {
    int duration = 0;
    digitalWrite(TRIG_PIN_RX, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN_RX, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    return duration;
}

void send_us() {
    digitalWrite(TRIG_PIN_TX, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN_TX, LOW);
}
