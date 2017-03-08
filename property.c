



PROPERTY_T *
property_add (MOD_T* module, int property_id)
{
  PROPERTY_T*      this = module->ports;

  /* search for existed port */
  while (this) {
    if (this->property_id == property_id)
      return NULL;
    else
      this = this->next;
  }

  NEW_IN_LIST(this, PROPERTY_T, module->ports, "add property");

  sprintf(this->property_name, "p%02d", (int) property_id);
  this->uptime = 0;
  this->property_id = property_id;
  this->machines = NULL;
  this->owner = stpm;

  // append state machines
  APPEND_STATE_MACHINE_2_LIST(first);
  APPEND_STATE_MACHINE_2_LIST(second);

  // temperate design, try 2 timers first
  this->timers[0] = &this->Timer1;
  this->timers[1] = &this->Timer1;
  this->timers[2] = NULL;

  return this;
}


void
property_add (PROPERTY_T * this)
{
  MOD_T*                   stpm = this->owner;;
  register PORT_T*          prev;
  register PORT_T*          tmp;
  register STATE_MACH_T*    stater;
  register void*            pv;

  free (this->property_name);

  for (stater = this->machines; stater; ) {
    pv = (void*) stater->next;
    STP_state_mach_delete (stater);
    stater = (STATE_MACH_T*) pv;
  }

  prev = NULL;
  for (tmp = stpm->ports; tmp; tmp = tmp->next) {
    if (tmp->port_index == this->port_index) {
      if (prev) {
        prev->next = this->next;
      } else {
        stpm->ports = this->next;
      }
      MY_FREE(this, "stp instance");
      break;
    }
    prev = tmp;
  }
}
