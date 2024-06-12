
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  ENCODER.H   \\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22

#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

#define ENC_CHANNEL           0


#define ENC_MM_STP            0.047627 //0.04764011 //0.04759126
#define ENC_MM_OFFSET         0.03996 //0.7241253 //1.22582995


#define ENC_POINTS            9

#define ENC_POINTS_MARGIN     50
#define ENC_POINT_CAMERA_TOP  100   // 0
#define ENC_POINT_CAMERA_LAT  865   // 1
#define ENC_POINT_PRE_PRINT   1820//1770 (funziona ma è al limite) //1370 (deafult)  // 2 (posizione di invio comando stampa per coda stampa)
#define ENC_POINT_PRINT       2170  // 3
#define ENC_POINT_END_GUIDE   3160  // 4
#define ENC_POINT_END_CONV    4980  // 5
#define ENC_POINT_SOL_CENTER  4470  // 6 (end_conv + calcolato)
//                                  // 7 ("in posizione")
//                                  // 8 (dequeue) (7+2=2 > ENC_POINTS)


extern uint32_t ENC_last_print;
extern double ENC_mm_stp;
extern uint32_t ENC_points_distances[ENC_POINTS];
extern uint32_t ENC_last_int_pulses;
extern double ENC_speed;


void FC_int_rise_routine();
void FC_int_fall_routine();

/*
extern void(*ENC_points_begin_functions[3])();
extern void(*ENC_points_end_functions[3])();
*/


inline double ENC_getDistance() {
  // aggiungere conversione
  return encoders[0].getPulses() * ENC_mm_stp;
}
inline uint32_t ENC_getDistance_pulses() {
  return encoders[0].getPulses();
}
inline double ENC_convert_to_mm(uint32_t pl){
  return pl * ENC_mm_stp;
}
inline uint32_t ENC_convert_to_pulses(double mm){
  return mm / ENC_mm_stp;
}

void ENC_print();
inline void ENC_reset();

void ENC_enqueue_begin();
void ENC_enqueue_end();
void ENC_send_misura_combinata(uint8_t a_id, uint32_t l_stp);

void ENC_check();

/*
void ENC_point_1_begin();
void ENC_point_2_begin();
void ENC_point_3_begin();
void ENC_point_1_end();
void ENC_point_2_end();
void ENC_point_3_end();
*/
#endif
