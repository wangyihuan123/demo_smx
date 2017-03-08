#include "lib.h"


typedef struct module_s {
  struct module_s*        next;
  char                    name[NAME_LEN];
  struct property_t*      properties;

  // TODO: implement this later
  //state_machine_t*         machines;
} MOD_T;
