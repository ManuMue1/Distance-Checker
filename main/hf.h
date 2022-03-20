#ifndef HF_H
#define HF_H

#include <RFM69.h>
#include <SPI.h>
#include "util.h"

#define NETWORKID     0   // Must be the same for all nodes
#define NODE_ID       OWN_ID   // The ID of this node
#define FREQUENCY     RF69_433MHZ

#define PAIR_CODE     0xF0
#define UNPAIR_CODE   0xE0

void initialize_hf();

void send_hf_broadcast(byte code);

void send_hf_to_id(byte recv_node_id, byte code);

bool send_with_ack(byte recv_node_id, byte code);

bool receive_hf(byte result_array[]);

bool acknowledge_pairing(byte result_array[]);

bool acknowledge_unpairing(byte result_array[]);

#endif
