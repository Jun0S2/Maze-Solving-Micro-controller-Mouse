/** ###################################################################
**     Filename  : Project.c
**     Project   : Project
**     Processor : MC9S12C32MFA25
**     Version   : Driver 01.14
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 2/27/2019, 5:41 PM
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Project */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "MotorR_EN.h"
#include "MotorL_EN.h"
#include "MotorRDrive_LCD_DB.h"
#include "MotorLDrive_1.h"
#include "MotorLDrive_2.h"
#include "MotorLDrive_4.h"
#include "MotorLDrive_8.h"
#include "MotorL_DutyTimer.h"
#include "MotorR_DutyTimer.h"
#include "MotorL_DriveTimer.h"
#include "MotorR_DriveTimer.h"
#include "PID.h"
#include "Sensor.h"
#include "LCD_RS.h"
#include "LCD_EN.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include <string.h>
#include <stdio.h>

/* User includes (#include below this line is not maintained by Processor Expert) */
  //Sensor and display methods 
  
  
  byte motorSeq[8]={1, 9, 8 ,12, 4, 6, 2, 3};
  int motorL=0;
  int motorR=0;
  int stepsL=0;
  int stepsR=0;
  byte sensors[3];
  int heading[4] = {1,0,0,0};
  int mouseX = 0;
  int mouseY = 0;
  
  //Final Project Global Material 
  int maze[6][13][4] = 
  {
    {{0,0,1,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{1,0,0,1}},
    {{0,0,1,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,0,0,0}},
    {{0,0,1,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,0,0,0}},
    {{0,0,1,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,0,0,0}},
    {{0,0,1,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,0,0,0}},
    {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{1,1,0,0}},
  };
  int place;
  
  int mazeDist[6][13] = 
  {
    {8,7,6,5,4,3,2,3,4,5,6,7,8},
    {7,6,5,4,3,2,1,2,3,4,5,6,7},
    {6,5,4,3,2,1,0,1,2,3,4,5,6},
    {6,5,4,3,2,1,0,1,2,3,4,5,6},
    {7,6,5,4,3,2,1,2,3,4,5,6,7},
    {8,7,6,5,4,3,2,3,4,5,6,7,8},
  };
  

  


void writeCom(byte Command){
  byte upper = Command >>4;

  LCD_RS_PutVal(FALSE);
  LCD_EN_PutVal(FALSE);

  MotorRDrive_LCD_DB_PutVal(upper);


  LCD_EN_PutVal(TRUE);
  LCD_EN_PutVal(FALSE);

  MotorRDrive_LCD_DB_PutVal(Command & 15);

  LCD_EN_PutVal(TRUE);
  LCD_EN_PutVal(FALSE);

  Cpu_Delay100US(100);

}

void InitializeLCD(){
  Cpu_Delay100US(160);
  writeCom(0x30);
  Cpu_Delay100US(45);
  writeCom(0x30);
  Cpu_Delay100US(2);
  writeCom(0x30);
  writeCom(0x20);
  writeCom(0x28);
  writeCom(0x0C);
  writeCom(0x01);
  writeCom(0x06);
  Cpu_Delay100US(100);
}

void writeData(byte data){
  byte upper = data >>4;

  LCD_RS_PutVal(TRUE);
  LCD_EN_PutVal(FALSE);

  MotorRDrive_LCD_DB_PutVal(upper);

  LCD_EN_PutVal(TRUE);
  LCD_EN_PutVal(FALSE);

  MotorRDrive_LCD_DB_PutVal(data & 15);

  LCD_EN_PutVal(TRUE);
  LCD_EN_PutVal(FALSE);

  Cpu_Delay100US(100);
}

void clearLCD(){
  writeCom(0x01);
}

void displaySensors(){
  int i = 0;
  char sensor1c[5];
  char sensor2c[5];
  char sensor3c[5];
  int sensors0 = 4 + abs(sensors[0] - 128)*.28;
  int sensors1 = 4 + abs(sensors[1] - 128)*.28;
  int sensors2 = 4 + abs(sensors[2] - 128)*.28;
  sprintf(sensor1c,"%d",sensors0);
  
  sprintf(sensor2c,"%d", sensors1);
  
  sprintf(sensor3c,"%d",  sensors2);
  
  for(i = 0; i < strlen(sensor1c); i++) {
    writeData(sensor1c[i]);
  }
  writeData(0x20);
  
  for(i = 0; i < strlen(sensor2c); i++) {
    writeData(sensor2c[i]);
  }
  writeCom(0xC0);
  for(i = 0; i < strlen(sensor3c); i++) {
    writeData(sensor3c[i]);
  }
}

void pollSensors(){
  Sensor_Measure(1);
  Sensor_GetValue8(sensors);
}
  

  
//Motor Methods 
void powerMotors(int power){
   if(power==1){
    MotorR_EN_PutVal(1);
    MotorL_EN_PutVal(0x03);
    Cpu_Delay100US(40);//delay 4ms
   }else{
    MotorR_EN_PutVal(0);
    MotorL_EN_PutVal(0);
    Cpu_Delay100US(40);//delay 4ms
   }
}

void steps(int numsteps){
    int i=0;
    powerMotors(1);
    for(i=0; i<numsteps; i++){
     motorR=(motorR+1)%8;
     motorL--;
     if(motorL<0) motorL=7;
     Cpu_Delay100US(40);//delay 4ms
    }
    powerMotors(0);
}

void Forward(void) {
    powerMotors(1);
    stepsL=-367;
    stepsR=367;
    while(stepsR != 0 && stepsL != 0) {
       
    }
    powerMotors(0);
}

void Right(void) {
    powerMotors(1);
    stepsL=-200;
    stepsR=-200;
    while(stepsR != 0 && stepsL != 0) {
      
    }
    powerMotors(0);
}

void Left(void) {
    powerMotors(1);
    stepsL=200;
    stepsR=200;
    while(stepsR != 0 && stepsL != 0) {
      
    }
    powerMotors(0);
}

void Turn(void) {
    powerMotors(1);
    stepsL=400;
    stepsR=400;
    while(stepsR != 0 && stepsL != 0) {
      
    }
    powerMotors(0);
}


//Final Project Methods  
  void solveMaze() 
  {
  
     int north = -1;
     int south = -1;
     int east = -1;
     int west = -1;
     //Larger for distance selection
     int compare = 1000000;
     int direction[4] = {0};
     
     
     //Just Testing
     //Forward();
     //Testing 
     
   
  
  
    pollSensors();
    
    //Cpu_Delay100US(750);

  
    //Check Walls
                                                     
    
     //modifies orientation value relative to north 
    
     place = -1;
    
     if(heading[0] == 1) 
     {
      
      place = 0;
      
     } 
     if(heading[1] == 1) 
     {
     
      // what it was originaly // place = 1;
      place = 1;
     
     }
     if(heading[2] == 1) 
     {
     
      // what it was originaly //place = 2;
      place = 2;
     
     }
     else if(heading[3] == 1) 
     {
      //was three intially 
      place = 3;
     
     }
    
     //check left 
     if(sensors[0] > 80) 
     {
     
       int placeLeft = place;
       placeLeft = place - 1;
       
       if(placeLeft < 0) {
       //was 4 I think this would have caused array index outofbounds
       placeLeft = 3;
       }
       
       maze[mouseX][mouseY][placeLeft] = 1;
       
     }
     
    
    //Testing so that walls are never generated
    
    //check front 
    
    if(sensors[1] > 90) 
     {
       int placeFront = place; 
       
       maze[mouseX][mouseY][placeFront] = 1;
       
     }
    
    //check right 
    
      // Had sensor value wrong
      
     if(sensors[2] > 80) 
     {
     
       int placeRight = place;
       placeRight = place + 1;
       
       if(placeRight > 3) {
       placeRight = 0;
       }
       
       maze[mouseX][mouseY][placeRight] = 1;
       
     }
    
    //Update the array 
    
    
    //Add two to the current position 
    
    mazeDist[mouseX][mouseY] = ((mazeDist[mouseX][mouseY]) + 4);
    
    //Case Dead End
    //Broke this by makeing -3 
    if(((maze[mouseX][mouseY][0]) + (maze[mouseX][mouseY][1]) + (maze[mouseX][mouseY][2]) + (maze[mouseX][mouseY][3])) == 5) 
    {
      //Now check heading in order to determine which value to moddify the value of the current square to 1 more than the square behind
      
      //Testing
      
      
      
      //If robot is pointing North
      if(heading[0] == 1) 
      {
        mazeDist[mouseX][mouseY] = ((mazeDist[mouseX][mouseY -1]) + 4); 
      }
      
      //If robot is pointing East
      if(heading[1] == 1) 
      {
        mazeDist[mouseX][mouseY] = ((mazeDist[mouseX - 1][mouseY]) + 4); 
      }
      
      //If robot is pointing South  
      if(heading[2] == 1) 
      {
        mazeDist[mouseX][mouseY] = ((mazeDist[mouseX ][mouseY + 1]) + 4); 
      }
      
      //If robot is pointing West
      if(heading[3] == 1) 
      {
        mazeDist[mouseX][mouseY] = ((mazeDist[mouseX + 1][mouseY]) + 4); 
      }
    
    }
    
   //Removed Case two trying my modified wallflower
   
    
  
    
    //move closest square with an emphasis on going left 
    //Make sure the ifs are checking the correct statement 
    //dosent implement left prefference 
  
   
   direction[0] = 0;
   direction[1] = 0;
   direction[2] = 0;
   direction[3] = 0;
   
   //check north
   if(maze[mouseX][mouseY][0] == 0) {
    
     north = mazeDist[mouseX][mouseY + 1];
     
     //Testing
      //Forward();
     
     
     //Used for best path slection
     if(north < compare)
     {
      compare = north;
      direction[0] = 1;
      direction[1] = 0;
      direction[2] = 0;
      direction[3] = 0;
     }
    
   }
   
    
    //check south 
    //Add conditional to prevent index out of bounds
     if(maze[mouseX][mouseY][2] == 0) {
    
     south = mazeDist[mouseX][mouseY-1];
     
     //used for best path selection
     if(south < compare) 
     {
       compare = south;
       direction[1] = 0;
       direction[0] = 0;
       direction[2] = 1;
       direction[3] = 0;
     }
    
   }
    
    
    //check east 
    
     if(maze[mouseX][mouseY][1] == 0) {
    
     east = mazeDist[mouseX+1][mouseY];
     
     //used for best path selection
     if(east < compare)
     {
       compare = east;
       direction[0] = 0;
       direction[2] = 0;
       direction[1] = 1;
       direction[3] = 0;
       
     }
    
   }
    
    //check west 
    
    if(maze[mouseX][mouseY][3] == 0) {
    
     west = mazeDist[mouseX-1][mouseY];
     
     //used for best path selection
     if(west < compare) {
        compare = west;
        direction[2] = 0;
        direction[1] = 0;
        direction[3] = 1;
        direction[0] = 0;
     
     }
    
    
   } 
   
   //Find which is smallest Already done 
   
   
   
   //Testing
  
   //direction[2] = 1;
   
  
     Cpu_Delay100US(750);

   
   
    
    //if want to move north move 
    if(direction[0] == 1)
    {
         //if already pointing north
           if(heading[0] == 1) {
            Forward();
           
           } 
           else if(heading[1] == 1)
           {
               // if pointing east
               
               Left();
               Forward();
           } 
           else if(heading[2] == 1)
           {
               //if pointing south
               
               Turn();
               Forward();
           } 
           else
           {
                //if pointing west
                
                Right();
                Forward();
           } 
           
           mouseY++;
           
           //Need to modify the heading array 
         heading[0] = 1;
         heading[1] = 0;
         heading[2] = 0;
         heading[3] = 0;
         
    }
      
      
    //Testing
    //heading[0] = 1;
    //direction[1] = 1;  
     
      
    //if east move
    if(direction[1] == 1)
    {
            //if pointing north
           if(heading[0] == 1) 
           {
            Right();
            Forward();
           } 
           else if(heading[1] == 1)
           {
               // if already pointing east
               Forward();
           } 
           else if(heading[2] == 1)
           {
               //if pointing south
               Left();
               Forward();
           } 
           else
           {
                //if pointing west
                Turn();
                Forward();
           } 
           
           mouseX++;
           //Need to modify the heading array 
           heading[0] = 0;
           heading[1] = 1;
           heading[2] = 0;
           heading[3] = 0;
            
    }
    //if south move
    if(direction[2] == 1)
    {
         //if pointing north
           if(heading[0] == 1) 
           {
            Turn();
            Forward();
           } 
           else if(heading[1] == 1)
           {
               // if pointing east
               Right();
               Forward();
           } 
           else if(heading[2] == 1)
           {
               //if already pointing south
               Forward();
           } 
           else
           {
                //if pointing west
                Left();
                Forward();
           }
           
           mouseY--;
           
           //Need to modify the heading array
           heading[0] = 0;
           heading[1] = 0;
           heading[2] = 1;
           heading[3] = 0;
            
             
    }
    
    //if west move
    if(direction[3] == 1)
    {
         //if pointing north
           if(heading[0] == 1) 
           {
            Left();
            Forward();
           } 
           else if(heading[1] == 1)
           {
               // if pointing east
               Turn();
               Forward();
           } 
           else if(heading[2] == 1)
           {
               //if pointing south
               Right();
               Forward();
           } 
           else
           {
                //if already pointing west
                Forward();
           }
           
           //Need to modify the heading array 
           heading[0] = 0;
           heading[1] = 0;
           heading[2] = 0;
           heading[3] = 1;
           
           
           mouseX--;
           //End
           
           
           
    }
       Cpu_Delay100US(1000);
       return;
  }
  
  //End of Project material   

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  Cpu_Delay100US(100);
  InitializeLCD();
  
  Cpu_Delay100US(750);
  Cpu_Delay100US(750);
  Cpu_Delay100US(750);
  Cpu_Delay100US(10000);
  
 while(1) 
 {
  
     if((mouseX == 2 || mouseX == 3) && (mouseY == 6))
    {
     break; 
    }
  
    Cpu_Delay100US(1100);
    solveMaze(); 
    //Forward();
    Cpu_Delay100US(1000);

 } 
   
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END Project */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
