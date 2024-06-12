
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  CAN BUS.H //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef CAN_BUS_H
#define CAN_BUS_H

#include <stdint.h>

#define DATARATE_2MB     2000000
#define DATARATE_1_5MB   1500000
#define DATARATE_1MB     1000000
#define DATARATE_800KB   800000
#define DATARATE_500KB   500000


#define CAN_ERROR   0x00
#define CAN_COMMAND 0x10
#define CAN_DATA    0x20
#define CAN_PING    0x30

#define CAN_ENCODER_ID  0x01
#define CAN_POTENZA_ID  0x02
#define CAN_SCHERMO_ID  0x0A
#define CAN_COMPUTER_ID 0x0B


#define CAN_myID 0x01


void CAN_begin();
bool CAN_incoming_data();

bool CAN_sendMsgBuf(uint8_t id, uint8_t boh, uint8_t len, uint8_t* msg);
void CAN_send_ping();

void CAN_send_lunghezza_anta(uint8_t id, uint16_t len, uint32_t len_raw = 0);
void CAN_send_stato_rullo(uint8_t st, uint8_t vel);
void CAN_send_posizione_anta(uint8_t id, uint8_t ogg, uint8_t pos, uint32_t sp = 0);
void CAN_send_stato_pulizia(uint8_t st);
void CAN_send_OK_stampa(uint8_t id, bool st);
void CAN_send_errore_stampa(uint8_t err);
void CAN_send_avvio();
void CAN_richiedi_step_pezzo(uint8_t a_id, uint32_t len);

#endif
