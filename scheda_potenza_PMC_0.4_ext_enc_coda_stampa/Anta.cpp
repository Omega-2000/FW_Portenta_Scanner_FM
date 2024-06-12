
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   ANTA    //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "Anta.h"
#include "RS232_debug.h"


uint8_t Anta::last_id;

Anta::Anta(uint32_t in, uint32_t fi, uint8_t ui, uint8_t uf, uint8_t _id) {
  inizio = in;
  fine = fi;
  uso_inizio = ui;
  uso_fine = uf;
  if (_id != NULL) {
    id = _id;
  } else {
    id = ++ last_id;
  }
}


void Anta::copy(Anta a) {
  id = a.getId();
  inizio = a.getBegin();
  fine = a.getEnd();
  uso_inizio = a.getBeginUse();
  uso_fine = a.getEndUse();
  sollevabile = a.getLiftEnabled();
}


void Anta::setId(uint8_t id_) {
  id = id_;
}
uint8_t Anta::getId() {
  return id;
}


void Anta::setBegin(uint32_t in) {
  inizio = in;
}
uint32_t Anta::getBegin() {
  return inizio;
}

void Anta::setEnd(uint32_t fi) {
  
  //--Serial.println("Anta anta.cpp set");
  //--Serial.println(fine);
  //--Serial.println(fi);
  fine = fi;
  //--Serial.println("Anta anta.cpp set fine");
}
uint32_t Anta::getEnd() {
  return fine;
}


void Anta::setLength(uint32_t le) {
  fine = inizio + le;
}
uint32_t Anta::getLength() {
  return fine - inizio;
}


void Anta::setBeginUse(uint8_t ui) {
  uso_inizio = ui;
}
uint8_t Anta::getBeginUse() {
  return uso_inizio;
}


void Anta::setEndUse(uint8_t uf) {
  uso_fine = uf;
}
uint8_t Anta::getEndUse() {
  return uso_fine;
}



void Anta::setLiftEnable(bool en){
  sollevabile = en;
}
bool Anta::getLiftEnabled(){
  return sollevabile;
}


void Anta::print() {
DEBUG("Anta #");
DEBUG(id);
DEBUG(" - BeginUse ");
DEBUG(uso_inizio);
DEBUG(" - EndUse ");
DEBUG(uso_fine);
DEBUG("\n");
DEBUG("Begin ");
DEBUG(inizio);
DEBUG(" - End ");
DEBUG(fine);
DEBUG(" - Len ");
DEBUG(getLength());
DEBUG("\n");
}
