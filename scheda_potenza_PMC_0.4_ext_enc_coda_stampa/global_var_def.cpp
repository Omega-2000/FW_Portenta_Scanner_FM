
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\ VARIABILI GLOBALI  //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>
#include "Wire.h"

#include "global_var_def.h"
#include "motore.h"
#include "anta_queue.h"


bool MAC_running;

void MAC_start(double sp) {
  MAC_running = 1;
  MOT_start(sp, 0);
}

void MAC_stop() {
  MAC_running = 0;
  MOT_stop();
}


void MAC_reset() {
  // TODO resettare dati antine NO id
  ANT_flush();
}


uint32_t lunghezza_step = 0;
bool lunghezza_step_ricevuta = 0;
uint32_t last_lunghezza_richiesta;
uint8_t last_id_richiesto;
