
#ifndef DEMO_SMX_PROPERTY_H
#define DEMO_SMX_PROPERTY_H

#include "state_machine.h"
#include "lib.h"

typedef unsigned int    PROPERTY_TIMER_T;

typedef struct property_t {
  struct property_t*     next;

  int               property_index;
  char*             property_name;
  unsigned long     uptime;       /* 14.8.2.1.3.a */


  struct MODULE_T *    owner;

  state_machine_t*     s_state_machine;  //  static state machine for furture use
  state_machine_t*     machines; /* list of machines */

  PROPERTY_TIMER_T      Timer1;
  PROPERTY_TIMER_T      Timer2;
  PROPERTY_TIMER_T*     timers[TIMERS_NUMBER]; // list of timers



} PROPERTY_T;


#endif //DEMO_SMX_PROPERTY_H
