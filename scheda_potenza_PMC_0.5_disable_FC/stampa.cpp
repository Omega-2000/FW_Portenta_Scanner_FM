
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   STAMPA   //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              20/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>


#include "global_var_def.h"
#include "stampa.h"
#include "emergenze.h"
#include "ingressi_uscite_prg.h"
#include "can_bus.h"
#include "RS232_debug.h"

/*
  #define STM_READY_PIN         (IO_READ_CH_PIN_02 + 32)
  #define STM_PRES_PIN          (IO_READ_CH_PIN_03 + 32)
  #define STM_FINE_CICLO_PIN    (IO_READ_CH_PIN_04 + 32)
  #define STM_PAP_END_PIN       (IO_READ_CH_PIN_05 + 32)

  #define STM_ABBASSA_PIN 7
*/

uint8_t STM_state;
uint8_t STM_position;
uint8_t STM_position_old;
bool STM_presence;
bool STM_enabled;
bool STM_paper_end;
bool STM_paper_end_sent;

bool STM_trigger_state = 0;
uint32_t STM_trigger_time = 0;

void STM_begin() {
  /*
    USCITE_setDir(STM_READY_PIN_DIR, 0);
    USCITE_setDir(STM_PRES_PIN_DIR, 0);
    USCITE_setDir(STM_FINE_CICLO_PIN_DIR, 0);
    USCITE_setDir(STM_PAP_END_PIN_DIR, 0);

    USCITE_setDir(STM_READY_PIN, 0);
    USCITE_setDir(STM_PRES_PIN, 0);
    USCITE_setDir(STM_FINE_CICLO_PIN, 0);
    USCITE_setDir(STM_PAP_END_PIN, 0);*/
}



bool STM_check_pre_stampa() {
  return !STM_printer_ready();
}

inline bool STM_printer_ready() {
  STM_presence = INGRESSI_read(STM_PRES_PIN);
  // ready se è ferma, in posizione 0 e c'è l'etichetta
  return STM_presence;
}

bool STM_print(uint8_t id) {
  bool ret = 0;
  //if (STM_enabled) {
  if (STM_printer_ready()) {
    digital_outputs.set(STM_ABBASSA_PIN, 1);
    STM_trigger_time = millis();
    STM_trigger_state = 1;
    DEBUG("Avvio stampa\n");
    ret = 1;
  } else {
    DEBUG("Stampa non pronta\n");
    MAC_stop();
    CAN_send_stato_rullo(MOT_state, (MOT_state == 2 ? MOT_bw_speed : MOT_fw_speed) * 10.0);
    CAN_send_errore_stampa(0);
    MAC_reset();

    //MAC_stop();
  }
  /*} else {
    MAC_stop();
    //--
    Serial.println("Stampante non abilitata");
    }
    STM_enabled = 0; // al termine della stampa la disabilito*/

  CAN_send_OK_stampa(id, ret);
  return ret;
}

void STM_check() {
  //STM_state = INGRESSI_read(STM_READY_PIN);
  STM_paper_end = INGRESSI_read(STM_PAP_END_PIN);
  if (STM_paper_end) {
    if (!STM_paper_end_sent) {
      CAN_send_errore_stampa(1);
      DEBUG("END PAPER\n");
    }
    STM_paper_end_sent = 1;
  } else {
    STM_paper_end_sent = 0;
  }

  if (STM_trigger_state && (millis() - STM_trigger_time >= 500)) {
    digital_outputs.set(STM_ABBASSA_PIN, 0);
    STM_trigger_state = 0;
  }


}
