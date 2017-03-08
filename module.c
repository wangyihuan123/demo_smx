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
_iterate_machines (MODULE_T* this,
                   int (*iter_callb) (state_machine_t*),
                   Bool exit_on_non_zero_ret)
{
  register state_machine_t* stater;
  register PROPERTY_T*       property;
  int                    iret, mret = 0;

  /* state machines per property */
  for (property = this->ports; property; property = property->next) {
    for (stater = property->machines; stater; stater = stater->next) {
      iret = (*iter_callb) (stater);
      if (exit_on_non_zero_ret && iret)
        return iret;
      else
        mret += iret;
    }
  }

  return mret;
}

int
MLD_update (MODULE_T* this) /* returns number of loops */
{
  register Bool     need_state_change;
  register int      number_of_loops = 0;

  need_state_change = False;

  for (;;) {/* loop until not need changes */
    need_state_change = _iterate_machines (this,
                                           DEMO_check_condition,
                                           True);

    if (! need_state_change) return number_of_loops;

    number_of_loops++;
    /* here we know, that at least one stater must be
       updated (it has changed state) */
    number_of_loops += _iterate_machines (this,
                                          DEMO_change_state,
                                          False);
  }

  return number_of_loops;
}

void
MDL_one_second (MODULE_T* param)
{
  MODULE_T*           this = (MODULE_T*) param;
  register PROPERTY_T*  property;
  register int      iii;

//  if (DEMO_ENABLED != this->admin_state) return;

  for (property = this->ports; property; property = property->next) {
    for (iii = 0; iii < TIMERS_NUMBER; iii++) {
      if (*(property->timers[iii]) > 0) {
        (*property->timers[iii])--;
      }
    }
    property->uptime++;
  }

  DEMO_update (this);

}
