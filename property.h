
#ifndef DEMO_SMX_PROPERTY_H
#define DEMO_SMX_PROPERTY_H

#include "state_machine.h"
#include "lib.h"

typedef unsigned int    PROPERTY_TIMER_T;

typedef struct property_s {
  struct property_s*     next;

  int               property_id;
  char*             property_name;
  unsigned long     uptime;


  struct MOD_T *    owner;

  state_machine_t*     s_state_machine;  //  static state machine for furture use
  state_machine_t*     machines; // list of machines

  PROPERTY_TIMER_T      Timer1;
  PROPERTY_TIMER_T      Timer2;
  PROPERTY_TIMER_T*     timers[TIMERS_NUMBER]; // list of timers

} PROPERTY_T;


PROPERTY_T *
property_add (MOD_T* module, int property_id);

#endif //DEMO_SMX_PROPERTY_H
