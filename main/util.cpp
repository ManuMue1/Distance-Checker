#include "util.h"

bool start_pairing = false;
bool start_unpairing = false;

void setup() {
    pinMode(3, INPUT_PULLUP); //Interrupt PIN for BTN (used for friend adding)
    pinMode(4, INPUT);
#if CLEAR_EEPROM_AT_START
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
#endif
    EEPROM.get(0, friends_array); // Read friend ids into array at beginning
    attachInterrupt(digitalPinToInterrupt(3), button_press_ISR, FALLING);
    initialize_hf();
    initialize_us();
    buzzer.begin(A0);
}

void warn_signal() {
    buzzer.play(2000, 500);
}

void start_pairing_signal() {
    buzzer.play(2500, 1500);
}

void friend_added_signal() {
    buzzer.play(1500, 1500);
}

bool is_id_friend(byte friend_id){
    for (byte i = 0; i < 20; i++){
        if (friends_array[i] == friend_id){
            return true;
        }
    }
    return false;
}

void add_friend(byte friend_id) {
    for (byte i = 0; i < 20; i++) {
        if (friends_array[i] == 0) {
            friends_array[i] = friend_id;
            EEPROM.put(0, friends_array);
            Serial.print("Added friend with ID: ");
            Serial.println(friend_id);
            friend_added_signal();
            return;
        } else if (friends_array[i] == friend_id) {
            Serial.print("Not adding friend, because already saved... ID: ");
            Serial.println(friend_id);
            return;
        }
    }
}void remove_friend(byte friend_id) {
    for (byte i = 0; i < 20; i++) {
        if (friends_array[i] == friend_id) {
            friends_array[i] = 0;
            EEPROM.put(0, friends_array);
            Serial.print("Removed friend with ID: ");
            Serial.println(friend_id);
            friend_added_signal();
            return;
        }
    }
}

void initiate_pairing() {
    unsigned long start_time = millis();
    start_pairing_signal();
    start_pairing = false;
    for (byte i = 0; i < 5; i++) {
        send_hf_broadcast(PAIR_CODE);
#if DEBUG
        Serial.print("Send pairing broadcast attempt: ");
        Serial.println(i+1);
#endif
        while (!acknowledge_pairing(hf_received_values) && (millis() - start_time) < 1000) {
            //Wait for something to receive or timeout
        }
        if (hf_received_values[1] == PAIR_CODE) { // If something was received
            add_friend(hf_received_values[0]);
            return;
        }
        start_time = millis();
    }

}

void initiate_unpairing() {
    unsigned long start_time = millis();
    start_pairing_signal();
    start_unpairing = false;
    for (byte i = 0; i < 5; i++) {
        send_hf_broadcast(UNPAIR_CODE);
#if DEBUG
        Serial.print("Send unpairing broadcast attempt: ");
        Serial.println(i+1);
#endif
        while (!acknowledge_unpairing(hf_received_values) && (millis() - start_time) < 1000) {
            //Wait for something to receive or timeout
        }
        if (hf_received_values[1] == UNPAIR_CODE) { // If something was received
            remove_friend(hf_received_values[0]);
            return;
        }
        start_time = millis();
    }

}

void button_press_ISR() {
    unsigned long timestamp = millis();

    if (timestamp - btn_lass_press < 100 || digitalRead(3)) return; // Debounce



    if (timestamp - btn_lass_press > 1000) {
        btn_press_count = 0;
    } else if (btn_press_count == 3 && !digitalRead(4)) {
            start_pairing = true;
            btn_press_count = -1;
    } else if (btn_press_count == 3 && digitalRead(4)){
        start_unpairing = true;
        btn_press_count = -1;
    }

    btn_press_count++;
    btn_lass_press = millis();
}