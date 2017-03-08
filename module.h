
#include "lib.h"
#include "state_machine.h"

typedef struct module_s {
  struct stpm_t*        next;
  char                  name[NAME_LEN];
  struct property_t*        ports;

  // TODO: implement this later
  //state_machine_t*         machines;

} MOD_T;
