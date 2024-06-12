
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   ANTA.H   //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22


#ifndef ANTA_H
#define ANTA_H

#include <Arduino_MachineControl.h>
using namespace machinecontrol;
#include <stdint.h>

class Anta {
  private:
    uint8_t id;
    uint32_t inizio;
    uint32_t fine;
    uint8_t uso_inizio;
    uint8_t uso_fine;
    bool sollevabile;
    static uint8_t last_id;

  public:
    Anta(uint32_t in = 0, uint32_t fi = 0, uint8_t ui = 0, uint8_t uf = 0, uint8_t _id = NULL);

    void copy(Anta a);

    void setId(uint8_t in = 0);
    uint8_t getId();

    void setBegin(uint32_t in = 0);
    uint32_t getBegin();
    void setEnd(uint32_t fi = 0);
    uint32_t getEnd();

    void setLength(uint32_t le = 0);
    uint32_t getLength();

    void setBeginUse(uint8_t ui = 1);
    uint8_t getBeginUse();
    void setEndUse(uint8_t uf = 1);
    uint8_t getEndUse();

    void setLiftEnable(bool en = 1);
    bool getLiftEnabled();

    void print();
};

#endif
