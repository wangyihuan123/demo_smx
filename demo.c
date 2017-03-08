#include "lib.h"
#include "state_machine.h"

state_machine_t *
DEMO_add_state_machine (void (*state_2_action) (state_machine_t*),
                       Bool (*check_condition) (state_machine_t*),
                       // char *(*concreteGetStatName) (int),
                       void *owner, char *name)
{
  state_machine_t *this;

  MY_NEW( this, 
          state_machine_t,
          name);
 
  this->current_state_ = BEGIN;
  this->name = (char*) strdup (name);
  this->change_state_ = False;
  this->state_2_action = state_2_action;
  this->check_condition = check_condition;

  return this;
}

void
DEMO_remove_state_machine (state_machine_t *this)
{
  MY_FREE(this);
}

Bool
DEMO_check_condition (state_machine_t* this)
{
  Bool bret;
  
  bret = (*(this->check_condition)) (this);
  if (bret) {
    this->change_state_ = True;
  }
  
  return bret;
}
        
Bool
DEMO_change_state (state_machine_t* this)
{
  register int number_of_loops;

  for (number_of_loops = 0; ; number_of_loops++) {
    if (! this->change_state_) return number_of_loops;
    (*(this->state_2_action)) (this);
    this->change_state_ = False;
    DEMO_check_condition (this);
  }

  return number_of_loops;
}

Bool
DEMO_set_state (state_machine_t* this, unsigned int new_state)
{

  switch (this->state_machine_type) {
#if 0
    case 2:
      if (mst_print_debug_msg_ok(0)) {
        printf("%s(%s): %s=>%s\n",this->name,*this->owner.stpm->name ? this->owner.stpm->name : "Glbl",
            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
        }
        break;
    case 1:
       if (mst_print_debug_msg_ok(this->owner.property)) {
          printf("%s(%s): %s=>%s\n",this->name,*this->owner.property->owner->name ? this->owner.property->owner->name : "Glbl",
            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
        }
        break;
   #endif
  }

  this->State = new_state;
  this->change_state_ = True;
  return True;
}

