
  
#define STATES { \
  CHOOSE(INIT),             \
  CHOOSE(INACTIVE),         \
  CHOOSE(TCACTIVE),         \
  CHOOSE(DETECTED),         \
  CHOOSE(NOTIFIED_TC),          \
  CHOOSE(PROPAGATING),          \
  CHOOSE(ACKNOWLEDGED),         \
  CHOOSE(NOTIFIED_TCN),         \
}

#define GET_STATE_NAME STP_topoch_get_state_name
#ifdef SLR_STP_CODE
#include "mstp_lib_choose.h"
#else
#include "choose.h"
#endif

#ifndef STRONGLY_SPEC_802_1W
/* 
 * In many kinds of hardware the function
 * STP_OUT_flush_lt is a) is very hard and b) cannot
 * delete learning emtries per port. The alternate
 * method may be used: we don't care operEdge flag here,
 * but clean learning table once for TopologyChange
 * for all ports, except the received port. I am ready to discuss :(
 * See below word STRONGLY_SPEC_802_1W
 */
#else
static Bool
action1 (STATE_MACH_T *this, char* reason) /* 17.19.9 */
{
  
}
#endif

static void
action2 (STATE_MACH_T* this, char* reason) /* 17.19.14 */
{
  
}


void
STP_topoch_enter_state (STATE_MACH_T* this)
{
  register PORT_T*      port = this->owner.port;

  //mst_statmch_enter_state_print_info(this);
  switch (this->State) {
    case BEGIN:
    case INIT:
      action1 (this, "action1");

      break;
    case STATES_A:
      action2 (this, "action2");
      port->rcvdTc =
      port->rcvdTcn =
      port->rcvdTcAck = False;
      break;
    case STATES_B:
      break;
    case STATES_C:
      action3 (this, "action3")
      break;
    default:
    // TODO: ALARM - sth wrong
      break;
  };
}

Bool
STP_topoch_check_conditions (STATE_MACH_T* this)
{
  register PORT_T*      port = this->owner.port;

  if (BEGIN == this->State) {
    return STP_hop_2_state (this, INIT);
  }

  switch (this->State) {
    case INIT:
      return STP_hop_2_state (this, STATES_A);
    case STATES_A:
      if (TRUE)
        return STP_hop_2_state (this, STATES_B);
      else
        return STP_hop_2_state (this, STATES_C);
      break;
    case STATES_B:
      
      break;
    case STATES_C:
      return STP_hop_2_state (this, TCACTIVE);
    case NOTIFIED_TC:
      return STP_hop_2_state (this, TCACTIVE);
    case PROPAGATING:
      return STP_hop_2_state (this, TCACTIVE);
    case ACKNOWLEDGED:
      return STP_hop_2_state (this, TCACTIVE);
    case NOTIFIED_TCN:
      return STP_hop_2_state (this, NOTIFIED_TC);
  };
  return False;
}

