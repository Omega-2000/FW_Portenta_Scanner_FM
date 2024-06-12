
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\ INGRESSI_USCITE_PRG.H  //\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              22/07/22


#ifndef INGRESSI_USCITE_PRG_H
#define INGRESSI_USCITE_PRG_H

#include <stdint.h>

#define INGRESSI_MASK   0b11111111111111111111111111111111
//                        [--- in ---][------ out -------]
//                        000000000011110    0
//                        012345678901109    7
//                        [     ][   ][                  ]
#define INGRESSI_2_MASK 0b11111110000000011110000000000000

#define USCITE_MASK 0b1111111111111111

extern uint32_t INGRESSI_mask;
extern uint32_t INGRESSI_state;
extern uint32_t INGRESSI_state_old;
extern uint32_t INGRESSI_rise;
extern uint32_t INGRESSI_fall;

extern uint32_t USCITE_mask;
extern uint32_t USCITE_state;
extern uint32_t USCITE_state_old;
extern uint32_t USCITE_rise;
extern uint32_t USCITE_fall;

void INGRESSI_USCITE_begin();

bool INGRESSI_readAll();
bool INGRESSI_read(uint8_t pin);
bool INGRESSI_rising(uint8_t pin);
bool INGRESSI_falling(uint8_t pin);

bool USCITE_writeAll();
bool USCITE_write(uint8_t pin, bool st);
void USCITE_setDir(uint8_t pin, bool st);
/*
bool USCITE_rising(uint8_t pin);
bool USCITE_falling(uint8_t pin);*/


#endif
