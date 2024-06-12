
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   MOTORE   //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;


#include "global_var_def.h"
#include "motore.h"
#include "emergenze.h"

uint8_t MOT_state;
uint8_t MOT_fw_speed;
uint8_t MOT_bw_speed;


void MOT_begin() {
  //analog_out.period_ms(CHANNEL, PERIOD_MILLISECONDS);
  analog_out.period_ms(MOT_ANALOG_PIN, 4);
  //--Serial.println("Motor begin");
}

void MOT_start(double sp, bool dir) {
  if (sp < 0) sp = 0;
  if (sp > 10.4) sp = 10.4;

  if (!EMERG_arresto) {
    digital_outputs.set(MOT_START_PIN, 1);
    digital_outputs.set(MOT_DIRECTION_PIN, dir);
    analog_out.write(MOT_ANALOG_PIN, sp);
    MOT_state = dir + 1;
  }
  if (dir) {
    MOT_bw_speed = sp;
  } else {
    MOT_fw_speed = sp;
  }
  //--Serial.print("Motor start ");
  //--Serial.print(dir);
  //--Serial.print(" ");
  //--Serial.println(sp);
}

void MOT_stop() {
  if (MOT_state) {
    digital_outputs.set(MOT_START_PIN, 0);
    digital_outputs.set(MOT_DIRECTION_PIN, 0);
    analog_out.write(MOT_ANALOG_PIN, 0);
    MOT_state = 0;
    //--Serial.println("Motor stop");
  }
}
