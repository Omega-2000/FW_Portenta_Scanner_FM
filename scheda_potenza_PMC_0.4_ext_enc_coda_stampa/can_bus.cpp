
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   CAN BUS  //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#include <Arduino_MachineControl.h>
#include <CAN.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "can_bus.h"
#include "motore.h"
#include "stampa.h"
#include "laser.h"
#include "segnalazione.h"
#include "segnalazione_queue.h"
#include "pulizia.h"
#include "RS232_debug.h"
#include "encoder.h"

//\\//\\//\\//\\ Preparazione //\\//\\//\\//\\//

void CAN_begin() {

  //--Serial.println("Start CAN initialization");
  comm_protocols.enableCAN();
  comm_protocols.can.frequency(DATARATE_500KB);
  //--Serial.println("CAN BUS Init OK!");
}


bool CAN_incoming_data() {
  mbed::CANMessage msg;
  while (comm_protocols.can.read(msg)) {
    uint8_t* buf = msg.data;
    unsigned long canId = msg.id;

    //--Serial.print("message: id " + String(canId, HEX));
    //--Serial.print(" - ");
    char st[50];
    sprintf(st, "message: id %x - ");
    DEBUG(st);
    for (uint8_t i = 0; i < 8; i++) {
      if (buf[i] < 0x10) {
        DEBUG("0");
      }
      sprintf(st, "%x ", buf[i]);
      DEBUG(st);
    }
    DEBUG("\n");

    // elaborazione
    if ((canId & 0xf0) == CAN_ERROR) { // errori
      DEBUG("Errore\n");

      uint16_t codice_errore = ((uint16_t)buf[0] << 8) + buf[1];
      if (codice_errore) {                                        // se codice errore > 0 è un set errore
        Sgn_obj o = Sgn_obj(canId & 0x0f, codice_errore, buf[2]);
        SGN_queue.enqueue(o);
      } else {
        SGN_queue.removeByError(canId & 0x0f, codice_errore);
      }

    } else if ((canId & 0xf0) == CAN_COMMAND) { // comandi
      DEBUG("Comando\n");

      if ((canId & 0x0f) == CAN_ENCODER_ID) { // da encoder esp
        if (buf[0] == 0xff) { // misura anta
          lunghezza_step = ((uint32_t)buf[1] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 8) | buf[4];
          lunghezza_step_ricevuta = 1;
          ENC_send_misura_combinata(buf[5], lunghezza_step);
        }
      } else if ((canId & 0x0f) == CAN_SCHERMO_ID) { // da schermo
        if (buf[0] == 0x02) { // comando motore
          if (buf[1] == 0x00) {
            MAC_stop();
            DEBUG("Stop motore\n");
          } else  if (buf[1] == 0x01) { // start
            MAC_start((float)buf[2] * MOT_MAX_FW_SPEED / 3.0);
            DEBUG("Start motore\n");
          } else if (buf[1] == 0x02) { // retro
            MAC_stop();
            MOT_start((float)buf[2] * MOT_BW_SPEED / 3.0, 1);
            DEBUG("Retro motore\n");
          } else if (buf[1] == 0x03) { // arresto macchina
            MAC_stop();
            MAC_reset();
            DEBUG("Stop motore con reset\n");
          }
          CAN_send_stato_rullo(MAC_running, (MOT_state == 2 ? MOT_bw_speed : MOT_fw_speed) * 10.0);
        } else if (buf[0] == 0x05) { // ok stampa
          STM_enabled = buf[2];
          // buf[3]; // ID ANTA TODO GESTIONE
        } else if (buf[0] == 0x08) { // pulizia rullo
          if (buf[1] == 1) { // inizio pulizia
            PUL_start();
          } else { // fine pulizia
            PUL_stop();
          }
        } else if (buf[0] == 0x0A) { // comando riavvio
          if (buf[1] == 0x00) {
            //riavvio
          }
        }
      } else if ((canId & 0x0f) == CAN_COMPUTER_ID) { // da computer
        if (buf[0] == 0x04) { // comando laser
          // buf[1] -> laser 0/1, buf[2] -> Stato on/off/auto 0/1/2
          LAS_set_mode(buf[2], buf[1]);
        }
      }
    } else if ((canId & 0xf0) == CAN_DATA) { // dati
      DEBUG("Dato\n");

    } else if ((canId & 0xf0) == CAN_PING) { // ping
      CAN_send_ping();
      DEBUG("Ping\n");
    }


    /*
      if (canId == CAN_myID) { // ELABORAZIONE VECCHIA TODO RIMUOVERE QUANDO ATTIVA COMUNICAZIONE NUOVA
      if (buf[0] == 0x00) { // comando motore
        if (buf[1] == 0x01) { // avanti
          INV.start(buf[2], 0);
          INV.loop();
        } else if (buf[1] == 0x02) { // indietro
          INV.start(buf[2], 1);
          INV.loop();
        } else if (buf[1] == 0x00) { // stop
          INV.stop();
          INV.loop();
        }
      }

      if (buf[0] == 0x01) { // comando relè
        digitalWrite(REL_1, buf[1]);
      }

      if (buf[0] == 0x10) { // velocità rullo
        velocita = 2560; // decidere come trasmetterla
        velocita_last_tm_old = velocita_last_tm;
        velocita_last_tm = millis();
        FC_distanza_upd = 1;
      }
      }
    */

  }
  return 0;
}


bool CAN_sendMsgBuf(uint8_t id, uint8_t boh, uint8_t len, uint8_t* msg) {
  mbed::CANMessage cmsg = mbed::CANMessage(id/*13ul*/, msg, len);
  if (comm_protocols.can.write(cmsg)) {

    DEBUG("Message sent:");
    DEBUG("message: id " + String(cmsg.id, HEX));
    DEBUG(" - ");
    for (uint8_t i = 0; i < 8; i++) {
      if (cmsg.data[i] < 0x10) DEBUG("0");
      char st[5];
      sprintf(st, "%x ", cmsg.data[i]);
      DEBUG(st);
    }
    DEBUG("\n");

    return 1;
  } else {
    DEBUG("Transmission Error: ");
    DEBUG(comm_protocols.can.tderror());
    DEBUG("\n")
    comm_protocols.can.reset();
  }
  return 0;
}


void CAN_send_ping() {
  uint8_t msg[8] = {0x01,
                    (uint32_t)versione >> 24, (uint32_t)versione >> 16, (uint32_t)versione >> 8, versione,
                    0x00, 0x00, 0x00
                   };
  CAN_sendMsgBuf(CAN_myID | CAN_PING, 0, 8, msg);
}



void CAN_send_lunghezza_anta(uint8_t id, uint16_t len, uint32_t len_raw) {
  uint8_t msg[8] = {0x01, 0x02, id, len >> 8, len, len_raw >> 16, len_raw >> 8, len_raw};
  CAN_sendMsgBuf(CAN_myID | CAN_DATA, 0, 8, msg);
  //--Serial.print("Invio lunghezza anta: ");
  //--Serial.print(len);
  //--Serial.print(" - raw: ");
  //--Serial.println(len_raw);
}

void CAN_send_stato_rullo(uint8_t st, uint8_t vel) {
  uint8_t msg[8] = {0x02, st, vel, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN_sendMsgBuf(CAN_DATA | CAN_myID, 0, 8, msg);
}

void CAN_send_posizione_anta(uint8_t id, uint8_t ogg, uint8_t pos, uint32_t sp) {
  uint8_t msg[8] = {0x03, id, ogg, pos, sp >> 24, sp >> 16, sp >> 8, sp};
  CAN_sendMsgBuf(CAN_DATA | CAN_myID, 0, 8, msg);
}

void CAN_send_stato_pulizia(uint8_t st) {
  uint8_t msg[8] = {0x08, st, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN_sendMsgBuf(CAN_DATA | CAN_myID, 0, 8, msg);
}


void CAN_send_errore_stampa(uint8_t err) {
  uint8_t msg[8] = {0x04, 04, (err ? 0x10 : 0xC0), err, 0x00, 0x00, 0x00, 0x00};
  CAN_sendMsgBuf(CAN_ERROR | CAN_myID, 0, 8, msg);
}

void CAN_send_avvio() {
  uint8_t msg[8] = {0x05, 0x05, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN_sendMsgBuf(CAN_ERROR | CAN_myID, 0, 8, msg);
}



void CAN_richiedi_step_pezzo(uint8_t a_id, uint32_t len){
  uint8_t msg[8] = {0xff, a_id, len >> 24, len >> 16, len >> 8, len, 0x00, 0x00};
  CAN_sendMsgBuf(CAN_ENCODER_ID | CAN_COMMAND, 0, 8, msg);
}


/*
  void CAN_send_misurazione_lato(uint16_t misura, uint16_t decimi) { // TODO chiedere tipo messaggio  e se encoder!!!!!!!!!!!!
  uint8_t msg[8] = {0x01, 0x02, misura >> 8, misura, decimi >> 8, decimi, 0x00, 0x00};
  CAN.sendMsgBuf(0x21, 0, 8, msg);
  }

  void CAN_send_misurazione_lato(float misura) {
  CAN_send_misurazione_lato((uint16_t)misura, (uint16_t)((misura - (uint16_t)misura) * 10.0));
  }*/
