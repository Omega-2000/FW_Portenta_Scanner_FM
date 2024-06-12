
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//   SEGNALAZIONE   //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              27/07/22

#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>

#include "segnalazione.h"
#include "segnalazione_queue.h"
#include "ingressi_uscite_prg.h"
#include "can_bus.h"

uint32_t SGN_last_short_pulse;
uint32_t SGN_last_medium_pulse;
uint32_t SGN_last_long_pulse;
bool SGN_pulse_state;

uint32_t SGN_last_single_pulse;
uint32_t SGN_single_pulse;


uint8_t SGN_cause_emergenza;
uint8_t SGN_cause_anomalia;
uint8_t SGN_cause_safe;
uint8_t SGN_cause_intervento;



//\\//\\// CODA SEGNALAZIONI //\\//\\//
// potrebbe essere una coda con priorità low-first
// da valutare per eventuale lettura top() per segnalazione corrente
// struttura dati in array con array di puntatori per ordine?
// in modo da salvare i dati nel pimo posto libero e
// non dover trasferire dati all'ordinamento






void SGN_begin() {
  USCITE_setDir(SGN_ROSSO_PIN, 1);
  USCITE_setDir(SGN_GIALLO_PIN, 1);
  USCITE_setDir(SGN_VERDE_PIN, 1);
  USCITE_setDir(SGN_BLU_PIN, 1);
  USCITE_setDir(SGN_BUZZER_PIN, 1);
}

void SGN_emergenza(uint8_t causa) {
  Sgn_obj o = Sgn_obj(CAN_myID, causa, 0x40);
  SGN_queue.enqueue(o);
  /*
    //--Serial.println("Segnalazione emergenza attivata");
    SGN_cause_emergenza |= causa;
    SGN_safe_reset(0xff);
    USCITE_write(SGN_ROSSO_PIN, 1);
    SGN_buzzer_single_long_pulse();*/
}
void SGN_emergenza_reset(uint8_t causa) {
  SGN_queue.removeByError(CAN_myID, causa);
  /*
    SGN_cause_emergenza &= ~causa;
    if (!SGN_cause_emergenza) {
    //--Serial.println("Segnalazione emergenza disattivata");
    USCITE_write(SGN_ROSSO_PIN, 0);
    USCITE_write(SGN_BUZZER_PIN, 0);
    } else {
    //--Serial.print("Segnalazione emergenza non disattivata causa: ");
    //--Serial.println(SGN_cause_emergenza);
    }
  */
}

void SGN_anomalia(uint8_t causa) {
  Sgn_obj o = Sgn_obj(CAN_myID, causa, 0x80);
  SGN_queue.enqueue(o);
  /*
    //--Serial.println("Segnalazione anomalia attivata");
    SGN_cause_anomalia |= causa;
    SGN_safe_reset(0xff);
    USCITE_write(SGN_GIALLO_PIN, 1);
  */
}
void SGN_anomalia_reset(uint8_t causa) {
  SGN_queue.removeByError(CAN_myID, causa);
  /*
    SGN_cause_anomalia &= ~causa;
    if (!SGN_cause_anomalia) {
    //--Serial.println("Segnalazione anomalia disattivata");
    USCITE_write(SGN_GIALLO_PIN, 0);
    } else {
    //--Serial.print("Segnalazione anomalia non disattivata causa: ");
    //--Serial.println(SGN_cause_anomalia);
    }*/
}

void SGN_safe(uint8_t causa) {

  /*
    SGN_cause_safe |= causa;
    if (!SGN_cause_emergenza) {
    //--Serial.println("Segnalazione safe attivata");
    USCITE_write(SGN_VERDE_PIN, 1);
    }*/
}
void SGN_safe_reset(uint8_t causa) {
  /*
    SGN_cause_safe &= ~causa;
    if (!SGN_cause_safe) {
    //--Serial.println("Segnalazione safe disattivata");
    USCITE_write(SGN_GIALLO_PIN, 0);
    } else {
    //--Serial.print("Segnalazione safe non disattivata causa: ");
    //--Serial.println(SGN_cause_safe);
    }
    USCITE_write(SGN_VERDE_PIN, 0);*/
}

void SGN_intervento(uint8_t causa) {
  Sgn_obj o = Sgn_obj(CAN_myID, causa, 0xC0);
  SGN_queue.enqueue(o);
  /*
    //--Serial.println("Segnalazione intervento richiesto attivata");
    SGN_cause_intervento |= causa;
    USCITE_write(SGN_BLU_PIN, 1);
    SGN_buzzer_single_short_pulse();*/
}
void SGN_intervento_reset(uint8_t causa) {
  SGN_queue.removeByError(CAN_myID, causa);
  /*
    SGN_cause_intervento &= ~causa;
    if (!SGN_cause_intervento) {
    //--Serial.println("Segnalazione intervento disattivata");
    USCITE_write(SGN_BLU_PIN, 0);
    } else {
    //--Serial.print("Segnalazione intervento non disattivata causa: ");
    //--Serial.println(SGN_cause_intervento);
    }*/
}

void SGN_buzzer_single_short_pulse() {
  USCITE_write(SGN_BUZZER_PIN, 1);
  SGN_last_single_pulse = millis();
  SGN_single_pulse = SGN_SHORT_PULSE_TIME;
}
void SGN_buzzer_single_medium_pulse() {
  USCITE_write(SGN_BUZZER_PIN, 1);
  SGN_last_single_pulse = millis();
  SGN_single_pulse = SGN_MEDIUM_PULSE_TIME;
}
void SGN_buzzer_single_long_pulse() {
  USCITE_write(SGN_BUZZER_PIN, 1);
  SGN_last_single_pulse = millis();
  SGN_single_pulse = SGN_LONG_PULSE_TIME;
}



void SGN_check() {
  Sgn_obj SGN_current = SGN_queue.top();      // leggo segnalazione corrente
  if (SGN_current.getErrorCode() != 0) {       // se segnalazione presente
    switch (SGN_current.getSignalRegister()) {    // switcho registro segnalazione
      case 0xff: // notifica nulla
        USCITE_write(SGN_ROSSO_PIN, 0);
        USCITE_write(SGN_GIALLO_PIN, 0);
        USCITE_write(SGN_VERDE_PIN, 1);
        USCITE_write(SGN_BLU_PIN, 0);
        //SGN_buzzer_single_long_pulse();
        break;

      case 0xc0: // azione richiesta
        USCITE_write(SGN_ROSSO_PIN, 0);
        USCITE_write(SGN_GIALLO_PIN, 0);
        USCITE_write(SGN_VERDE_PIN, 1);
        USCITE_write(SGN_BLU_PIN, 1);
        //SGN_buzzer_single_long_pulse();
        break;

      case 0x80: // warning
        USCITE_write(SGN_ROSSO_PIN, 0);
        USCITE_write(SGN_GIALLO_PIN, 1);
        USCITE_write(SGN_VERDE_PIN, 0);
        USCITE_write(SGN_BLU_PIN, 0);
        //SGN_buzzer_single_long_pulse();
        break;

      case 0x40: // errore
        USCITE_write(SGN_ROSSO_PIN, 1);
        USCITE_write(SGN_GIALLO_PIN, 0);
        USCITE_write(SGN_VERDE_PIN, 0);
        USCITE_write(SGN_BLU_PIN, 0);
        //SGN_buzzer_single_long_pulse();
        if (!SGN_single_pulse && (millis() - SGN_last_single_pulse > SGN_LONG_PULSE_TIME)) {
          //--Serial.println("avvio beep");
          SGN_buzzer_single_long_pulse();
        }
        break;

      case 0x10: // errore grave
        USCITE_write(SGN_ROSSO_PIN, 1);
        USCITE_write(SGN_GIALLO_PIN, 0);
        USCITE_write(SGN_VERDE_PIN, 0);
        USCITE_write(SGN_BLU_PIN, 0);
        //if (millis() - SGN_last_single_pulse > SGN_single_pulse) SGN_buzzer_single_long_pulse();
        break;
    }
  } else {                                    // coda segnalazione vuota
    USCITE_write(SGN_ROSSO_PIN, 0);
    USCITE_write(SGN_GIALLO_PIN, 0);
    USCITE_write(SGN_VERDE_PIN, 1);
    USCITE_write(SGN_BLU_PIN, 0);
  }

  // impulso singolo buzzer
  if (SGN_single_pulse && (millis() - SGN_last_single_pulse > SGN_single_pulse)) {
    //--Serial.println("stop beep");
    USCITE_write(SGN_BUZZER_PIN, 0);
    SGN_single_pulse = 0;
    SGN_last_single_pulse = millis();
  }

  /* //da togliere
    if (!SGN_cause_safe && !SGN_cause_emergenza && !SGN_cause_anomalia) {
    SGN_safe(0x01);
    }*/

  /*
    if(SGN_cause_emergenza && (millis() - SGN_last_single_pulse > SGN_LONG_PULSE_TIME)){
    SGN_buzzer_single_long_pulse();
    }*/


  /*
    if (millis() - SGN_last_short_pulse > SGN_SHORT_PULSE_TIME) {
    SGN_last_short_pulse = millis();
    }
    if (millis() - SGN_last_medium_pulse > SGN_MEDIUM_PULSE_TIME) {
    SGN_last_medium_pulse = millis();

    USCITE_write(SGN_ROSSO_PIN, 1);
    }
    if (millis() - SGN_last_long_pulse > SGN_LONG_PULSE_TIME) {
    SGN_last_long_pulse = millis();
    }*/
}
