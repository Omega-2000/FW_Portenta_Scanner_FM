
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  LASER.H  //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef LASER_H
#define LASER_H


#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include <stdint.h>


#define LAS_1_PIN           6
#define LAS_2_PIN           5

#define LAS_MODE_OFF        0
#define LAS_MODE_ON         1
#define LAS_MODE_AUTO       2
#define LAS_MODE_AUTO_OFF   3


extern uint8_t LAS_mode;


void LAS_set_mode(uint8_t mode, uint8_t n_las = 0);

void LAS_1_on(bool man = 0);
void LAS_2_on(bool man = 0);
void LAS_1_off(bool man = 0);
void LAS_2_off(bool man = 0);



#endif
