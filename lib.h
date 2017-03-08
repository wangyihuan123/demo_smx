
#ifndef _STP_BASE_H__
#define _STP_BASE_H__

#include <stdlib.h>
#include <string.h>

// TODO: add debug info or log later

#ifdef __LINUX__
#  include <stddef.h>
#  include <stdio.h>
#  define Print printf
extern char* strdup (const char *s);
#else
// TODO: try other OS later
#  include <psos.h>
#endif

#ifndef Bool
#  define Bool        int
#  define False       0
#  define True        1
#endif

#ifndef TIMERS_NUMBER
#  define TIMERS_NUMBER        10
#endif

#ifndef NAME_LEN
#  define NAME_LEN             100
#endif

#define MY_NEW(PTR, TYPE, MSG)              \
  {                                             \
    PTR = (TYPE*) calloc (1, sizeof (TYPE));    \
    if (! PTR) {                                \
      Print ("FATAL: malloc code %s:%d\n", MSG, -1);             \
    }                                           \
  }

#define MY_FREE(PTR, MSG)              \
  {                                     \
    if (! PTR) {                        \
      Print ("FATAL: free code %s:%d\n", MSG, -2);       \
    }                                   \
    free (PTR);                         \
    PTR = NULL;                         \
  }


#endif

