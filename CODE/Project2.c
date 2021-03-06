
/** ###################################################################
**     Filename  : Project2.c
**     Version   : ver2.0
**     Project   : Project
**     Processor : MC9S12C32MFA25
**     Version   : Driver 01.14
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 04-20-2022 3:15 pm
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
// Sensor and display methods

byte motorSeq[8] = {1, 9, 8, 12, 4, 6, 2, 3};
int motorL = 0;
int motorR = 0;
int stepsL = 0;
int stepsR = 0;
byte sensors[3];

int mouseX = 0;
int mouseY = 0;

// Final Project Global Material
int map[6][13] = {{0}};
int dx[4] = {-1, 1, 0, 0}; //λμλ¨λΆ
int dy[4] = {0, 0, -1, 1};
int place;

void writeCom(byte Command)
{
    byte upper = Command >> 4;

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

void InitializeLCD()
{
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

void writeData(byte data)
{
    byte upper = data >> 4;

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

void clearLCD()
{
    writeCom(0x01);
}

void displaySensors()
{
    int i = 0;
    char sensor1c[5];
    char sensor2c[5];
    char sensor3c[5];
    int sensors0 = 4 + abs(sensors[0] - 128) * .28;
    int sensors1 = 4 + abs(sensors[1] - 128) * .28;
    int sensors2 = 4 + abs(sensors[2] - 128) * .28;
    sprintf(sensor1c, "%d", sensors0);

    sprintf(sensor2c, "%d", sensors1);

    sprintf(sensor3c, "%d", sensors2);

    for (i = 0; i < strlen(sensor1c); i++)
    {
        writeData(sensor1c[i]);
    }
    writeData(0x20);

    for (i = 0; i < strlen(sensor2c); i++)
    {
        writeData(sensor2c[i]);
    }
    writeCom(0xC0);
    for (i = 0; i < strlen(sensor3c); i++)
    {
        writeData(sensor3c[i]);
    }
}

void pollSensors()
{
    Sensor_Measure(1);
    Sensor_GetValue8(sensors);
}

// Motor Methods
void powerMotors(int power)
{
    if (power == 1)
    {
        MotorR_EN_PutVal(1);
        MotorL_EN_PutVal(0x03);
        Cpu_Delay100US(40); // delay 4ms
    }
    else
    {
        MotorR_EN_PutVal(0);
        MotorL_EN_PutVal(0);
        Cpu_Delay100US(40); // delay 4ms
    }
}

void steps(int numsteps)
{
    int i = 0;
    powerMotors(1);
    for (i = 0; i < numsteps; i++)
    {
        motorR = (motorR + 1) % 8;
        motorL--;
        if (motorL < 0)
            motorL = 7;
        Cpu_Delay100US(40); // delay 4ms
    }
    powerMotors(0);
}

void Forward(void)
{
    powerMotors(1);
    stepsL = -367;
    stepsR = 367;
    while (stepsR != 0 && stepsL != 0)
    {
    }
    powerMotors(0);
}

void Right(void)
{
    powerMotors(1);
    stepsL = -200;
    stepsR = -200;
    while (stepsR != 0 && stepsL != 0)
    {
    }
    powerMotors(0);
}

void Left(void)
{
    powerMotors(1);
    stepsL = 200;
    stepsR = 200;
    while (stepsR != 0 && stepsL != 0)
    {
    }
    powerMotors(0);
}

void Turn(void)
{
    powerMotors(1);
    stepsL = 400;
    stepsR = 400;
    while (stepsR != 0 && stepsL != 0)
    {
    }
    powerMotors(0);
}

// Final Project Methods
int nx, ny;
void solveMaze()
{
    pollSensors();

    //λμ°©ν  λ κΉμ§
    while ((mouseX != 3 && mouseY != 6) || (mouseX != = 4 && mouseY != 6))
    {
        ///μ¬λ°© νμ
        for (int d = 0; d < 4; d++)
        {
            nx = dx[d] + mouseX; // next direction
            ny = dy[d] + mouseY;
            /* array out of index */
            if (nx < 0 || ny < 0 || nx >= 6 || ny >= 13)
            {
                map[mouseX][mouseY] = -1; //νμ¬ μμΉμμ ν μμλκ² μμ
                Turn();                   //λ€μ λ€λ‘κ°λ€
                Forward();                //μ΄μ μΌλ‘ λμκ°
                continue;
            }
            /* dead end */
            else if (sensors[0] > 80 && sensors[1] > 90 && sensors[2] > 80)
            {
                map[mouseX][mouseY] = -1; //νμ¬ μμΉμμ ν μμλκ² μμ
                Turn();
                Forward();
                continue;
            }
            /* νκ΅°λ° μ΄μ κ° μ μλ€. : λ μ λ¨ λΆ μμΌλ‘ traverseνκ³  μ?κΈΈ λ λ§λ€ 1λν΄μ§λ€.*/
            else
            {
                //μΌμͺ½κ³Ό μμ λ²½μ΄λΌλ©΄μ€λ₯Έμͺ½μΌλ‘ νμ 
                if (sensors[0] > 80 && sensors[1] > 90)
                    Right();
                //μΌμͺ½κ³Ό μ€λ₯Έμͺ½μ΄ λ²½ -> νμ ν  νμ x
                else if (sensors[0] > 80 && sensors[2] > 80)
                {
                }
                //μμͺ½κ³Ό μ€λ₯Έμͺ½μ΄ λ²½ -> μΌμͺ½μΌλ‘ νμ 
                else if (sensors[1] > 90 && sensors[2] > 80)
                    Left();
                //λ²½μ΄ νμͺ½λ°μ μκ±°λ μμμμΌλ©΄ λ²½μ΄ μλμͺ½ μλ¬΄λλ κ΄μ°?λ€
                else
                {
                    //λ²½μ΄ μμΌλ©΄ break -> iκ° 1 2 3μΈκ²½μ°λ‘ κ°λ©΄ λλ€.
                    int i;
                    for (i = 0; i < 3; i++)
                    {
                        if (sensors[i] > 80)
                            break;
                    }
                    if (i == 1)
                    {
                        Left();
                    }
                    else if (i == 2)
                    {
                        continue; //λ°©ν₯μ ν νμ μμ
                    }
                    else
                    {
                        Right();
                    }
                }
                // mouseXμ mouseYμ μμΉμ 1μ λνλ€ (μλ€κ°λ€λ λ»)
                map[mouseX][mouseY]++;
                // μ§μ§νλ€.
                Forward();
                Cpu_Delay100US(750);
            }
        }
    }
    return;
}

// End of Project material

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

    while (1)
    {

        if ((mouseX == 2 || mouseX == 3) && (mouseY == 6))
        {
            break;
        }

        Cpu_Delay100US(1100);
        solveMaze();
        // Forward();
        Cpu_Delay100US(1000);
    }

    /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
    for (;;)
    {
    }
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
