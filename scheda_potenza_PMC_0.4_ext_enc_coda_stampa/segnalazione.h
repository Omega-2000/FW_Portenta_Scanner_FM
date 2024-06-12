
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//  SEGNALAZIONE.H  //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              27/07/22


#ifndef SEGNALAZIONE_H
#define SEGNALAZIONE_H

#define SGN_ROSSO_PIN   IO_WRITE_CH_PIN_07
#define SGN_GIALLO_PIN  IO_WRITE_CH_PIN_08
#define SGN_VERDE_PIN   IO_WRITE_CH_PIN_09
#define SGN_BLU_PIN     IO_WRITE_CH_PIN_10
#define SGN_BUZZER_PIN  IO_WRITE_CH_PIN_11

#define SGN_SHORT_PULSE_TIME   200
#define SGN_MEDIUM_PULSE_TIME  1000
#define SGN_LONG_PULSE_TIME    1500


#define SGN_EMG_BOOT    0x01
#define SGN_EMG_EMG     0x02
#define SGN_INT_BOOT    0x03
#define SGN_INT_FC3     0x04


void SGN_begin();

void SGN_emergenza(uint8_t causa = 1);
void SGN_emergenza_reset(uint8_t causa = 1);
void SGN_anomalia(uint8_t causa = 1);
void SGN_anomalia_reset(uint8_t causa = 1);
void SGN_safe(uint8_t causa = 1);
void SGN_safe_reset(uint8_t causa = 1);
void SGN_intervento(uint8_t causa = 1);
void SGN_intervento_reset(uint8_t causa = 1);

void SGN_buzzer_single_short_pulse();
void SGN_buzzer_single_medium_pulse();
void SGN_buzzer_single_long_pulse();

void SGN_check();

#endif
