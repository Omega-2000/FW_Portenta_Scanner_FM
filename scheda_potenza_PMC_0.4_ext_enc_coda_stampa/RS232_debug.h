
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\  RS232_DEBUG.H //\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              26/07/23


#ifndef RS232_DEBUG_H
#define RS232_DEBUG_H

void RS232_begin();
void RS232_check();

#include <stdint.h>
#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#define DEBUG( msg ) comm_protocols.rs485.noReceive();comm_protocols.rs485.beginTransmission();comm_protocols.rs485.print(msg);comm_protocols.rs485.endTransmission();comm_protocols.rs485.receive();


#endif
