
typedef struct stpm_t {
  struct stpm_t*        next;

  struct port_t*        ports;

  /* The only "per bridge" state machine */
  state_machine_t*         rolesel;   /* the Port Role Selection State machione: 17.22 */
  state_machine_t*         machines;

  /* variables */
  PROTOCOL_VERSION_T    ForceVersion;   /* 17.12, 17.16.1 */
  /* mst specific per-bridge variables */
  Bool 					begin; /* 13.23.1 */

  int                   mstid;        /* let's say: tag */
#ifndef SLR_STP_CODE
  char*                 name;           /* name of the MSTID, maily for debugging */
#else
  char                  name[NAME_LEN];
#endif
  UID_STP_MODE_T        admin_state;    /* STP_DISABLED or STP_ENABLED; type see in UiD */

  unsigned long         timeSince_Topo_Change; /* 14.8.1.1.3.b */
  unsigned long         Topo_Change_Count;     /* 14.8.1.1.3.c */
  unsigned char         Topo_Change;           /* 14.8.1.1.3.d */
} STPM_T;
