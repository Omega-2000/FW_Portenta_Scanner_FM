
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//  SOLLEVATORE.H   //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              18/07/22


#ifndef SOLLEVATORE_H
#define SOLLEVATORE_H


#include <stdint.h>


#define SOL_PED_PIN       DIN_READ_CH_PIN_03
#define SOL_POS1_PIN      DIN_READ_CH_PIN_04
#define SOL_POS2_PIN      DIN_READ_CH_PIN_05

#define SOL_ALZA_PIN      2
#define SOL_ABBASSA_PIN   3

#define SOL_MIN_LEN       1300
#define SOL_MAX_LEN       3000
#define SOL_ROLLERS_DIST  1079
#define SOL_FC_OFFSET     47

extern uint8_t SOL_state;
extern uint8_t SOL_position;
extern uint8_t SOL_position_old;
extern uint8_t SOL_position_reset;
extern bool SOL_pedale_state;
extern bool SOL_enabled;

void SOL_alza();
void SOL_abbassa();
void SOL_stop();

void SOL_check();

bool SOL_isLiftable(double len);
double SOL_getOverhang(double len);

#endif
