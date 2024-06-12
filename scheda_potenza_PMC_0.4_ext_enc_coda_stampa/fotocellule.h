
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//  FOTOCELLULE.H   //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef FOTOCELLULE_H
#define FOTOCELLULE_H

#include <stdint.h>
#include <Arduino.h>
#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#define FC_0_PIN  DIN_READ_CH_PIN_00
#define FC_1_PIN  DIN_READ_CH_PIN_01
#define FC_2_PIN  DIN_READ_CH_PIN_02
#define FC_0_INT_PIN PinNameToIndex(PJ_10)


extern uint8_t FC_PINS[3];

//extern bool FC_state[3], FC_state_old[3];

extern uint32_t FC_last_dist_fc1_rise;
extern uint32_t FC_last_dist_fc1_fall;
extern uint32_t FC_last_dist_fc2_rise;
extern uint32_t FC_last_dist_fc2_fall;

extern bool FC_2_override;


bool FC_read(uint8_t n);
bool FC_read_all();
void FC_check();


#endif
