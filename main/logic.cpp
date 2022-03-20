#include "logic.h"

void master() {
    unsigned long next_time;
    unsigned long time_everyone_in_slavemode;
    int wait_start = 200;
    int wait_warn = 600;
    int flag_received;
    byte received_values_master[2];

    time_everyone_in_slavemode = millis() + wait_start;

    // warte bis alle Slaves im slave Mode
    while (millis() <= time_everyone_in_slavemode) {}

    send_hf_broadcast(DISTANCE_CODE);
    delay(1); // warten bis Abarbeitung des Codes im slave
    send_us();

    //warten, ob slave Warn Signal zurück sendet
    flag_received = 0;
    next_time = millis() + wait_warn;

    while (millis() <= next_time && flag_received != 1) {
        receive_hf(received_values_master);

        // falls WARN_CODE received_values_master wird, setze flag und löse Warn Signal aus
        if (received_values_master[1] == WARN_CODE) {
            flag_received = 1;
            if (!is_id_friend(received_values_master)){
                warn_signal();
            }
        }
    }
}

void slave(byte other_node_id) {
    unsigned long next_time;
    int wait_master = WAIT_MASTER;
    int received_distance_flag;
    byte received_values_slave[2];
    int distance;

    // Sende GET_MASTER_CODE an master zurück  
    send_hf_to_id(other_node_id, GET_MASTER_CODE);

    // warte auf Start Signal der Zeitmessung
    int flag = 0;
    received_distance_flag = 0;
    next_time = millis() + wait_master;

    while (millis() <= next_time && received_distance_flag != 1) {
        while (!receive_hf(received_values_slave)) {

        }

        // Abfrage ob HF Signal für Entfernungsmessung empfangen wird, falls ja, flag setzen
        if (received_values_slave[0] == other_node_id && received_values_slave[1] == DISTANCE_CODE) {
            // Ultraschall Sensor für empfangen vorbereiten/aktivieren/Messung
            distance = receive_us() * 0.0343; // falls außer Reichweite 0 zurück senden
            distance = distance - 25;
            Serial.print("Ultraschall Distanz: ");
            Serial.print(distance);
            Serial.println(" cm");
            break;
        }
    }
    // falls Abstand zu gering, warn Signal aktivieren und Warncode an master schicken
    if (distance <= WARN_DISTANCE && distance != 0) {
        if (!is_id_friend(received_values_slave[0])){
            warn_signal();
            send_hf_to_id(other_node_id, WARN_CODE);
        }
    }
}
