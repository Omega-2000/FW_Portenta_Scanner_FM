
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\  SEGNALAZIONE QUEUE  \\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//


#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "segnalazione_queue.h"
#include "RS232_debug.h"


Sgn_obj::Sgn_obj(uint8_t is, uint16_t ce, uint8_t rs) {
  id_scheda = is;
  codice_errore = ce;
  registro_segnalazione = rs;
}

void Sgn_obj::copy(Sgn_obj o) {

  id_scheda = o.id_scheda;
  codice_errore = o.codice_errore;
  registro_segnalazione = o.registro_segnalazione;
}

void Sgn_obj::setId(uint8_t is) {
  id_scheda = is;
}
uint8_t Sgn_obj::getId() {
  return id_scheda;
}

void Sgn_obj::setErrorCode(uint16_t ce) {
  codice_errore = ce;
}
uint16_t Sgn_obj::getErrorCode() {
  return codice_errore;
}

void Sgn_obj::setSignalRegister(uint8_t rs) {
  registro_segnalazione = rs;
}
uint8_t Sgn_obj::getSignalRegister() {
  return registro_segnalazione;
}

void Sgn_obj::print() {
  char pr[100];
  sprintf(pr, "Segnalazione da 0x%x - Codice errore 0x%x - Registro segnalazione 0x%x \n\0", id_scheda, codice_errore, registro_segnalazione);
  DEBUG(pr);
}










Sgn_queue::Sgn_queue() {
  sgn_queue[SGN_QUEUE_SIZE];
  highest_priority = 0xff;
}

bool Sgn_queue::enqueue(Sgn_obj o) {
  DEBUG("sgn enqueue\n");
  o.print();
  for (uint8_t i = 0; i < SGN_QUEUE_SIZE; i++) {
    if (sgn_queue[i].getErrorCode() == 0) {         // se oggetto vuoto
      sgn_queue[i].copy(o);                           // inserisco l'oggetto
      this->calcTop();                                 // ricalcolo priorità
      return 1;                                       // esco e do ok
    }
  }                                               // se non c'è spazio
  return 0;                                         // esco e do errore
}

bool Sgn_queue::calcTop() {
  uint8_t priority_index = 0xff;
  uint8_t priority_code = 0xff;
  for (uint8_t i = 0; i < SGN_QUEUE_SIZE; i++) {
    if (sgn_queue[i].getErrorCode() != 0 &&                 // se oggetto non vuoto e
        sgn_queue[i].getSignalRegister() < priority_code) { // se codice minore di quelli precedenti
      priority_index = i;                                     // salvo l'indice
      priority_code = sgn_queue[i].getSignalRegister();       // salvo il codice
    }
  }
  highest_priority = priority_index;
  //DEBUG("highest priority: ");
  //DEBUG(highest_priority);
  //DEBUG("\n");
  return (highest_priority != 0xff);
}

Sgn_obj Sgn_queue::top() {
  Sgn_obj o;                                      // creo oggetto vuoto
  if (highest_priority != 0xff) {                   // se l'indice è diverso da non definito
    o.copy(sgn_queue[highest_priority]);               // copio l'oggetto nell'output
  }
  //DEBUG("top\n");
  //o.print();
  this->calcTop();                               // ricalcolo priorità
  return o;
}

bool Sgn_queue::removeByError(uint8_t is, uint16_t ce) {
  DEBUG("sgn remove\n");
  for (uint8_t i = 0; i < SGN_QUEUE_SIZE; i++) {
    if ((sgn_queue[i].getId() == is) &&           // se id scheda è uguale a quello dell'errore da cancellare e
        (sgn_queue[i].getErrorCode() == ce)) {    // se codice errore è uguale a quello da cancellare
      sgn_queue[i].print();
      Sgn_obj o;                                    // creo oggetto vuoto
      sgn_queue[i].copy(o);                         // sovrascrivo l'oggetto nella coda con oggetto vuoto
      this->calcTop();                               // ricalcolo priorità
      return 1;                                     // ritorno ok
    }
  }
  return 0;                                       // ritorno errore
}



Sgn_queue SGN_queue;
