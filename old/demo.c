/************************************************************************ 
 * RSTP library - Rapid Spanning Tree (802.1t, 802.1w) 
 * Copyright (C) 2001-2003 Optical Access 
 * Author: Alex Rozin 
 * 
 * This file is part of RSTP library. 
 * 
 * RSTP library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; version 2.1 
 * 
 * RSTP library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with RSTP library; see the file COPYING.  If not, write to the Free 
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 
 * 02111-1307, USA. 
 **********************************************************************/

/* Generic (abstract) state machine : 17.13, 17.14 */
 
#ifdef SLR_STP_CODE
#include <string.h>

#include "mstp_lib_base.h"
#include "mstp_lib_statmch.h"
#else
#include "base.h"
#include "statmch.h"
#include "mstp_lib_statmch.h"

#endif

#if STP_DBG
#ifdef SLR_STP_CODE
#  include "mstp_lib_stpm.h"
#else
#  include "stpm.h"
#endif
#endif

STATE_MACH_T *
STP_state_mach_create (void (*concreteEnterState) (STATE_MACH_T*),
                       Bool (*concreteCheckCondition) (STATE_MACH_T*),
                       char *(*concreteGetStatName) (int),
                       void *owner, char *name)
{
  STATE_MACH_T *this;

#ifdef SLR_STP_CODE
  MSTP_TRC(MSTP, TRC_INFORMATIVE + (FMASK & F_STP_LIB_STATMCH),
  "%s\n", __FUNCTION__);
#endif


  STP_MALLOC(this, STATE_MACH_T, "state machine");
 
  this->State = BEGIN;
#ifdef SLR_STP_CODE
  strcpy(this->name, name);
#else
  this->name = (char*) strdup (name);
#endif
  this->changeState = False;
#if STP_DBG
  this->debug = False;
  this->ignoreHop2State = BEGIN;
#endif
  this->concreteEnterState = concreteEnterState;
  this->concreteCheckCondition = concreteCheckCondition;
  this->concreteGetStatName = concreteGetStatName;
  this->owner.owner = owner;

  return this;
}
                              
void
STP_state_mach_delete (STATE_MACH_T *this)
{

#ifdef SLR_STP_CODE
  MSTP_TRC(MSTP, TRC_INFORMATIVE + (FMASK & F_STP_LIB_STATMCH),
  "%s\n", __FUNCTION__);
#endif


#ifndef SLR_STP_CODE
  free (this->name);
#endif
  STP_FREE(this, "state machine");
}

Bool
STP_check_condition (STATE_MACH_T* this)
{
  Bool bret;

#ifdef SLR_STP_CODE
  MSTP_TRC(MSTP, TRC_INFORMATIVE+1 + (FMASK & F_STP_LIB_STATMCH),
  "%s\n", __FUNCTION__);
#endif

  bret = (*(this->concreteCheckCondition)) (this);
  if (bret) {
    this->changeState = True;
  }
  
  return bret;
}
        
Bool
STP_change_state (STATE_MACH_T* this)
{
  register int number_of_loops;

#ifdef SLR_STP_CODE
  MSTP_TRC(MSTP, TRC_INFORMATIVE+1 + (FMASK & F_STP_LIB_STATMCH),
  "%s\n", __FUNCTION__);
#endif

  for (number_of_loops = 0; ; number_of_loops++) {
    if (! this->changeState) return number_of_loops;
    (*(this->concreteEnterState)) (this);
    this->changeState = False;
    STP_check_condition (this);
  }

  return number_of_loops;
}

Bool
STP_hop_2_state (STATE_MACH_T* this, unsigned int new_state)
{

#ifdef SLR_STP_CODE
  MSTP_TRC(MSTP, TRC_INFORMATIVE+1 + (FMASK & F_STP_LIB_STATMCH),
  "%s(newState=%d)\n", __FUNCTION__, new_state);
#endif

#ifdef STP_DBG
  switch (this->debug) {
    case 0: break;
    case 1:
      if (new_state == this->State || new_state == this->ignoreHop2State) break;
#ifdef SLR_STP_CODE
      stp_trace ("%-8s(%s-%s-0x%x): %s=>%s",
        this->name,
        *this->owner.port->owner->name ? this->owner.port->owner->name : "Glbl",
        this->owner.port->port_name,
        this->owner.port->portSlrId,
        (*(this->concreteGetStatName)) (this->State),
        (*(this->concreteGetStatName)) (new_state));
#else
      stp_trace ("%-8s(%s-%s): %s=>%s",
        this->name,
        *this->owner.port->owner->name ? this->owner.port->owner->name : "Glbl",
        this->owner.port->port_name,
        (*(this->concreteGetStatName)) (this->State),
        (*(this->concreteGetStatName)) (new_state));
#endif
      break;
    case 2:
      if (new_state == this->State) break;
      stp_trace ("%s(%s): %s=>%s", 
        this->name,
        *this->owner.stpm->name ? this->owner.stpm->name : "Glbl",
        (*(this->concreteGetStatName)) (this->State),
        (*(this->concreteGetStatName)) (new_state));
      break;
  }
#endif
  switch (this->state_mach_type) {
#if 0
    case 2:
      if (mst_print_debug_msg_ok(0)) {
        printf("%s(%s): %s=>%s\n",this->name,*this->owner.stpm->name ? this->owner.stpm->name : "Glbl",
            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
        }
        break;
    case 1:
       if (mst_print_debug_msg_ok(this->owner.port)) {
          printf("%s(%s): %s=>%s\n",this->name,*this->owner.port->owner->name ? this->owner.port->owner->name : "Glbl",
            (*(this->concreteGetStatName)) (this->State),(*(this->concreteGetStatName)) (new_state));
        }
        break;
   #endif
  }

  this->State = new_state;
  this->changeState = True;
  return True;
}


#if 0

void mst_statmch_enter_state_print_info(STATE_MACH_T *this)
{

  switch (this->state_mach_type) {
  case 2:
    if (mst_print_debug_msg_ok(0)) {
      printf("%s(%s):[Enter]=>%s\n",this->name,*this->owner.stpm->name?this->owner.stpm->name :"Glbl",(*(this->concreteGetStatName))(this->State));
    }
    break;
  case 1:
    if (mst_print_debug_msg_ok(this->owner.port)) {
      printf("%s(%s):[Enter]=>%s\n",this->name,*this->owner.port->owner->name ? this->owner.port->owner->name : "Glbl",(*(this->concreteGetStatName))(this->State));
      }
      break;
  }
   
}

#endif 

static int
_stp_stpm_init_machine (STATE_MACH_T* this)
{
#if defined(SLR_STP_CODE) && defined(STP_FN_CALL_TRACING)
  MSTP_TRC(MSTP, TRC_INFORMATIVE + (FMASK & F_STP_LIB_STPM),
    "In stp_lib_stpm.c:_stp_stpm_init_machine(thisPtr=0x%x)", 
      this);
#endif
  this->State = BEGIN;
  (*(this->concreteEnterState)) (this);
  return 0;
}

static int
_stp_stpm_iterate_machines (STPM_T* this,
                           int (*iter_callb) (STATE_MACH_T*),
                           Bool exit_on_non_zero_ret)
{
  register STATE_MACH_T* stater;
  register PORT_T*       port;
  int                    iret, mret = 0;

#if defined(SLR_STP_CODE) && defined(STP_FN_CALL_TRACING)
/*
  STP_TRC(STP, TRC_INFORMATIVE + (FMASK & F_STP_LIB_STPM),
    "stp_lib_stpm.c:_stp_stpm_iterate_machines(thisPtr=0x%x,iterCBptr=0x%x,exit..=%d)", 
      this, iter_callb, exit_on_non_zero_ret);
*/

#endif
  /* state machines per bridge */
  for (stater = this->machines; stater; stater = stater->next) {
    iret = (*iter_callb) (stater);
    if (exit_on_non_zero_ret && iret)
      return iret;
    else
      mret += iret;
  }

  /* state machines per port */
  for (port = this->ports; port; port = port->next) {
    for (stater = port->machines; stater; stater = stater->next) {
      iret = (*iter_callb) (stater);
      if (exit_on_non_zero_ret && iret) 
        return iret;
      else
        mret += iret;
    }
  }
  
  return mret;
}



int
STP_stpm_update (STPM_T* this) /* returns number of loops */
{
  register Bool     need_state_change;
  register int      number_of_loops = 0;
#ifdef SLR_STP_CODE
  PORT_T *port;
#endif

#if defined(SLR_STP_CODE) && defined(STP_FN_CALL_TRACING)
  MSTP_TRC(MSTP, TRC_INFORMATIVE+1 + (FMASK & F_STP_LIB_STPM),
    "In stp_lib_stpm.c:STP_stpm_update(thisPtr=0x%x)", 
      this);
#endif
  need_state_change = False; 
  
  for (;;) {/* loop until not need changes */
    need_state_change = _stp_stpm_iterate_machines (this,
                                                   STP_check_condition,
                                                   True);
#ifdef SLR_STP_CODE
    if (! need_state_change) break;
#else
    if (! need_state_change) return number_of_loops;
#endif

//printf(" --- changing state, since state changed needed\n");

    number_of_loops++;
    /* here we know, that at least one stater must be
       updated (it has changed state) */
    number_of_loops += _stp_stpm_iterate_machines (this,
                                                  STP_change_state,
                                                  False);

  }

#ifdef SLR_STP_CODE
      // update GDB
      //stp_updateDmStateInGDB(this);
      for (port = this->ports; port; port = port->next) {
          // skip unchanged port
     if (!port->changed) {
              continue;
          }
     
          // skip PON port
          if (port->port_index == STP_PON_PORT_INDEX) {
              continue;
          }
          // skip virtual ONU PON port
          if (objId_oo(port->portSlrId)) {
              continue;
          }
          port->changed = 0;
      }
#endif

  return number_of_loops;
}

void
STP_stpm_one_second (STPM_T* param)
{
  STPM_T*           this = (STPM_T*) param;
  register PORT_T*  port;
  register int      iii;

  if (STP_ENABLED != this->admin_state) return;

  for (port = this->ports; port; port = port->next) {
    for (iii = 0; iii < TIMERS_NUMBER; iii++) {
      if (*(port->timers[iii]) > 0) {
        (*port->timers[iii])--;
      }
    }    
    port->uptime++;
  }

  STP_stpm_update (this);
  this->Topo_Change = _check_topoch (this);
  if (this->Topo_Change) {
    this->Topo_Change_Count++;
    this->timeSince_Topo_Change = 0;
  } else {
    this->Topo_Change_Count = 0;
    this->timeSince_Topo_Change++;
  }
}
