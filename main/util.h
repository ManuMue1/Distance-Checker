#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include <Tone.h>
#include <EEPROM.h>
#include "us.h"
#include "hf.h"

#define DEBUG true

#define CLEAR_EEPROM_AT_START false

#define OWN_ID 0b0001

#define START_CODE 0b0001
#define GET_MASTER_CODE 0b0010
#define DISTANCE_CODE 0b0011
#define WARN_CODE 0b0100

#define WAIT_MASTER 200
#define WAIT_SLAVE 5000
#define MIN_DELAY 1
#define MAX_DELAY 50
#define WARN_DISTANCE 150 // abstandszeit zwischen Ultraschall signal gesendet und empfangen


static Tone buzzer;
static int btn_press_count = 0;
static unsigned long btn_lass_press = 0;
static byte friends_array[20]{0};
static byte hf_received_values[2]{0};

extern bool start_pairing;
extern bool start_unpairing;

void initiate_pairing();

void initiate_unpairing();

void setup();

void warn_signal();

void start_pairing_signal();

void friend_added_signal();

void button_press_ISR();

bool is_id_friend(byte friend_id);

void add_friend(byte friend_id);

void remove_friend(byte friend_id);

#endif
