



PROPERTY_T *
property_add (MOD_T* module, int property_id)
{
  PROPERTY_T*      this = module->properties;

  /* search for existed port */
  while (this) {
    if (this->property_id == property_id)
      return NULL;
    else
      this = this->next;
  }

  NEW_IN_LIST(this, PROPERTY_T, module->properties, "add property");

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
property_remove (PROPERTY_T * this)
{

  MOD_T*                    module = this->owner;
  register void*            pointer_reserve;
  register PROPERTY_T*          p_prev_property, p_tmp_property;
  register state_machine_t*    p_state_machine = this->machines;

  free (this->property_name);

  while (p_state_machine) {
    pointer_reserve = (void*) p_state_machine->next;
    SM_remove_state_machine (p_state_machine);
    p_state_machine = (state_machine_t*) pointer_reserve;
  }

  p_prev_property = NULL;
  p_tmp_property = module->properties;
  while (p_tmp_property) {
    // search the property in the list
    if (p_tmp_property->property_id == this->property_id) {
      if (p_prev_property) {
        p_prev_property->next = this->next;
      } else {
        module->properties = this->next;
      }
      MY_FREE(this);
      break;
    }
    p_prev_property = p_tmp_property;
    p_tmp_property = p_tmp_property->next;
  }

}
