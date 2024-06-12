
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//  TELECAMERA_MANIGLIE.H \\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef TELECAMERA_MANIGLIE_H
#define TELECAMERA_MANIGLIE_H


#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include <stdint.h>


#define TELM_TRIGGER_PIN    4

#define TELM_MS_PERIOD    200
#define TELM_PULSE_COUNT    1


void TELM_start_pulse_train();
void TELM_check();


#endif
