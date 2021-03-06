/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : PE_Timer.c
**     Project   : Project
**     Processor : MC9S12C32MFA25
**     Component : PE_Timer
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 4/10/2019, 7:24 PM
**     Abstract  :
**         This module "PE_Timer" implements internal methods and definitions
**         used by components working with timers.
**     Settings  :
**     Contents  :
**         No public methods
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE PE_Timer. */

#include "PE_Timer.h"


typedef unsigned long UINT32;

typedef union {
  UINT32 val;
  struct {
    word hi16, lo16;
  } s;
} OP_UINT32;

typedef struct {
  word dummy;
  UINT32 mid;
} M_UINT32;

typedef struct {
  UINT32 hi32, lo32;
} UINT64;

typedef union {
  UINT64 val;
  M_UINT32 m;
  dlong DL;
} OP_UINT64;

/*
** ===================================================================
**     Method      :  PE_Timer_LngMul (component PE_Timer)
**
**     Description :
**         The method multiplies two 32 bit values and returns pointer to 
**         64 bit result.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_Timer_LngMul(dword va1, dword va2, dlong *var)
{
  OP_UINT64 Result;

  Result.val.hi32 = 0UL;
  Result.val.lo32 = (va1 & 0xFFFFU) * (va2 & 0xFFFFU);
  {
    OP_UINT32 tmp;

    tmp.val = (va1 & 0xFFFFU) * ((va2 >> 16U) & 0xFFFFU);
    Result.m.mid += (UINT32)tmp.s.lo16;
    Result.val.hi32 += (UINT32)tmp.s.hi16;
  }
  {
    OP_UINT32 tmp;

    tmp.val = ((va1 >> 16U) & 0xFFFFU) * (va2 & 0xFFFFU);
    Result.m.mid += (UINT32)tmp.s.lo16;
    Result.val.hi32 += (UINT32)tmp.s.hi16;
  }
  Result.val.hi32 += ((va1 >> 16U) & 0xFFFFU) * ((va2 >> 16U) & 0xFFFFU);
  /* *** */
  (*var)[0] = Result.DL[0];
  (*var)[1] = Result.DL[1];
}

/*
** ===================================================================
**     Method      :  PE_Timer_LngHi2 (component PE_Timer)
**
**     Description :
**         The method transfers 64 bit result to 16 bit ratio value and 
**         returns overflow flag.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
bool PE_Timer_LngHi2(dword High, dword Low, word *Out)
{
  if (High == 0UL) {
    if ((Low & 0x8000UL) != 0UL) {
      if ((Low >> 16U) < 0xFFFFUL) {
        *Out = ((word)(Low >> 16U)) + 1U;
        return FALSE;
      }
    } else {
      *Out = (word)(Low >> 16U);
      return FALSE;
    }
  }
  *Out = (word)(Low >> 16U);
  return TRUE;
}

/*
** ===================================================================
**     Method      :  PE_Timer_LngHi4 (component PE_Timer)
**
**     Description :
**         The method transfers 64 bit result to 16 bit ratio value and 
**         returns overflow flag.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
bool PE_Timer_LngHi4(dword High, dword Low, word *Out)
{
  if ((High >> 16U) == 0U) {
    if ((Low & 0x80000000UL) != 0UL) {
      if (High < 0xFFFFUL) {
        *Out = ((word)High) + 1U;
        return FALSE;
      }
    } else {
      *Out = (word)High;
      return FALSE;
    }
  }
  *Out = (word)High;
  return TRUE;
}



/* END PE_Timer. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
