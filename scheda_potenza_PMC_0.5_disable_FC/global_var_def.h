
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\ VARIABILI GLOBALI  //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef GLOBAL_VAR_DEF_H
#define GLOBAL_VAR_DEF_H

#include <stdint.h>
#include "motore.h"

#define versione 1

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#define DBSerial Serial
#else
#define DBSerial \/\/
#endif


extern bool MAC_running;
extern bool MAC_FC_enabled;

void MAC_FC_disable_check();
void MAC_start(double sp);
void MAC_stop();
void MAC_reset();


extern uint32_t lunghezza_step;
extern bool lunghezza_step_ricevuta;
extern uint32_t last_lunghezza_richiesta;
extern uint8_t last_id_richiesto;

#endif
