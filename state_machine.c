/******************************************************************************
 * Using Object Oriented Thinking in C programming
 * Base class: State machine
 * Implement inheritance for sub class using MACRO
 *****************************************************************************/
#include "lib.h"
#include "state_machine.h"

state_machine_t *
SM_add_state_machine (void (*state_2_action) (state_machine_t*),
                       Bool (*check_condition) (state_machine_t*),
                       char *name)
{
  state_machine_t *this;

  MY_NEW( this,
          state_machine_t,
          name);

  //  state machine constructor
  this->check_condition = check_condition;
  this->state_2_action = state_2_action;

  this->current_state_ = BEGIN;
  this->name = (char*) strdup (name);
  this->change_state_ = False;

  return this;
}

void
SM_remove_state_machine (state_machine_t *this)
{
  MY_FREE(this);
}

Bool
SM_check_condition (state_machine_t* this)
{
  Bool bret;

  bret = (*(this->check_condition)) (this);
  if (bret) {
    this->change_state_ = True;
  }

  return bret;
}

Bool
SM_change_state (state_machine_t* this)
{
  register int number_of_loops;

  for (number_of_loops = 0; ; number_of_loops++) {
    if (! this->change_state_) return number_of_loops;
    (*(this->state_2_action)) (this);
    this->change_state_ = False;
    SM_check_condition (this);
  }

  return number_of_loops;
}

Bool
SM_set_new_state (state_machine_t* this, unsigned int new_state)
{
//  switch (this->state_machine_type) {
//#if 0
//    case 2:
//        printf("%s(%s): %s=>%s\n",this->name,*this->owner.stpm->name ? this->owner.stpm->name : "Glbl",
//            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
//        break;
//    case 1:
//          printf("%s(%s): %s=>%s\n",this->name,*this->owner.property->owner->name ? this->owner.property->owner->name : "Glbl",
//            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
//        break;
//#endif
//  }

  this->current_state_ = new_state;
  this->change_state_ = True;

  return True;
}

