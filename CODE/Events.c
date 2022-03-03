 /** ###################################################################
**     Filename  : Events.c
**     Project   : Project
**     Processor : MC9S12C32MFA25
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 2/27/2019, 5:41 PM
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         PID_OnInterrupt               - void PID_OnInterrupt(void);
**         MotorR_DriveTimer_OnInterrupt - void MotorR_DriveTimer_OnInterrupt(void);
**         MotorL_DriveTimer_OnInterrupt - void MotorL_DriveTimer_OnInterrupt(void);
**         MotorR_DutyTimer_OnInterrupt  - void MotorR_DutyTimer_OnInterrupt(void);
**         MotorL_DutyTimer_OnInterrupt  - void MotorL_DutyTimer_OnInterrupt(void);
**
** ###################################################################*/
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"
extern byte motorSeq[8];
extern int motorL;
extern int motorR;
extern int stepsL;
extern int stepsR;
int toggle1 = 0;
int toggle2 = 0;
extern byte sensors[3];
int integral = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  PID_OnInterrupt (module Events)
**
**     Component   :  PID [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


void MotorR_WriteBits(byte value){
   MotorRDrive_LCD_DB_PutVal(value);   
}
void MotorL_WriteBits(byte value){
   MotorLDrive_1_PutVal((value & 0x01)&& 0x01);
   MotorLDrive_2_PutVal((value & 0x02)&& 0x02);
   MotorLDrive_4_PutVal((value & 0x04)&& 0x04);
   MotorLDrive_8_PutVal((value & 0x08)&& 0x08);
}

void PID_OnInterrupt(void)
{
  /* Write your code here ... */
  
  //int k = 20;   //change later
  //int left = sensors[0];
  //int right = sensors[2];
  //int error = (left-right)/2;
  //MotorR_DriveTimer_SetPeriodUS(4000+k*error);
  //MotorL_DriveTimer_SetPeriodUS(4000-k*error) ;
  
  int k = 30; //some value you fine tune
  int ki = 3;
  int left = sensors[0];
  int right = sensors[2];
  int error = (left - right) / 2;
  if(abs(error) > 20) {
   k = 0;
   ki = 0;
  } 
  else if(abs(error < 200))
  {
    k = 0;
   ki = 0;
  }
  integral = integral + error;
  MotorR_DriveTimer_SetPeriodUS(4000 + (error * k) + (integral * ki));
  MotorL_DriveTimer_SetPeriodUS(4000 - (error * k) - (integral * ki));
  if (left == right) {
    integral = 0; 
  }
  
}

/*
** ===================================================================
**     Event       :  MotorR_DriveTimer_OnInterrupt (module Events)
**
**     Component   :  MotorR_DriveTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorR_DriveTimer_OnInterrupt(void)
{
  /* Write your code here ... */
  
  if (stepsR > 0) {
     motorR=(motorR+1)%8;
     stepsR--;
  } else if (stepsR < 0) {
     motorR--;
     if(motorR<0) motorR=7;
     stepsR++;
  }
  
}

/*
** ===================================================================
**     Event       :  MotorL_DriveTimer_OnInterrupt (module Events)
**
**     Component   :  MotorL_DriveTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorL_DriveTimer_OnInterrupt(void)
{
  /* Write your code here ... */
  
  if (stepsL > 0) {
     motorL=(motorL+1)%8;
     stepsL--;
  } else if (stepsL < 0) {
     motorL--;
     if(motorL<0) motorL=7;
     stepsL++;
  }
  
}

/*
** ===================================================================
**     Event       :  MotorR_DutyTimer_OnInterrupt (module Events)
**
**     Component   :  MotorR_DutyTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorR_DutyTimer_OnInterrupt(void)
{
  /* Write your code here ... */
  
  if(toggle2){
    MotorR_WriteBits(0);
    toggle2=0;
 } else{
      MotorR_WriteBits(motorSeq[motorR]);
      toggle2=1;
 }
  
}

/*
** ===================================================================
**     Event       :  MotorL_DutyTimer_OnInterrupt (module Events)
**
**     Component   :  MotorL_DutyTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorL_DutyTimer_OnInterrupt(void)
{
  /* Write your code here ... */
  
  if(toggle1){
    MotorL_WriteBits(0);
    toggle1=0;
 } else{
      MotorL_WriteBits(motorSeq[motorL]);
      toggle1=1;
 }
  
}

/*
** ===================================================================
**     Event       :  Sensor_OnEnd (module Events)
**
**     Component   :  Sensor [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Sensor_OnEnd(void)
{
  /* Write your code here ... */
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
