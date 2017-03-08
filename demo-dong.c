


#include "base.h"
#include "statmch.h"
#include "mstp_lib_statmch.h"
#include "stpm.h"


STATE_MACH_T *
STP_state_mach_create (void (*concreteEnterState) (STATE_MACH_T*),
                       Bool (*concreteCheckCondition) (STATE_MACH_T*),
                       char *(*concreteGetStatName) (int),
                       void *owner, char *name)
{
  STATE_MACH_T *this;

  STP_MALLOC(this, STATE_MACH_T, "state machine");
 
  this->State = BEGIN;
  this->name = (char*) strdup (name);
  this->changeState = False;
  this->concreteEnterState = concreteEnterState;
  this->concreteCheckCondition = concreteCheckCondition;
  this->concreteGetStatName = concreteGetStatName;
  this->owner.owner = owner;

  return this;
}
                              
void
STP_state_mach_delete (STATE_MACH_T *this)
{
  STP_FREE(this, "state machine");
}

Bool
STP_check_condition (STATE_MACH_T* this)
{
  Bool bret;



  bret = (*(this->concreteCheckCondition)) (this);
  if (bret) {
    this->changeState = True;
  }
  
  return bret;
}
        
Bool
STP_change_state (STATE_MACH_T* this)
{
  register int number_of_loops;

  for (number_of_loops = 0; ; number_of_loops++) {
    if (! this->changeState) return number_of_loops;
    (*(this->concreteEnterState)) (this);
    this->changeState = False;
    STP_check_condition (this);
  }

  return number_of_loops;
}

Bool
STP_hop_2_state (STATE_MACH_T* this, unsigned int new_state)
{

  switch (this->state_mach_type) {
#if 0
    case 2:
      if (mst_print_debug_msg_ok(0)) {
        printf("%s(%s): %s=>%s\n",this->name,*this->owner.stpm->name ? this->owner.stpm->name : "Glbl",
            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
        }
        break;
    case 1:
       if (mst_print_debug_msg_ok(this->owner.port)) {
          printf("%s(%s): %s=>%s\n",this->name,*this->owner.port->owner->name ? this->owner.port->owner->name : "Glbl",
            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
        }
        break;
   #endif
  }

  this->State = new_state;
  this->changeState = True;
  return True;
}


#if 0

void mst_statmch_enter_state_print_info(STATE_MACH_T *this)
{

  switch (this->state_mach_type) {
  case 2:
    if (mst_print_debug_msg_ok(0)) {
      printf("%s(%s):[Enter]=>%s\n",this->name,*this->owner.stpm->name?this->owner.stpm->name :"Glbl",(*(this->concreteGetStatName))(this->State));
    }
    break;
  case 1:
    if (mst_print_debug_msg_ok(this->owner.port)) {
      printf("%s(%s):[Enter]=>%s\n",this->name,*this->owner.port->owner->name ? this->owner.port->owner->name : "Glbl",(*(this->concreteGetStatName))(this->State));
      }
      break;
  }
   
}

#endif 

static int
_stp_stpm_init_machine (STATE_MACH_T* this)
{
  this->State = BEGIN;
  (*(this->concreteEnterState)) (this);
  return 0;
}

static int
_stp_stpm_iterate_machines (STPM_T* this,
                           int (*iter_callb) (STATE_MACH_T*),
                           Bool exit_on_non_zero_ret)
{
  register STATE_MACH_T* stater;
  register PORT_T*       port;
  int                    iret, mret = 0;


  /* state machines per bridge */
  for (stater = this->machines; stater; stater = stater->next) {
    iret = (*iter_callb) (stater);
    if (exit_on_non_zero_ret && iret)
      return iret;
    else
      mret += iret;
  }

  /* state machines per port */
  for (port = this->ports; port; port = port->next) {
    for (stater = port->machines; stater; stater = stater->next) {
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
STP_stpm_update (STPM_T* this) /* returns number of loops */
{
  register Bool     need_state_change;
  register int      number_of_loops = 0;

  need_state_change = False; 
  
  for (;;) {/* loop until not need changes */
    need_state_change = _stp_stpm_iterate_machines (this,
                                                   STP_check_condition,
                                                   True);

    if (! need_state_change) return number_of_loops;

    number_of_loops++;
    /* here we know, that at least one stater must be
       updated (it has changed state) */
    number_of_loops += _stp_stpm_iterate_machines (this,
                                                  STP_change_state,
                                                  False);

  }

  return number_of_loops;
}

void
STP_stpm_one_second (STPM_T* param)
{
  STPM_T*           this = (STPM_T*) param;
  register PORT_T*  port;
  register int      iii;

  if (STP_ENABLED != this->admin_state) return;

  for (port = this->ports; port; port = port->next) {
    for (iii = 0; iii < TIMERS_NUMBER; iii++) {
      if (*(port->timers[iii]) > 0) {
        (*port->timers[iii])--;
      }
    }    
    port->uptime++;
  }

  STP_stpm_update (this);
  this->Topo_Change = _check_topoch (this);
  if (this->Topo_Change) {
    this->Topo_Change_Count++;
    this->timeSince_Topo_Change = 0;
  } else {
    this->Topo_Change_Count = 0;
    this->timeSince_Topo_Change++;
  }
}
