
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//   SEGNALAZIONE QUEUE.H   //\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              07/09/22


#ifndef SEGNALAZIONE_QUEUE_H
#define SEGNALAZIONE_QUEUE_H

#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>

#define SGN_QUEUE_SIZE 20

class Sgn_obj {
  private:
    uint8_t id_scheda;
    uint16_t codice_errore;
    uint8_t registro_segnalazione;

  public:
    Sgn_obj(uint8_t is = 0, uint16_t ce = 0, uint8_t rs = 0);

    void copy(Sgn_obj o);

    void setId(uint8_t is = 0);
    uint8_t getId();
    void setErrorCode(uint16_t ce = 0);
    uint16_t getErrorCode();
    void setSignalRegister(uint8_t rs = 0);
    uint8_t getSignalRegister();

    void print();
};



class Sgn_queue {
  private:
    Sgn_obj sgn_queue[SGN_QUEUE_SIZE];
    uint8_t highest_priority;

  public:
    Sgn_queue();
    bool enqueue(Sgn_obj o);
    bool calcTop();
    Sgn_obj top();
    bool removeByError(uint8_t is, uint16_t ce);
};


extern Sgn_queue SGN_queue;

#endif
