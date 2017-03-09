#include "state_machine.h"


typedef enum {
  STATES_A = 0x0100,
  STATES_B,
  STATES_C
} ;

static void
_action_a (state_machine_t* this, char* reason)
{

}

static void
_action_b (state_machine_t* this, char* reason)
{
  
}

static void
_action_c (state_machine_t* this, char* reason)
{

}

void
SM_second_state_2_action (state_machine_t* this)
{
  switch (this->current_state_) {
    case BEGIN:
      _action_a (this, "action_a");
      break;
    case STATES_A:
      _action_c (this, "action_c");
      break;
    case STATES_B:
      break;
    case STATES_C:
      _action_b (this, "action_b");
      break;
    default:
    // TODO: ALARM - sth may wrong
      break;
  };
}

Bool
SM_second_check_conditions (state_machine_t* this)
{
  if (BEGIN == this->current_state_) {
    return SM_set_new_state (this, (unsigned int)STATES_A);
  }

  switch (this->current_state_) {
    case STATES_A:
        return SM_set_new_state (this, STATES_B);
    case STATES_B:
      return SM_set_new_state (this, STATES_C);
    case STATES_C:
      return SM_set_new_state (this, STATES_A);
    default:
      // TODO: Oops
      break;
  };
  return False;
}

