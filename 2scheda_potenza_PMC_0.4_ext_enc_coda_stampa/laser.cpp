
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   LASER  //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22

#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "laser.h"


uint8_t LAS_mode = LAS_MODE_AUTO;
bool LAS_1_state;
bool LAS_2_state;


void LAS_set_mode(uint8_t mode, uint8_t n_las) {
  LAS_mode = mode;
  switch (mode) {
    case LAS_MODE_OFF:
      LAS_1_off(1);
      LAS_2_off(1);
      break;
    case LAS_MODE_ON:
      LAS_1_on(1);
      LAS_2_on(1);
      break;
    case LAS_MODE_AUTO:
      if (LAS_1_state) {
        LAS_1_on();
      } else {
        LAS_1_on();
      }
      if (LAS_2_state) {
        LAS_2_on();
      } else {
        LAS_2_off();
      }
      break;
    case LAS_MODE_AUTO_OFF:
      if (n_las == 0) {
        LAS_1_off();
      } else {
        LAS_2_off();
      }
      break;
  }
}


void LAS_1_on(bool man) {
  if (LAS_mode != LAS_MODE_OFF) {
    digital_outputs.set(LAS_1_PIN, 1);
  }
  if (!man) {
    LAS_1_state = 1;
  }
}

void LAS_2_on(bool man) {
  if (LAS_mode != LAS_MODE_OFF) {
    digital_outputs.set(LAS_2_PIN, 1);
  }
  if (!man) {
    LAS_2_state = 1;
  }
}


void LAS_1_off(bool man) {
  if (LAS_mode != LAS_MODE_ON) {
    digital_outputs.set(LAS_1_PIN, 0);
  }
  if (!man) {
    LAS_1_state = 0;
  }
}

void LAS_2_off(bool man) {
  if (LAS_mode != LAS_MODE_ON) {
    digital_outputs.set(LAS_2_PIN, 0);
  }
  if (!man) {
    LAS_2_state = 0;
  }
}
