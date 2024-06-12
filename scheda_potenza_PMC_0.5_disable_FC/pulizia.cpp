//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//   SEGNALAZIONE   //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              27/07/22

#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>

#include "pulizia.h"

#include "global_var_def.h"
#include "can_bus.h"
#include "ingressi_uscite_prg.h"
#include "motore.h"

void PUL_begin() {
  USCITE_setDir(PUL_PIN, 1);
}

void PUL_start(){
  USCITE_write(PUL_PIN, 1);
  MAC_start(MOT_MAX_FW_SPEED / 3.0);
  CAN_send_stato_pulizia(1);
}

void PUL_stop(){
  USCITE_write(PUL_PIN, 0);
  MAC_stop();
  CAN_send_stato_pulizia(0);
}
