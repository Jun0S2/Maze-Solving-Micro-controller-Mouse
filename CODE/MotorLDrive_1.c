/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : MotorLDrive_1.c
**     Project   : Project
**     Processor : MC9S12C32MFA25
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.15, CPU db: 2.87.402
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 4/10/2019, 6:07 PM
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       45            |  PM0_RxCAN
**             ----------------------------------------------------
**
**         Port name                   : M
**
**         Bit number (in port)        : 0
**         Bit mask of the port        : $0001
**
**         Initial direction           : Output (direction can be changed)
**         Safe mode                   : yes
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTM       [$0250]
**         Port control register       : DDRM      [$0252]
**
**         Optimization for            : speed
**     Contents  :
**         SetDir - void MotorLDrive_1_SetDir(bool Dir);
**         GetVal - bool MotorLDrive_1_GetVal(void);
**         PutVal - void MotorLDrive_1_PutVal(bool Val);
**         ClrVal - void MotorLDrive_1_ClrVal(void);
**         SetVal - void MotorLDrive_1_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE MotorLDrive_1. */

#include "MotorLDrive_1.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma DATA_SEG MotorLDrive_1_DATA    /* Select data segment "MotorLDrive_1_DATA" */
#pragma CODE_SEG MotorLDrive_1_CODE
#pragma CONST_SEG MotorLDrive_1_CONST  /* Constant section for this module */
/*
** ===================================================================
**     Method      :  MotorLDrive_1_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool MotorLDrive_1_GetVal(void)

**  This method is implemented as a macro. See MotorLDrive_1.h file.  **
*/

/*
** ===================================================================
**     Method      :  MotorLDrive_1_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void MotorLDrive_1_PutVal(bool Val)
{
  if (Val) {
    setReg8Bits(PTM, 0x01U);           /* PTM0=0x01U */
    Shadow_M |= 0x01U;                 /* Set-up shadow variable */
  } else { /* !Val */
    clrReg8Bits(PTM, 0x01U);           /* PTM0=0x00U */
    Shadow_M &= 0xFEU;                 /* Set-up shadow variable */
  } /* !Val */
}

/*
** ===================================================================
**     Method      :  MotorLDrive_1_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void MotorLDrive_1_ClrVal(void)

**  This method is implemented as a macro. See MotorLDrive_1.h file.  **
*/

/*
** ===================================================================
**     Method      :  MotorLDrive_1_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void MotorLDrive_1_SetVal(void)

**  This method is implemented as a macro. See MotorLDrive_1.h file.  **
*/

/*
** ===================================================================
**     Method      :  MotorLDrive_1_SetDir (component BitIO)
**
**     Description :
**         This method sets direction of the component.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void MotorLDrive_1_SetDir(bool Dir)
{
  if (Dir) {
    setReg8(PTM, (getReg8(PTM) & (byte)(~(byte)0x01U)) | (Shadow_M & 0x01U)); /* PTM0=Shadow_M[bit 0] */
    setReg8Bits(DDRM, 0x01U);          /* DDRM0=0x01U */
  } else { /* !Dir */
    clrReg8Bits(DDRM, 0x01U);          /* DDRM0=0x00U */
  } /* !Dir */
}


/* END MotorLDrive_1. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
