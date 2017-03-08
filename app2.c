

typedef enum {
  STATES_A = 0x1000,
  STATES_B,
  STATES_C
};

static void
_action1 (state_machine_t* this, char* reason)
{

}

static void
_action2 (state_machine_t* this, char* reason)
{
  
}

static void
_action3 (state_machine_t* this, char* reason)
{

}

void
DEMO_first_state_2_action (state_machine_t* this)
{
  register PORT_T*      port = this->owner.port;

  //mst_statmch_enter_state_print_info(this);
  switch (this->State) {
    case BEGIN:
    case INIT:
      _action1 (this, "action1");

      break;
    case STATES_A:
      _action2 (this, "action2");
      port->rcvdTc =
      port->rcvdTcn =
      port->rcvdTcAck = False;
      break;
    case STATES_B:
      break;
    case STATES_C:
      _action3 (this, "action3")
      break;
    default:
    // TODO: ALARM - sth may wrong
      break;
  };
}

Bool
DEMO_first_check_conditions (state_machine_t* this)
{
  register PORT_T*      port = this->owner.port;

  if (BEGIN == this->State) {
    return SM_set_new_state (this, INIT);
  }

  switch (this->State) {
    case INIT:
      return SM_set_new_state (this, STATES_A);
    case STATES_A:
      if (TRUE)
        return SM_set_new_state (this, STATES_B);
      else
        return SM_set_new_state (this, STATES_C);
    case STATES_B:
      break;
    case STATES_C:
      return SM_set_new_state (this, TCACTIVE);
    default:
      // TODO: Oops
      break;
  };
  return False;
}

