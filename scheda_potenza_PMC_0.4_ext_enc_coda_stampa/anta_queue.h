
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//  ANTA QUEUE.H  \\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              15/07/22


#ifndef ANTA_QUEUE_H
#define ANTA_QUEUE_H

#include <stdint.h>

#include "Anta.h"


#define ANT_queue_size 20


extern Anta ANT_queue[ANT_queue_size];
extern uint8_t ANT_first;
extern uint8_t ANT_last;
extern uint8_t ANT_last_id;

void ANT_enqueue(Anta a);
void ANT_last_update(Anta a);
Anta* ANT_last_view();
Anta* ANT_first_view();
Anta ANT_dequeue();
void ANT_flush();

void ANT_last_add_end(uint32_t end_pulses);

#endif
