/******************************************************************************
 *
 * class module
 * Each module contains multiple property,
 * each property can have a list of state machines.
 * The module is updated every second to fresh all the state machine in this
 * module.
 *
 *****************************************************************************/
//static int
//_init_machine (state_machine_t* this)
//{
//  this->State = BEGIN;
//  (*(this->state_2_action)) (this);
//  return 0;
//}

static int
_iterate_state_machines_check_condition (MODULE_T* this,
                   int (*iterator_callback) (state_machine_t*))
{
  register state_machine_t* p_state_machine;
  register PROPERTY_T*       property = this->ports;
  int                    iret = 0;

  while (property) {
    while (++counter < TIMERS_NUMBER) {
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
_iterate_state_machines_change_state (MODULE_T* this,
                         int (*iterator_callback) (state_machine_t*))
{
  register state_machine_t* p_state_machine;
  register PROPERTY_T*       property = this->ports;
  int                    mret = 0;

  while (property) {
    while (++counter < TIMERS_NUMBER) {
      mret += (*iterator_callback) (p_state_machine);
      p_state_machine = p_state_machine->next;  // loop the list of state_machine
    }
    property = property->next;  // loop the list of property
  }

  return mret;
}

int
MLD_update (MODULE_T* this) /* returns number of loops */
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
  } while (TRUE);

  return number_of_change;
}

void
MDL_one_second (MODULE_T* param)
{
  MODULE_T*           this = (MODULE_T*) param;
  register PROPERTY_T*  property = this->ports;
  register int      counter = 0;

  property = this->ports;
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
