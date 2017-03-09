#ifndef DEMO_SMX_MODULE_H
#define DEMO_SMX_MODULE_H

#include "lib.h"

typedef struct module_s {
  struct module_s*        next;
  char                    name[NAME_LEN];
  struct property_s*      properties;

  // TODO: implement this later
  //state_machine_t*         machines;
} MOD_T;

#endif