
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//   SOLLEVATORE    //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              18/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;


#include "global_var_def.h"
#include "sollevatore.h"
#include "emergenze.h"
#include "ingressi_uscite_prg.h"
#include "anta.h"
#include "anta_queue.h"
#include "fotocellule.h"

//#include "pulizia.h"


uint8_t SOL_state = 0;
uint8_t SOL_position = 0;
uint8_t SOL_position_old = 0;
uint8_t SOL_position_reset = 0;
bool SOL_pedale_state = 0;
bool SOL_pedale_state_old = 0;
bool SOL_enabled = 0;

void SOL_alza() {
  if (!EMERG_arresto && SOL_enabled) {
    digital_outputs.set(SOL_ALZA_PIN, 1);
    digital_outputs.set(SOL_ABBASSA_PIN, 0);
    SOL_state = 1;
    //--Serial.println("Sollevatore in alzo");
  } else {
    //--Serial.println("Sollevatore non abilitato all'alzo");
  }
}

void SOL_abbassa() {
  if (!EMERG_arresto) {
    digital_outputs.set(SOL_ALZA_PIN, 0);
    digital_outputs.set(SOL_ABBASSA_PIN, 1);
    SOL_state = 2;
    //--Serial.println("Sollevatore in abbasso");
  } else {
    //--Serial.println("Sollevatore non abilitato all'abbasso");
  }
}

void SOL_stop() {
  if (SOL_state) {
    digital_outputs.set(SOL_ALZA_PIN, 0);
    digital_outputs.set(SOL_ABBASSA_PIN, 0);
    SOL_state = 0;
    //--Serial.println("Sollevatore arrestato");
  }
}


void SOL_check() {
  SOL_position = !INGRESSI_read(SOL_POS2_PIN) + INGRESSI_read(SOL_POS1_PIN);
  SOL_position_reset = (SOL_position == 0) && (SOL_position_old == 1);
  SOL_position_old = SOL_position;

  // TODO gestione pedale
  SOL_pedale_state = INGRESSI_read(SOL_PED_PIN);

  if (!MOT_state && SOL_pedale_state && !SOL_pedale_state_old) {
    //--Serial.println("pedale premuto");
    // TODO CHECK RIBALTABILITA'
    // TODO CHECK POSIZIONE ANTA PER AVANZAMENTO E SOLLEVAMENTO
    Anta* anta_tmp = ANT_first_view();
    if (anta_tmp->getLiftEnabled() ) { // se sollevamento abilitato
      if (anta_tmp->getBeginUse() == 5) { // se fermo e deve ancora suoerare il check di fine rullo
        FC_2_override = 1;
        if (!EMERG_arresto && MAC_running) { // se macchina in funzione riavvio motore
          MOT_start();
        }
      } else if (anta_tmp->getBeginUse() == 7) { // altrimenti se in posizione sollevamento
        //SOL_enabled = 1; // forse non necessario
        SOL_alza();
      }
    }

  } else if (!SOL_pedale_state && SOL_pedale_state_old) {
    //--Serial.println("Pedale rilasciato");
    SOL_abbassa();
  }

  if (SOL_position == 0 && SOL_state == 2) {
    SOL_stop();
  }
  // verificare condizione di arresto in salita
  if (SOL_position == 2 && SOL_state == 1) {
    SOL_stop();
  }

  SOL_pedale_state_old = SOL_pedale_state;
}

bool SOL_isLiftable(double len) {
  return ((len >= SOL_MIN_LEN) && (len <= SOL_MAX_LEN));
}
double SOL_getOverhang(double len) {
  if (SOL_isLiftable(len)) {
    return SOL_FC_OFFSET + (len - SOL_ROLLERS_DIST) / 2;
  }
  return 0;
}
