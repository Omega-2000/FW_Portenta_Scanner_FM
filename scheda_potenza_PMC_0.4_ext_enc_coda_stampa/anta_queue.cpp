
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\ ANTA QUEUE //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              15/07/22


#include "anta_queue.h"
#include "Anta.h"


Anta ANT_queue[ANT_queue_size];

uint8_t ANT_first = 0;
uint8_t ANT_last = 0;
uint8_t ANT_last_id = 1;

void ANT_enqueue(Anta a) {
  ANT_queue[ANT_last].copy(a);
  if (++ANT_last >= ANT_queue_size) ANT_last = 0;
}

void ANT_last_update(Anta a) {
  ANT_queue[(int16_t)ANT_last - 1 >= 0 ? ANT_last - 1 : ANT_queue_size - 1].copy(a);
}

Anta* ANT_last_view() {
  if (ANT_first == ANT_last)return NULL;
  return &ANT_queue[(int16_t)ANT_last - 1 >= 0 ? ANT_last - 1 : ANT_queue_size - 1];
}

Anta* ANT_first_view() {
  if (ANT_first == ANT_last)return NULL;
  return &ANT_queue[ANT_first];
}

Anta ANT_dequeue() {
  if (ANT_first == ANT_last)return NULL;
  Anta a = Anta(0, 0, 0, 0, 1);
  ANT_queue[ANT_first].copy(a);
  if (++ANT_first >= ANT_queue_size) ANT_first = 0;
  return a;
}

void ANT_flush(){
  Anta a = ANT_dequeue();
  uint8_t cnt = 0;
  while (&a != NULL){
    a = ANT_dequeue();
    if (cnt++ > ANT_queue_size){
      //--Serial.println("Qualche errore con flsh e dequeue");
      return;
    }
  }
}

void ANT_last_add_end(uint32_t end_pulses) {
  Anta *a = ANT_last_view();//Anta(0, 0, 0, 0, ANT_last_id);
  //a->print();
  a->setEnd(end_pulses);
  a->setEndUse(1);
}
