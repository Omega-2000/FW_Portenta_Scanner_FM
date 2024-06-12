#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>


#include "global_var_def.h"
#include "RS232_debug.h"

#include "telecamera_maniglie.h"

bool TELM_pulse_enabled = 0;
uint8_t TELM_pulse_count = 0;
bool TELM_out_state = 0;
uint32_t TELM_last_change = 0;

void TELM_start_pulse_train() {
  TELM_pulse_enabled = 1;
  TELM_pulse_count = 0;
  DEBUG("AVVIO TRIGGER TELECAMERA MANIGLIE");
}

void TELM_check() {
  if (TELM_pulse_enabled && (TELM_pulse_count < TELM_PULSE_COUNT)) {
    if ((millis() - TELM_last_change) > (TELM_MS_PERIOD / 2)) {
      TELM_out_state = !TELM_out_state;
      digital_outputs.set(TELM_TRIGGER_PIN, TELM_out_state);
      if (TELM_out_state) {
        DEBUG("----> TRIGGER TELECAMERA MANIGLIE, impulso n°: " + String(TELM_pulse_count)+"\n");
      } else {
        TELM_pulse_count++;
      }
      TELM_last_change = millis();
    }
  } else {
    TELM_pulse_enabled = 0;
  }
}
