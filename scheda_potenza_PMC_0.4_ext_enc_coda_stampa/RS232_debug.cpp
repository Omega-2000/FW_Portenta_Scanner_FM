#include "RS232_debug.h"
#include <stdint.h>
#include <Arduino_MachineControl.h>

using namespace machinecontrol;



uint32_t RS232_last;
uint32_t RS232_send_interval = 500;

uint8_t RS232_cnt;


void RS232_begin() {
  // Enable the RS485/RS232 system
  comm_protocols.rs485Enable(true);
  // Enable the RS232 mode
  comm_protocols.rs485ModeRS232(true);

  // Specify baudrate for RS232 communication
  comm_protocols.rs485.begin(115200);
  // Start in receive mode
  comm_protocols.rs485.receive();

  ////--Serial.println("Initialization done!");
}


void RS232_check() {
  if (comm_protocols.rs485.available()) {
    char ch = comm_protocols.rs485.read();
    ////--Serial.write(ch);
  }

  if (millis() - RS232_last > RS232_send_interval) {
    RS232_last = millis();

    String msg = "alive\n";
    //msg += String(RS232_cnt++);
    //if (RS232_cnt > 9) RS232_cnt = 0;

    // Disable receive mode before transmission
    comm_protocols.rs485.noReceive();

    comm_protocols.rs485.beginTransmission();
    comm_protocols.rs485.print(msg);
    comm_protocols.rs485.endTransmission();

    // Re-enable receive mode after transmission
    comm_protocols.rs485.receive();

  }

}
