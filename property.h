
#ifndef DEMO_SMX_PROPERTY_H
#define DEMO_SMX_PROPERTY_H

#include "state_machine.h"
#include "lib.h"

typedef unsigned int    PROPERTY_TIMER_T;

typedef struct property_t {
  struct property_t*     next;

  int               property_index;
  char*             property_name;

  /* per Port state machines */
  state_machine_t*     info;      /* 17.21 */


  state_machine_t*     machines; /* list of machines */

  struct stpm_t*    owner; /* Bridge, that this port belongs to */

  /* fixed port Timers */
  PROPERTY_TIMER_T      fdWhile;      /* 17.15.1 */


  PROPERTY_TIMER_T*     timers[TIMERS_NUMBER]; /*list of timers */

  unsigned long     uptime;       /* 14.8.2.1.3.a */

} PORT_T;


#endif //DEMO_SMX_PROPERTY_H
