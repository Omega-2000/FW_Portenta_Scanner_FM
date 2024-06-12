
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  MOTORE.H  //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef MOTORE_H
#define MOTORE_H


#include <stdint.h>


#define MOT_ANALOG_PIN    0
#define MOT_START_PIN     0
#define MOT_DIRECTION_PIN 1

#define MOT_MAX_FW_SPEED 5.0
#define MOT_BW_SPEED 2.0

extern uint8_t MOT_state;
extern uint8_t MOT_fw_speed;
extern uint8_t MOT_bw_speed;

void MOT_begin();

void MOT_start(double sp = MOT_fw_speed, bool dir = 0);
void MOT_stop();


#endif
