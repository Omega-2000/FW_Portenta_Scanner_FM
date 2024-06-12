
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//   STAMPA.H   //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              18/07/22


#ifndef STAMPA_H
#define STAMPA_H


#include <stdint.h>


#define STM_READY_PIN         IO_READ_CH_PIN_02 + 32
#define STM_PRES_PIN          IO_READ_CH_PIN_03 + 32
#define STM_FINE_CICLO_PIN    IO_READ_CH_PIN_04 + 32
#define STM_PAP_END_PIN       IO_READ_CH_PIN_05 + 32

#define STM_READY_PIN_DIR         IO_WRITE_CH_PIN_02
#define STM_PRES_PIN_DIR          IO_WRITE_CH_PIN_03
#define STM_FINE_CICLO_PIN_DIR    IO_WRITE_CH_PIN_04
#define STM_PAP_END_PIN_DIR       IO_WRITE_CH_PIN_05

#define STM_ABBASSA_PIN 7

extern uint8_t STM_state;
extern uint8_t STM_position;
extern uint8_t STM_position_old;
extern bool STM_position_reset;
extern bool STM_pedale_state;
extern bool STM_enabled;

void STM_begin();

void STM_abbassa();
void STM_alza();
inline bool STM_printer_ready();
bool STM_print();

void STM_check();

#endif
