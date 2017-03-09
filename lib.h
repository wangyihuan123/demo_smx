
#ifndef DEMO_SMX_LIB_H
#define DEMO_SMX_LIB_H

#include <stdlib.h>
#include <string.h>

// TODO: add debug info or log later
#define __LINUX__
#ifdef __LINUX__
#  include <stddef.h>
#  include <stdio.h>
#  define Print printf
extern char* strdup (const char *s);
#else
// TODO: try other OS later
#endif

#ifndef Bool
#  define Bool        int
#  define False       0
#  define True        1
#endif

#ifndef TIMERS_NUMBER
#  define TIMERS_NUMBER        3
#endif

#ifndef NAME_LEN
#  define NAME_LEN             100
#endif

#define MY_NEW(PTR, TYPE, MSG)                  \
  {                                             \
    PTR = (TYPE*) calloc (1, sizeof (TYPE));    \
    if (! PTR) {                                \
      Print ("FATAL: malloc %s:%d\n", MSG, -1,  \
              __LINE__, __FUNCTION__);          \
    }                                           \
  }

#define MY_FREE(PTR)                    \
  {                                     \
    if (! PTR) {                        \
      Print ("FATAL: free %s:%d\n", -2, \
          __LINE__, __FUNCTION__);      \
    }                                   \
    free (PTR);                         \
    PTR = NULL;                         \
  }


#define NEW_IN_LIST(WHAT, TYPE, LIST, MSG)  \
  {                                             \
    MY_NEW(WHAT, TYPE, MSG);                \
    WHAT->next = LIST;                          \
    LIST = WHAT;                                \
  }


#endif

