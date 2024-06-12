
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//   FOTOCELLULE    //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "fotocellule.h"
#include "encoder.h"
#include "motore.h"
#include "can_bus.h"
#include "sollevatore.h"
#include "emergenze.h"
#include "ingressi_uscite_prg.h"
#include "segnalazione.h"

uint8_t FC_PINS[] = {
  FC_0_PIN,
  FC_1_PIN,
  FC_2_PIN
};

bool FC_state[3], FC_state_old[3];
bool FC_rising[3], FC_falling[3];


uint32_t FC_last_dist_fc1_rise;
uint32_t FC_last_dist_fc1_fall;
uint32_t FC_last_dist_fc2_rise;
uint32_t FC_last_dist_fc2_fall;

bool FC_2_override;



bool FC_read(uint8_t n) {
  return FC_state[n];
}

bool FC_read_all() {
  bool ch;

  FC_state_old[0] = FC_state[0];
  if (MAC_FC_enabled) {
    FC_state[0] = INGRESSI_read(FC_PINS[0]);
    FC_rising[0] = INGRESSI_rising(FC_PINS[0]);
    FC_falling[0] = INGRESSI_falling(FC_PINS[0]);
  } else {
    //FC_state[0] // non cambia
    FC_rising[0] = 0;
    FC_falling[0] = 0;
  }
  ch = FC_rising[0] | FC_falling[0];

  FC_state_old[1] = FC_state[1];
  FC_state[1] = INGRESSI_read(FC_PINS[1]);
  FC_rising[1] = INGRESSI_rising(FC_PINS[1]);
  FC_falling[1] = INGRESSI_falling(FC_PINS[1]);
  ch = FC_rising[1] | FC_falling[1];

  FC_state_old[2] = FC_state[2];
  FC_state[2] = !INGRESSI_read(FC_PINS[2]);
  FC_rising[2] = INGRESSI_falling(FC_PINS[2]);
  FC_falling[2] = INGRESSI_rising(FC_PINS[2]);
  ch = FC_rising[2] | FC_falling[2];
  return ch;
}

void FC_check() {
  FC_read_all();

  if (FC_rising[0]) { //fotocellula di misura
    ENC_enqueue_begin();
    //--Serial.println("Fotocellula di misura inizio");
  }
  if (FC_falling[0]) { //fotocellula di misura
    /*CAN_richiedi_step_pezzo();
      }
      if(lunghezza_step_ricevuta){*/
    ENC_enqueue_end();
    lunghezza_step_ricevuta = 0;
    //--Serial.println("Fotocellula di misura fine");
  }

  if (FC_rising[1]) {
    //--Serial.println("Fotocellula fine guida impegnata");
    FC_last_dist_fc1_rise = ENC_getDistance_pulses();
    //--Serial.println(FC_last_dist_fc1_rise);
    //--Serial.println(ENC_convert_to_mm(FC_last_dist_fc1_rise));
  }
  if (FC_falling[1]) {
    //--Serial.println("Fotocellula fine guida libera");
    FC_last_dist_fc2_rise = ENC_getDistance_pulses();
  }

  if (FC_rising[2]) { // fotocellula di fine rullo
    FC_last_dist_fc2_rise = ENC_getDistance_pulses();
  }
  if (FC_rising[2] || (MOT_state == 1 && FC_state[2])) { // fotocellula di fine rullo per stop
    //--Serial.println("Fotocellula di fine rullo impegnata");
    if (!FC_2_override) MOT_stop(); // fermo motore se fotocellula fine rullo impegnata e override per centraggio non attivo
    SGN_intervento(SGN_INT_FC3);
    // TODO check lunghezza pezzo SOL_enabled = 1;
  }
  if (FC_falling[2]) { // fotocellula di fine rullo
    FC_last_dist_fc2_fall = ENC_getDistance_pulses();
    SGN_intervento_reset(SGN_INT_FC3);
    //--Serial.println("Fotocellula di fine rullo libera");
  }
  if (!SOL_position && (FC_falling[2] || (!FC_state[2] && SOL_position_reset))) { // fotocellula di fine rullo per restart
    //--Serial.println("Fotocellula di fine rullo libera e sollevatore in posizione");
    FC_2_override = 0; // per sicurezza disabilitooverride se il pezzo viene spostato
    if (!EMERG_arresto && MAC_running) { // se macchina in funzione riavvio motore
      MOT_start();
    }
  }
}
