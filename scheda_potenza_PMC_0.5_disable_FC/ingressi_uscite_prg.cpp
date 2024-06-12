
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//  INGRESSI  \\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              22/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>
#include "Wire.h"
#include <mbed.h>

#include "global_var_def.h"
#include "ingressi_uscite_prg.h"
#include "encoder.h"
#include "fotocellule.h"

//extern uint32_t INGRESSI_mask = INGRESSI_MASK;
uint32_t INGRESSI_state;
uint32_t INGRESSI_state_old;
uint32_t INGRESSI_rise;
uint32_t INGRESSI_fall;

//extern uint32_t INGRESSI_mask_2 = INGRESSI_MASK;
uint32_t INGRESSI_2_state;
uint32_t INGRESSI_2_state_old;
uint32_t INGRESSI_2_rise;
uint32_t INGRESSI_2_fall;

uint32_t USCITE_mask = 0; //USCITE_MASK;
uint32_t USCITE_state;
uint32_t USCITE_state_old;
uint32_t USCITE_rise;
uint32_t USCITE_fall;

// private
//mbed::InterruptIn FC_int(PJ_10);


void INGRESSI_USCITE_begin() {
  Wire.begin();
  Wire.setClock(400000);
  //--Serial.println("inizializzazione GPIO");
  if (digital_inputs.init()) {
    //--Serial.println("Digital input GPIO expander inizialized");
  } else {
    //--Serial.println("Digital input GPIO expander initialization fail!!");
  }

  if (digital_programmables.init()) {
    //--Serial.println("GPIO expander initialization done");
  } else {
    //--Serial.println("GPIO expander initialization fail!!");
  }
  digital_programmables.setLatch();

  //pinMode(FC_0_INT_PIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(FC_0_INT_PIN), FC_int_rise_routine, RISING);
  //attachInterrupt(digitalPinToInterrupt(FC_0_INT_PIN), FC_int_fall_routine, FALLING);

  //INGRESSI_readAll();
  //FC_int.rise(mbed::callback(FC_int_rise_routine));
  //FC_int.fall(mbed::callback(FC_int_fall_routine));
}

bool INGRESSI_readAll() {
  INGRESSI_state = digital_inputs.readAll() & INGRESSI_MASK;
  INGRESSI_2_state = digital_programmables.readAll() & INGRESSI_MASK;

  INGRESSI_rise = INGRESSI_state & ~INGRESSI_state_old;
  INGRESSI_fall = ~INGRESSI_state & INGRESSI_state_old;
  INGRESSI_state_old = INGRESSI_state;

  INGRESSI_2_rise = INGRESSI_2_state & ~INGRESSI_2_state_old;
  INGRESSI_2_fall = ~INGRESSI_2_state & INGRESSI_2_state_old;
  INGRESSI_2_state_old = INGRESSI_2_state;

  if (INGRESSI_2_rise || INGRESSI_2_fall){
  ////--Serial.print("IN ");
  ////--Serial.println(INGRESSI_2_state, BIN);
  }
  return 1;
}

bool INGRESSI_read(uint8_t pin) {
  if (pin < 32) {
    return (INGRESSI_state >> pin) & 0x00000001;
  } else {
    return (INGRESSI_2_state >> (pin - 32)) & 0x00000001;
  }
}
bool INGRESSI_rising(uint8_t pin) {
  if (pin < 32) {
    return (INGRESSI_rise >> pin) & 0x00000001;
  } else {
    return (INGRESSI_2_rise >> (pin - 32)) & 0x00000001;
  }
}
bool INGRESSI_falling(uint8_t pin) {
  if (pin < 32) {
    return (INGRESSI_fall >> pin) & 0x00000001;
  } else {
    return (INGRESSI_2_fall >> (pin - 32)) & 0x00000001;
  }
}


bool USCITE_write(uint8_t pin, bool st) {
  USCITE_state &= ~(0x00000001 << pin);
  USCITE_state |= ((0x00000001 * st) << pin);
  return 1;
}
bool USCITE_writeAll() {
  ////--Serial.print("OUT UNF ");
  ////--Serial.println(USCITE_state, BIN);
  USCITE_state &= USCITE_mask;
  digital_programmables.setAll(&USCITE_state);
  ////--Serial.print("OUT ");
  ////--Serial.println(USCITE_state, BIN);
  return 1;
}

void USCITE_setDir(uint8_t pin, bool st) {
  USCITE_mask &= ~(0x00000001 << pin);
  USCITE_mask |= ((0x00000001 * st) << pin);
  USCITE_state &= USCITE_mask;//~((0x00000001 * !st) << pin);

  //--Serial.print("OUT MASK ");
  //--Serial.println(USCITE_mask, BIN);
}
