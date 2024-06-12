//////////////////////////////////////////////////////////////////
//                    Scheda Potenza FM                         //
//                                                              //
//    V 0.1         14/07/2022                by Nicola Grizzo  //
//////////////////////////////////////////////////////////////////
//  Ultima modifica                              13/07/22

#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include <stdint.h>
#include "global_var_def.h"
#include "can_bus.h"
#include "RS232_debug.h"
#include "motore.h"
#include "encoder.h"
#include "fotocellule.h"
#include "sollevatore.h"
#include "emergenze.h"
#include "stampa.h"
#include "ingressi_uscite_prg.h"
#include "segnalazione.h"
#include "laser.h"
#include "pulizia.h"
#include "telecamera_maniglie.h"


// TODO gestire posizione sollevatore per blocco motore trazione



void setup() {
  ////--
  //--Serial.begin(115200);
  ///while (!Serial) {
  //; // wait for serial port to connect.
  //}

  digital_outputs.setLatch();
  digital_outputs.setAll(0);

  //comm_protocols.init();

  RS232_begin();
  CAN_begin();
  MOT_begin();
  INGRESSI_USCITE_begin();
  EMERG_begin();
  SGN_begin();
  SGN_intervento(SGN_INT_BOOT);
  PUL_begin();
  //STM_begin();
  CAN_send_avvio();
  // TODO aggiungere check iniziali stato macchina e comunicazione


  //MAC_start(1 * MOT_MAX_FW_SPEED / 3.0);
}

uint32_t last_cycle;

uint32_t last_speed_sent;

void loop() {

  CAN_incoming_data();
  if (millis() - ENC_last_print > 2000) {
    ENC_print();
    ENC_last_print = millis();
  }

  INGRESSI_readAll();

  EMERG_check();
  if (EMERG_arresto_ch) {
    if (EMERG_arresto) {
      MAC_stop();
      SOL_stop();
      LAS_set_mode(LAS_MODE_OFF);
      SGN_emergenza(SGN_EMG_EMG);
    } else {
      SGN_emergenza_reset(SGN_EMG_EMG);
      SGN_intervento_reset(SGN_INT_BOOT);
    }
  }

  if (millis() - last_speed_sent > 1000) {
    last_speed_sent = millis();
    CAN_send_stato_rullo(MAC_running, ENC_speed);
  }

  MAC_FC_disable_check();
  FC_check();
  SOL_check();
  ENC_check();
  STM_check();
  TELM_check();
  SGN_check();
  RS232_check();

  USCITE_writeAll();


  ////--
  //--uint32_t cycle = micros() - last_cycle;
  //--if (cycle > 600) Serial.println(cycle); // check velocità superloop 600us
  //--last_cycle = micros();
  //delay(200);
  ////--Serial.print("------> ");
}
