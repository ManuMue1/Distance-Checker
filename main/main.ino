#include "util.h"
#include "logic.h"


int main() {
    init();
    Serial.begin(9600);
    setup();
    Serial.print(OWN_ID);
    unsigned long time_till_master_detection;
    int wait_master = 100;
    int wait_slave = 500;
    int min_delay = 1;
    int max_delay = 50;

    while (1) {

        //warte zu Beginn random Time damit Kollisionen verhindert werden können
        delay(random(min_delay, max_delay));

        // Sende eigene ID
        send_hf_broadcast(START_CODE);

        // überprüfe, ob in 0,2 Sekundne eigene ID zurück kommt
        time_till_master_detection = millis() + wait_master;

        while (millis() <= time_till_master_detection) {
            receive_hf(hf_received_values);

            if (GET_MASTER_CODE == hf_received_values[1]) {
                master();
            }
        }

        // überprüfe für 5s, ob man eine Fremde ID empfängt
        time_till_master_detection = millis() + wait_slave;


        while (millis() <= time_till_master_detection) {

            receive_hf(hf_received_values);

            // Prüfen, ob ein Code received_values_master wird
            if (hf_received_values[1] == START_CODE) {
                    slave(hf_received_values[0]);
            }
        }
        if (start_pairing) initiate_pairing();
        if (start_unpairing) initiate_unpairing();


    }
}
