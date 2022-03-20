#include "hf.h"

RFM69 radio;


/*
 * Call this function once to initialize the hf modul
*/
void initialize_hf() {
    radio.initialize(FREQUENCY, NODE_ID, NETWORKID);
    Serial.print("HF initialized with NODE_ID: ");
    Serial.println(NODE_ID);
}

/*
 * Sends a broadcast which is received by all nodes with the same NETWORKID
 * @param byte code the code you wanna send
*/
void send_hf_broadcast(byte code) {
    radio.send(0, &code, 1); // 0 is the broadcast address
};

/* Sends a broadcast to the node with the specified id
 *
 * @param byte recv_node_id the id of the node you wanna send to
 * @param byte code the code you wanna send
*/
void send_hf_to_id(byte recv_node_id, byte code) {
    radio.send(recv_node_id, &code, 1);
};

/* Sends a broadcast to the node with the specified id and waits for ACK
 *
 * @param byte recv_node_id the id of the node you wanna send to
 * @param byte code the code you wanna send
 * @return true if ACK recv, else false
*/
bool send_with_ack(byte recv_node_id, byte code) {
    if (radio.sendWithRetry(recv_node_id, &code, 1)) {
        return true;
    } else {
        return false;
    }
}

/*
 * If anything was received this function returns true.
 * The passed array "result_array" then has the following values:
        result_array[0] = the node id which send the message
        result_array[1] = the code which was received
 * If nothing was received the function returns false.
 * @param byte result_array[] for storing the received values
 * @return bool with receive status
*/
bool receive_hf(byte result_array[]) {

    if (radio.receiveDone()) {
        result_array[0] = (byte) radio.SENDERID;
        result_array[1] = (byte) radio.DATA[0];
        if (result_array[0] == OWN_ID) return false;

        if (result_array[1] == PAIR_CODE && !digitalRead(3)) {
            if (send_with_ack(result_array[0], PAIR_CODE)) {
                add_friend(result_array[0]);
            }
        }

        if (result_array[1] == UNPAIR_CODE && !digitalRead(3)) {
            if (send_with_ack(result_array[0], UNPAIR_CODE)) {
                remove_friend(result_array[0]);
            }
        }

        if (radio.ACKRequested()) {
            radio.sendACK();
        }
        return true;
    }
    result_array[0] = 0;
    result_array[1] = 0;
    return false;
}


/*
 * Check if anything was received and if it was a PAIR_CODE send back an ACK
 * If nothing was received the function returns false.
 * @param byte result_array[] for storing the received values (ID that should be saved)
 * @return bool true if PAIR_CODE recv and ACK send, else false
*/
bool acknowledge_pairing(byte result_array[]) {
    // while button pressed
    if (radio.receiveDone()) {
        result_array[0] = (byte) radio.SENDERID;
        result_array[1] = (byte) radio.DATA[0];
        if (result_array[0] == OWN_ID) return false;
        if (result_array[1] == PAIR_CODE) {
            radio.sendACK();
            return true;
        }
    }
    return false;
}

bool acknowledge_unpairing(byte result_array[]) {
    // while button pressed
    if (radio.receiveDone()) {
        result_array[0] = (byte) radio.SENDERID;
        result_array[1] = (byte) radio.DATA[0];
        if (result_array[0] == OWN_ID) return false;
        if (result_array[1] == UNPAIR_CODE) {
            radio.sendACK();
            return true;
        }
    }
    return false;
}
