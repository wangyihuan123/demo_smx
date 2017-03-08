

typedef enum {
  STATES_1 = 0x1000,
  STATES_2,
  STATES_3
} STATE_FIRST_e;


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
DEMO_first_state_2_action (state_machine_t* this)
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
DEMO_first_check_conditions (state_machine_t* this)
{
  if (BEGIN == this->State) {
    return SM_set_new_state (this, STATES_1);
  }

  switch (this->State) {
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

