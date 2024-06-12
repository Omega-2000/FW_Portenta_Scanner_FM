
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  EMERGENZE   \\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              18/07/22


#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "emergenze.h"
#include "ingressi_uscite_prg.h"
#include "RS232_debug.h"


bool EMERG_arresto = 0;
bool EMERG_arresto_old = 0;
bool EMERG_arresto_ch = 0;

void EMERG_begin() {
  EMERG_check();
}

bool EMERG_check() {
  EMERG_arresto = !INGRESSI_read(EMERG_PIN);
  EMERG_arresto_ch = EMERG_arresto != EMERG_arresto_old;
  EMERG_arresto_old = EMERG_arresto;
  if(EMERG_arresto_ch){
    DEBUG("Emergenze change: ");
    DEBUG(EMERG_arresto);
    DEBUG("\n");
  }
}
