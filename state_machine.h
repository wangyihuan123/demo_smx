
#ifndef DEMO_SMX_STATE_MACHINE_H
#define DEMO_SMX_STATE_MACHINE_H

#include "lib.h"

#define BEGIN  -1 /* distinct from any valid state */

typedef struct state_machine_s {
  struct state_machine_s* next;

  char          *name;

  Bool          change_state_;
  unsigned int  current_state_;

  void          (* state_2_action) (struct state_machine_s * );
  Bool          (* check_condition) (struct state_machine_s * );
  void          *owner;

} state_machine_t;

#define APPEND_STATE_MACHINE_2_LIST(NAME)                              \
  {                                                               \
    state_machine_t* base;                                          \
                                                                  \
    base = SM_add_state_machine (SM_##NAME##_state_2_action,      \
                                  SM_##NAME##_check_conditions,  \
                                  this,                           \
                                  #NAME);                           \
    base->next = this->machines;                                    \
    this->machines = base;                                          \
    this->owner = this;                                             \
    this->NAME = base;                       \
  }

state_machine_t *
SM_add_state_machine (void (*state_2_action) (state_machine_t*),
                      Bool (*check_condition) (state_machine_t*),
                      void *owner,
                      char *name);
void
SM_remove_state_machine (state_machine_t *this);

Bool
SM_check_condition (state_machine_t* this);

Bool
SM_change_state (state_machine_t* this);

Bool
SM_set_new_state (state_machine_t* this, unsigned int new_state);

#endif

