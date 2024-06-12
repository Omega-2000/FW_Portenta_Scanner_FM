
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  EMERGENZE.H \\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              18/07/22


#ifndef EMERGENZE_H
#define EMERGENZE_H

#include <stdint.h>


#define EMERG_PIN DIN_READ_CH_PIN_06


extern bool EMERG_arresto;
extern bool EMERG_arresto_old;
extern bool EMERG_arresto_ch;

void EMERG_begin();
bool EMERG_check();

#endif
