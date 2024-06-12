
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\   ENCODER  //\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//  Ultima modifica                              14/07/22

#include <Arduino_MachineControl.h>
using namespace machinecontrol;

#include "global_var_def.h"
#include "encoder.h"
#include "Anta.h"
#include "can_bus.h"
#include "anta_queue.h"
#include "stampa.h"
#include "fotocellule.h"
#include "sollevatore.h"
#include "laser.h"
#include "segnalazione.h"
#include "RS232_debug.h"
#include "telecamera_maniglie.h"

//extern
uint32_t ENC_last_print;
uint32_t ENC_last_int_rise_pulses,
         ENC_last_int_fall_pulses;

double ENC_mm_stp = ENC_MM_STP;
uint32_t ENC_points_distances[ENC_POINTS] = {
  ENC_POINT_CAMERA_TOP,
  ENC_POINT_CAMERA_LAT,
  ENC_POINT_PRE_PRINT,
  ENC_POINT_PRINT,
  ENC_POINT_END_GUIDE + ENC_POINTS_MARGIN / 2,
  ENC_POINT_END_CONV + ENC_POINTS_MARGIN / 2,
  ENC_POINT_END_CONV
};
/*void (*ENC_points_begin_functions[3])() = {
  ENC_point_1_begin,
  ENC_point_2_begin,
  ENC_point_3_begin
  };
  void (*ENC_points_end_functions[3])() = {
  ENC_point_1_end,
  ENC_point_2_end,
  ENC_point_3_end
  };*/

// local
uint32_t ENC_queue_begin_pulses;
uint32_t ENC_queue_end_pulses;
bool ENC_done;
uint32_t ENC_old;
double ENC_speed;

/*
  inline double ENC_getDistance() {
  // aggiungere conversione
  return encoders[0].getPulses() * ENC_mm_stp;
  }
  inline uint32_t ENC_getDistance_pulses() {
  return encoders[0].getPulses();
  }*/


bool print_debug = 0;

void FC_int_rise_routine() {
  ENC_last_int_rise_pulses = encoders[0].getPulses();
  print_debug = 1;
}
void FC_int_fall_routine() {
  ENC_last_int_fall_pulses = encoders[0].getPulses();
  print_debug = 1;
}

void ENC_print() {
  uint32_t pulses_ = encoders[0].getPulses();
  ENC_speed = ENC_convert_to_mm(pulses_ - ENC_old) / ((millis() - ENC_last_print) / 1000.0);
  ENC_old = pulses_;
  /*//--Serial.print("Encoder 0 State: ");
    //--Serial.println(encoders[0].getCurrentState(), BIN);
    //--Serial.print("Encoder 0 Pulses: ");
    //--Serial.println(pulses_);
    //--Serial.print("Encoder 0 Revolutions: ");
    //--Serial.println(encoders[0].getRevolutions());*/
  char st[50];
  sprintf(st, " - Speed: %d.%d\n", uint32_t(ENC_speed), uint32_t(ENC_speed * 1000) % 1000);
  DEBUG(st);
  //--Serial.print(ENC_speed, 3);
  //--Serial.println();
}

inline void ENC_reset() {
  encoders[0].reset();
}

void ENC_enqueue_begin() {
  uint32_t tmp = ENC_getDistance_pulses();//ENC_last_int_rise_pulses;//

  char st[50];

  if (!ENC_done && (tmp - ENC_queue_end_pulses > ENC_convert_to_pulses(50.0) )) {
    ENC_queue_begin_pulses = tmp;
    ENC_done = 1;

    if (++ANT_last_id == 0) ANT_last_id = 1;
    Anta a = Anta(ENC_queue_begin_pulses, 0, 1, 0, ANT_last_id);
    ANT_enqueue(a);

    CAN_send_posizione_anta(ANT_last_id, 0, 0);
    //--Serial.print("");
    //--Serial.println(tmp);
    sprintf(st, "Begin enqueued: %d\n", tmp);
    DEBUG(st);
  } else {
    //--Serial.print("Begin NOT enqueued: ");
    //--Serial.println(tmp);
    sprintf(st, "Begin NOT enqueued: %d\n", tmp);
    DEBUG(st);
  }
}

void ENC_enqueue_end() {

  //Anta *a = ANT_last_view();

  //uint32_t tmp =  a->getBegin() + lunghezza_step; ////ENC_last_int_fall_pulses; //
  uint32_t tmp = ENC_getDistance_pulses();
  /*if (!(tmp - tmp_read < ENC_convert_to_pulses(50.0)) ||
      !(tmp_read - tmp < ENC_convert_to_pulses(50.0))) {
    tmp = tmp_read;


    char st[50];
    sprintf(st, "Error: %d\n", int16_t(tmp - tmp_read));
    DEBUG(st);
    }*/
  ////--Serial.println("enc enqueue end");
  if (ENC_done && (tmp - ENC_queue_begin_pulses > ENC_convert_to_pulses(50.0) )) {
    //--Serial.println("enc enqueue end abilitato");
    ENC_done = 0;
    ENC_queue_end_pulses = tmp;


    ////--Serial.println("Anta last add end");
    Anta *a = ANT_last_view();
    if (a != NULL) {
      ANT_last_add_end(ENC_queue_end_pulses);
    }

    uint32_t len = ENC_queue_end_pulses - ENC_queue_begin_pulses;
    double len_mm = ENC_convert_to_mm(len) + ENC_MM_OFFSET;
    len_mm *= 10;
    if (len_mm - (uint32_t)len_mm > 0.5) {
      len_mm += 1.0;
    }

    CAN_richiedi_step_pezzo(ANT_last_id, len);
    last_lunghezza_richiesta = millis();
    last_id_richiesto = ANT_last_id;
    //CAN_send_lunghezza_anta(ANT_last_id, len_mm, len);
    CAN_send_posizione_anta(ANT_last_id, 1, 0);

    if (SOL_isLiftable(len_mm)) {
      Anta *a = ANT_last_view();
      a->setLiftEnable();
      ////--Serial.println("Anta sollevabile");
    } else {
      ////--Serial.println("Anta NON sollevabile");
    }

    ////--Serial.print("End enqueued: ");
    ////--Serial.println(tmp);
  } else {
    //--Serial.print("Begin NOT enqueued: ");
    //--Serial.println(tmp);
  }
}

void ENC_send_misura_combinata(uint8_t a_id, uint32_t l_stp) {
  if (a_id == last_id_richiesto) {
    last_lunghezza_richiesta = 0;
    last_id_richiesto = 0;

    char st[50];

    uint32_t len = 0;
    Anta *a = ANT_last_view();
    if (a != NULL) {
      len = a->getLength();
    }

    if (len - l_stp < ENC_convert_to_pulses(1) || l_stp - len < ENC_convert_to_pulses(1)) {
      len = l_stp;

      sprintf(st, "misura simile, media: %d\n", a_id);
      DEBUG(st);

    } else if (len - l_stp < ENC_convert_to_pulses(20) || l_stp - len < ENC_convert_to_pulses(10)) {
      len = l_stp;

      sprintf(st, "misura diversa, encoder: %d\n", a_id);
      DEBUG(st);
    }// else resta il mio


    double len_mm = ENC_convert_to_mm(len) + ENC_MM_OFFSET;
    len_mm *= 10;
    if (len_mm - (uint32_t)len_mm > 0.5) {
      len_mm += 1.0;
    }

    CAN_send_lunghezza_anta(ANT_last_id, len_mm, len);

    sprintf(st, "misura inviata: %d\n", a_id);
    DEBUG(st);
  }
}

void ENC_check() {
  //  if (print_debug) {
  //    Serial.print("interrupt fotocellula: r ");
  //    Serial.print(ENC_last_int_rise_pulses);
  //    Serial.print(", f ");
  //    Serial.println(ENC_last_int_fall_pulses);
  //    print_debug = 0;
  //  }

  // controllo ricezione lunghezza
  if ((last_lunghezza_richiesta > 0) && (millis() - last_lunghezza_richiesta >= 100)) {
    last_lunghezza_richiesta = 0;
    last_id_richiesto = 0;

    char st[50];

    uint32_t len = 0;
    Anta *a = ANT_last_view();
    if (a != NULL) {
      len = a->getLength();
    }

    double len_mm = ENC_convert_to_mm(len) + ENC_MM_OFFSET;
    len_mm *= 10;
    if (len_mm - (uint32_t)len_mm > 0.5) {
      len_mm += 1.0;
    }

    CAN_send_lunghezza_anta(ANT_last_id, len_mm, len);

    sprintf(st, "misura non ricevuta: %d\n", ANT_last_id);
    DEBUG(st);
  }

  uint32_t new_distances[ENC_POINTS];
  uint32_t distance = ENC_getDistance_pulses();
  for (uint8_t i = 0; i < ENC_POINTS; i++) {
    new_distances[i] = distance - ENC_convert_to_pulses(ENC_points_distances[i]);
  }

  for (uint8_t i = 0; i < ANT_queue_size; i++) { // ciclo nella coda  di ante
    uint8_t cur_anta = (i + ANT_first) % ANT_queue_size; // partire da più vecchia a ultima
    uint8_t beg_use = ANT_queue[cur_anta].getBeginUse();
    uint8_t end_use = ANT_queue[cur_anta].getEndUse();
    if (beg_use && (beg_use <= ENC_POINTS)) { // se il punto è in uso e maggiore di zero
      if ( new_distances[beg_use - 1] >=
           (ANT_queue[cur_anta].getBegin() + (beg_use == 7 ? SOL_getOverhang(ANT_queue[cur_anta].getLength()) : 0)) ) { // verifico se ho superato il punto

        if (beg_use < 5) { // punti senza fotocellule
          CAN_send_posizione_anta(ANT_queue[cur_anta].getId(), 0, beg_use, ENC_speed); // invio messaggio canbus
          if (beg_use == 1) { // camera top
            LAS_1_on();
          } else if (beg_use == 2) { // camera lato
            LAS_2_on();
            TELM_start_pulse_train();
          } else if (beg_use == 3) { // pre stampante
          }
        } else if (beg_use == 5) { // punti con fotocellule -> fine guida
          if ((FC_last_dist_fc1_rise >= distance - ENC_convert_to_pulses(ENC_POINTS_MARGIN)) && (FC_last_dist_fc1_rise <= distance)) { // se l'evento è nelle tolleranze pezzo ok
            // tengo traccia del pezzo (non modifico nulla)
            DEBUG("Punto fine guida: inizio pezzo ok!\n");
          } else {
            DEBUG("Punto fine guida: inizio pezzo NON ok!\n");
            ANT_queue[cur_anta].setLiftEnable(0); // disabilito il ribaltamento se il pezzo è spostato
          }
        } else if (beg_use == 6) { // fine rullo fotocellula + margin // verrà eseguito dopo la pressione del pedale
          if ((FC_last_dist_fc2_rise >= distance - ENC_convert_to_pulses(ENC_POINTS_MARGIN)) && (FC_last_dist_fc2_rise <= distance)) { // se l'evento è nelle tolleranze pezzo ok
            // tengo traccia del pezzo (non modifico nulla)
            DEBUG("Punto fine guida: inizio pezzo ok!\n");
          } else {
            DEBUG("Punto fine guida: inizio pezzo NON ok!\n");
            ANT_queue[cur_anta].setLiftEnable(0); // disabilito il rbaltamento se il pezzo è spostato
            FC_2_override = 0; // disabilito override e fermo
          }
        } else if (beg_use == 7) { // centro pezzo dopo partenza da pedale
          if (ANT_queue[cur_anta].getLiftEnabled() && FC_2_override) { // se anta sollevabile e override abilitato
            FC_2_override = 0; // disabilito override per fermarlo
            MOT_stop(); // fermo motore
            SOL_enabled = 1; // abilito sollevatore
            /*if (new_distances[beg_use - 1] >= ANT_queue[cur_anta].getBegin() + ANT_queue[cur_anta].getLength() / 2) {
              beg_use--;
              } else {
              MOT_stop(); // fermo motore se fotocellula fine rullo impegnata
              SGN_intervento(SGN_INT_FC3);
              }*/
          } else {
            /*
              MOT_stop(); // fermo motore se fotocellula fine rullo impegnata
              SGN_intervento(SGN_INT_FC3);*/
          }
        }

        ANT_queue[cur_anta].setBeginUse(beg_use + 1); // aggiono punto
        ANT_queue[cur_anta].print();
      }
    }

    if (end_use) { // se il punto è in uso


      if ( new_distances[end_use - 1] >= ANT_queue[cur_anta].getEnd() ) { // verifico se ho superato il punto
        if (end_use == 4) { // print
          //stampo
          STM_print(ANT_queue[cur_anta].getId());
        } else if (end_use < 5) { // punti senza fotocellule
          bool err_pre_stampa = 0;
          if (end_use == 3) { // pre stampa
            if (!STM_check_pre_stampa()) {
              CAN_send_errore_stampa(0x02);
              err_pre_stampa = 1;
            }
          }
          if (!err_pre_stampa) CAN_send_posizione_anta(ANT_queue[cur_anta].getId(), 1, end_use); // invio messaggio canbus
        } else  if (end_use == 5) { // punti con fotocellule -> fine guida
          if ((FC_last_dist_fc1_fall >= distance - ENC_POINTS_MARGIN) && (FC_last_dist_fc1_fall <= distance)) { // se l'evento è nelle tolleranze pezzo ok
            // tengo traccia del pezzo (non modifico nulla)
            DEBUG("Punto fine guida: fine pezzo ok!\n");
          } else {
            DEBUG("Punto fine guida: fine pezzo NON ok!\n");
            ANT_queue[cur_anta].setLiftEnable(0); // disabilito il rbaltamento se il pezzo è spostato
          }
        }
        // else  if (end_use == 5) { // punti con fotocellule -> fine rullo
        // non fa nulla perché il pezzo non deve cadere quindi ciao
        //}
        ANT_queue[cur_anta].setEndUse(end_use + 1); // aggiorno punto
      }


      if (end_use >= 6) { // se superata fine rullo
        //--Serial.println(" --> dequeue");
        ANT_queue[cur_anta].print();
        ANT_queue[cur_anta].setEndUse(0); // aggiorno punto
        ANT_dequeue();
      }


    }
    if (cur_anta == ANT_last) break;
  }
}






/*
  void ENC_point_1_begin(){
  // send can
  }

  void ENC_point_2_begin(){
  // send can
  }

  void ENC_point_3_begin(){
  //stampa
  }

  void ENC_point_1_end(){
  // send can
  }

  void ENC_point_2_end(){
  // send can
  }

  void ENC_point_3_end(){
  // nop
  }
*/
