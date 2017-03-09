#include "state_machine.h"
#include "property.h"
#include "module.h"
/******************************************************************************
 *
 * class module
 * Each module contains multiple property,
 * each property can have a list of state machines.
 * The module is updated every second to fresh all the state machine in this
 * module.
 *
 *****************************************************************************/

static int
_iterate_state_machines_check_condition (MOD_T * this,
                   int (*iterator_callback) (state_machine_t*))
{
  register state_machine_t*   p_state_machine;
  register PROPERTY_T*        property = this->properties;
  int                         iret = 0;

  while (property) {
    while (p_state_machine) {
      iret = (*iterator_callback) (p_state_machine);
      if (iret)
        return iret;
      p_state_machine = p_state_machine->next;  // loop the list of state_machine
    }
    property = property->next;  // loop the list of property
  }

  return iret;
}


static int
_iterate_state_machines_change_state (MOD_T * this,
                         int (*iterator_callback) (state_machine_t*))
{
  register PROPERTY_T*        property = this->properties;
  register state_machine_t*   p_state_machine = property->machines;
  int                         mret = 0;

  while (property) {
    while (p_state_machine) {
      mret += (*iterator_callback) (p_state_machine);
      p_state_machine = p_state_machine->next;  // loop the list of state_machine
    }
    property = property->next;  // loop the list of property
  }

  return mret;
}

// TODO: not finished!!
MOD_T *
MLD_add (char* name) {

//  LIST_STATE_MACHINE_APPEND
}

int
MLD_update (MOD_T* this) /* returns number of loops */
{
  register Bool     need_2_change_state;
  register int      number_of_change = 0;

  need_2_change_state = False;

  do {
    need_2_change_state =
        _iterate_state_machines_check_condition (this,
                                                 SM_check_condition);
    if (! need_2_change_state)
      break;

    number_of_change = _iterate_state_machines_change_state (this,
                                             SM_change_state);
  } while (1);

  return number_of_change;
}

// one second time out trigger by event callback
void
MDL_time_out (MOD_T* event_handle_param)
{
  MOD_T*           this = (MOD_T*) event_handle_param;
  register PROPERTY_T*  property = this->properties;
  register int      counter = 0;

  property = this->properties;
  while (property) {
    while (++counter < TIMERS_NUMBER) {
      if (*(property->timers[counter]) > 0) {
        (*property->timers[counter])--;
      }
    }
    property->uptime++;

    // loop the list of property
    property = property->next;
  }

  MDL_update (this);
}

// TODO: not finished!!
void
MLD_remove (MOD_T* this) {

//  while () {
//    // loop and remove all the state machine
//  }

//  while () {
//    // remove all the properties
//  }


}