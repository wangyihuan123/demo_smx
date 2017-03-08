
#ifndef _STP_STATER_H__
#define _STP_STATER_H__

#define BEGIN  -1 /* distinct from any valid state */

typedef struct state_machine_s {
  struct state_machine_s* next;

  char          name[NAME_LEN];


//#ifdef STP_DBG
//  char          debug; /* 0- no dbg, 1 - port, 2 - stpm */
//  unsigned int  ignoreHop2State;
//#endif

  Bool          change_state_;
  unsigned int  current_state_;

  void          (* state_2_action) (struct state_machine_s * );
  Bool          (* check_condition) (struct state_machine_s * );
//  // char*         (* concreteGetStatName) (int);
//  union {
//    struct stpm_t* stpm;
//    struct property_t* port;
//    void         * owner;
//  } owner;
  char          state_machine_type; /* 0=unknown,1=property,2=module */

} state_machine_t;

#define LIST_STATE_MACHINE_APPEND(NAME)                              \
  {                                                               \
    state_machine_t* abstract;                                          \
                                                                  \
    abstr = DEMO_add_state_machine (DEMO_##NAME##_state_2_action,      \
                                  DEMO_##NAME##_check_conditions,  \
                                  DEMO_##NAME##_get_state_name,    \
                                  this,                           \
                                  #NAME);                         \
    abstr->next = this->machines;                                 \
    this->machines = abstr;                                       \
    this->NAME = abstr;                       \
  }

state_machine_t *
DEMO_add_state_machine (void (*state_2_action) (state_machine_t*),
                        Bool (*check_condition) (state_machine_t*),
    // char *(*concreteGetStatName) (int),
                        void *owner, char *name)
                     
void
STP_state_mach_delete (state_machine_t* this);

Bool
STP_check_condition (state_machine_t* this);

Bool
STP_change_state (state_machine_t* this);

Bool
STP_hop_2_state (state_machine_t* this, unsigned int new_state);

#endif /* _STP_STATER_H__ */

