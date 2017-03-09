#include "state_machine.h"


typedef enum {
  STATES_1 = 0x1000,
  STATES_2,
  STATES_3
} ;


static void
_action_1 (state_machine_t* this, char* reason)
{

}

static void
_action_2 (state_machine_t* this, char* reason)
{

}

static void
_action_3 (state_machine_t* this, char* reason)
{

}

void
SM_first_state_2_action (state_machine_t* this)
{
  switch (this->current_state_) {
    case BEGIN:
      _action_1 (this, "action_1");
      break;
    case STATES_1:
      _action_2 (this, "action_3");
      break;
    case STATES_2:
      break;
    case STATES_3:
      _action_3 (this, "action_2");
      break;
    default:
      // TODO: ALARM - sth may wrong
      break;
  };
}

Bool
SM_first_check_conditions (state_machine_t* this)
{
  if (BEGIN == this->current_state_) {
    return SM_set_new_state (this, STATES_1);
  }

  switch (this->current_state_) {
    case STATES_1:
      return SM_set_new_state (this, STATES_2);
    case STATES_2:
      return SM_set_new_state (this, STATES_3);
    case STATES_3:
      return SM_set_new_state (this, STATES_1);
    default:
      // TODO: Oops
      break;
  };
  return False;
}

