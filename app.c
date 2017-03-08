
#define STATES {        \
  CHOOSE(DISABLED),         \
  CHOOSE(DETECTED),     \
  CHOOSE(DELEAYED),     \
  CHOOSE(RESOLVED),     \
}

#define GET_STATE_NAME STP_edge_get_state_name
#ifdef SLR_STP_CODE 
#include "mstp_lib_choose.h"
#else
#include "choose.h"
#endif

#define DEFAULT_LINK_DELAY  3

void
STP_edge_enter_state (state_machine_t *s)
{
  register PORT_T *port = s->owner.port;

  //mst_statmch_enter_state_print_info(s);
  switch (s->State) {
    case BEGIN:
      break;
    case DISABLED:
      port->operEdge = port->adminEdge;
#ifdef SLR_STP_CODE
      port->changed = 1;
#endif
      port->wasInitBpdu = False;
      port->lnkWhile = 0;
      port->portEnabled = False;
      break;
    case DETECTED:
      port->portEnabled = True;
      port->lnkWhile = port->LinkDelay;
      port->operEdge = False;
      port->checkEdge = False;
      break;
    case DELEAYED:
      break;
    case RESOLVED:
      if (! port->wasInitBpdu) {
          port->operEdge = port->adminEdge;
#ifdef SLR_STP_CODE
          port->changed = 1;
#endif
      }
      break;
  }
}

Bool
STP_edge_check_conditions (state_machine_t *s)
{
  register PORT_T *port = s->owner.port;

  switch (s->State) {
    case BEGIN:
      return SM_set_new_state (s, DISABLED);
    case DISABLED:
      if (port->adminEnable) {
        return SM_set_new_state (s, DETECTED);
      }
      break;
    case DETECTED:
      return SM_set_new_state (s, DELEAYED);
    case DELEAYED:
      if (port->wasInitBpdu) {
#ifdef STP_DBG
        if (s->debug)
            stp_trace ("port %s 'edge' resolved by BPDU", port->port_name);
#endif        
        return SM_set_new_state (s, RESOLVED);
      }

      if (! port->lnkWhile)  {
#ifdef STP_DBG
        if (s->debug)
          stp_trace ("port %s 'edge' resolved by timer", port->port_name);
#endif        
        return SM_set_new_state (s, RESOLVED);
      }

      if (! port->adminEnable) {
        return SM_set_new_state (s, DISABLED);
      }
      break;
    case RESOLVED:
      if (! port->adminEnable) {
        return SM_set_new_state (s, DISABLED);
      }
      if (port->checkEdge) {
      	return SM_set_new_state (s, DETECTED);
      }
      break;
  }
  return False;
}

