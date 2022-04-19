# Micro-controller Maze Solving Mouse

In this project, all algorithms are allowed to be used in the creation of the program processed by the microcontroller (MC912C32) in addition to the Dragonflybot board. Programming will be done with Freescale IDE that enables groups to develop embedded applications on HCS12 systems. The micromouse will solve a 6 x 13 maze with some rules discussed below.

## Problem Statement

The micromouse in 6 x 13 maze is required to reach the center of the maze from bottom left.
Micromouse must be self-contained without any remote controls and shall not damage or destroy the
wall of the maze.

![MazeRules](./assets/Rules.JPG)

## Mechanical Platform and Electrical Platform

### Discussion : Advantages and Disadvantages

In this project, stepper motor instead of DC motor. DC motors are cheaper and easy to use, however they
are not very accurate and overheat easily when they run too fast compared to stepper motors. Because
the ultimate goal when building micromouse is to solve the maze accurately and fast, these features will
prevent the micromouse from completing the maze in the most optimal manner.

#### Pin-out Diagram showing pins used on HCS12

![Figure1](./assets/Figure1.JPG)

#### Micromouse Schematic

![Figure2](./assets/Figure2.JPG)

## Software Platform

### Beans and Methods Used

1. Project.c

   ```
   - void pollSensors() : It polls the sensor values from the micromouse
   - void solveMaze() : Includes all the algorithms and methods to solve the maze for the project, including checking walls, checking left, front, and right, updating the distance values for the map, and checking directions where the micromouse is headed
   - void powerMotors(int powers) : powers motor
   - void steps(int numsteps) : methods for steps
   - void Forward() : move forward
   - void Left() : move left
   - void Right() : move right
   - void Turn() : Turn 180 degrees
   - main() : It runs the solveMaze() with polling the sensors until the maze find  the destination

   ```

2. Event.c
   ```
   - void PID_OnInterrupt() : It implements the PID functions
   - void MotorR_DriveTimer_OnInterrupt() : Driver timer for right motor
   - void MotorL_DriveTimer_OnInterrupt() : Driver timer for left motor
   - void MotorR_DutyTimer_OnInterrupt() : Duty timer for right motor
   - void MotorL_DutyTimer_OnInterrupt() : Duty timer for left motor
   ```

## Key Function Descriptions

The micromouse implements the algorithm where it finds the shortest distance left from the goal. The
distance array is initially defined in the code and the code modifies the array by adding 4 to the place it
went. This prevents the micromouse from going back in the path it already went. The micromouse
updates wall values and uses the shortest distance available until it finds the goal.

Functions that control the motors, sensors, timers, and interrupts are all required in order to
implementate this algorithm. These auxiliary functions are used within the solve maze method
algorithm explained above.

### Motor Control

Controlling motors in this project was very important for this project because it is the base
function that needs to be used while solving the maze. In order to control the motor, functions
such as power Motors that turns on the power, the steps function that is used for moving the
micromouse forward, right, left, and turn around functions are used in the project.

```c

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
```

[code 1] powerMotors and steps method

```c
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

```

[code 2] Motor direction methods

### Sensors

pollSensors() is a crucial function for this project. It detects the distance of the wall that allows
the mouse to know where the walls are located in the maze. This is important so the
micromouse does not collide with the wall and finds the right direction.

```c
void pollSensors(){
    Sensor_Measure(1);
    Sensor_GetValue8(sensors);
}
```

[code 3] pollSensors

### Timer and Interrupt

In event.c, timers for motor such as left and right motor drive timer and duty timer are also
used.
When a timer interrupt occurs, MotorL_DriveTimer_OnInterrupt and
motorR_DriveTimer_OnInterrupt are called only when the interrupt event is enabled

```c
void MotorR_DriveTimer_OnInterrupt(void)
{
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
```

[Code4] DriveTimer_OnInterrupt

When a timer interrupt occurs, dutytimer_oninterrupt is called only if the interrupt event is
enabled.

```c
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

```

[Code5]DutyTimer_OnInterrupt

### PID Interrupt

PID interrupt helps the mouse to not collide with the wall by having timer interrupts when the
this interrupt is called.

```c
void PID_OnInterrupt(void)
{
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
```

[Code6] PID_OnInterrupt

### Check Walls

After changing the micromouse orientation value relative to the north, it checks for the wall.
Below is checking left walls. If the wall is detected, it modifies the distance to the goal
accordingly and updates the array that contains wall values stored in the map. This is implemented for front and right as well.

```c
//Check Walls
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
```

[Code7] Check Left

### Making Decisions for next step

The algorithm first checks where the direction is then check where the micromouse is physical
heading. For example, if it is moving east and pointing north, it will turn right and go forward to
move east. Else if it is already heading east, it will go forward and the same for other directions.

```c
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
```

[Code8] move east

## Testing

Testing for this project first required many debugging sessions to fix the errors with the code. After the
code ran without problem, while observing how the micromouse worked in the maze, many
optimizations had to occur. PID functions were very inconsistent during the first implementation, the
micromouse collided with the wall. This issue was fixed by implementing integration into the control
scheme. Another behavior that had to be troubleshooted was where it first stuck in an infinite loop of
going back and forth between the goals but by including the position of the goal in the main method the
problem was solved.

## Results

Micromouse solved the maze correctly. The result code is included in this repository.

## Conclusion

With this lab, the group learned about creating an algorithm that solves a maze with a micromouse
where the group can implement sensors, motors, and interrupts. With implementing the algorithm, we
could think deeply about how the motors had to be moved in order to make the correct decision with
pid values and recognizing walls with the sensors. Having more working mice, or assigning a mouse per
each group could help with the project in the future. We issues that were the result of switching back
and forth between mice.

# Improvements

After I took education via Samsung Software Academy for Youth, I decided to tune up the codes that always bothered me. Although I could not test the code in real life below modification enhances code quality and liability.

## Original Code :

### Global Variables

```C
int heading[4] = {1, 0, 0, 0};
int mouseX = 0;
int mouseY = 0;

// Final Project Global Material
int maze[6][13][4] =
    {
        {{0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}},
        {{0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}},
};
int place;

int mazeDist[6][13] =
    {
        {8, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8},
        {7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7},
        {6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6},
        {7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7},
        {8, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8},
};
```

### Algorithms

```C
// Final Project Methods
void solveMaze()
{
    int north = -1;
    int south = -1;
    int east = -1;
    int west = -1;
    // Larger for distance selection
    int compare = 1000000;
    int direction[4] = {0};

    // Just Testing
    // Forward();
    // Testing

    pollSensors();

    // Cpu_Delay100US(750);

    // Check Walls

    // modifies orientation value relative to north

    place = -1;

    if (heading[0] == 1)
    {

        place = 0;
    }
    if (heading[1] == 1)
    {

        // what it was originaly // place = 1;
        place = 1;
    }
    if (heading[2] == 1)
    {

        // what it was originaly //place = 2;
        place = 2;
    }
    else if (heading[3] == 1)
    {
        // was three intially
        place = 3;
    }

    // check left
    if (sensors[0] > 80)
    {

        int placeLeft = place;
        placeLeft = place - 1;

        if (placeLeft < 0)
        {
            // was 4 I think this would have caused array index outofbounds
            placeLeft = 3;
        }

        maze[mouseX][mouseY][placeLeft] = 1;
    }

    // Testing so that walls are never generated

    // check front

    if (sensors[1] > 90)
    {
        int placeFront = place;

        maze[mouseX][mouseY][placeFront] = 1;
    }

    // check right

    // Had sensor value wrong

    if (sensors[2] > 80)
    {

        int placeRight = place;
        placeRight = place + 1;

        if (placeRight > 3)
        {
            placeRight = 0;
        }

        maze[mouseX][mouseY][placeRight] = 1;
    }

    // Update the array

    // Add two to the current position

    mazeDist[mouseX][mouseY] = ((mazeDist[mouseX][mouseY]) + 4);

    // Case Dead End
    // Broke this by makeing -3
    if (((maze[mouseX][mouseY][0]) + (maze[mouseX][mouseY][1]) + (maze[mouseX][mouseY][2]) + (maze[mouseX][mouseY][3])) == 5)
    {
        // Now check heading in order to determine which value to moddify the value of the current square to 1 more than the square behind

        // Testing

        // If robot is pointing North
        if (heading[0] == 1)
        {
            mazeDist[mouseX][mouseY] = ((mazeDist[mouseX][mouseY - 1]) + 4);
        }

        // If robot is pointing East
        if (heading[1] == 1)
        {
            mazeDist[mouseX][mouseY] = ((mazeDist[mouseX - 1][mouseY]) + 4);
        }

        // If robot is pointing South
        if (heading[2] == 1)
        {
            mazeDist[mouseX][mouseY] = ((mazeDist[mouseX][mouseY + 1]) + 4);
        }

        // If robot is pointing West
        if (heading[3] == 1)
        {
            mazeDist[mouseX][mouseY] = ((mazeDist[mouseX + 1][mouseY]) + 4);
        }
    }

    // Removed Case two trying my modified wallflower

    // move closest square with an emphasis on going left
    // Make sure the ifs are checking the correct statement
    // dosent implement left prefference

    direction[0] = 0;
    direction[1] = 0;
    direction[2] = 0;
    direction[3] = 0;

    // check north
    if (maze[mouseX][mouseY][0] == 0)
    {

        north = mazeDist[mouseX][mouseY + 1];

        // Testing
        // Forward();

        // Used for best path slection
        if (north < compare)
        {
            compare = north;
            direction[0] = 1;
            direction[1] = 0;
            direction[2] = 0;
            direction[3] = 0;
        }
    }

    // check south
    // Add conditional to prevent index out of bounds
    if (maze[mouseX][mouseY][2] == 0)
    {

        south = mazeDist[mouseX][mouseY - 1];

        // used for best path selection
        if (south < compare)
        {
            compare = south;
            direction[1] = 0;
            direction[0] = 0;
            direction[2] = 1;
            direction[3] = 0;
        }
    }

    // check east

    if (maze[mouseX][mouseY][1] == 0)
    {

        east = mazeDist[mouseX + 1][mouseY];

        // used for best path selection
        if (east < compare)
        {
            compare = east;
            direction[0] = 0;
            direction[2] = 0;
            direction[1] = 1;
            direction[3] = 0;
        }
    }

    // check west

    if (maze[mouseX][mouseY][3] == 0)
    {

        west = mazeDist[mouseX - 1][mouseY];

        // used for best path selection
        if (west < compare)
        {
            compare = west;
            direction[2] = 0;
            direction[1] = 0;
            direction[3] = 1;
            direction[0] = 0;
        }
    }

    // Find which is smallest Already done

    // Testing

    // direction[2] = 1;

    Cpu_Delay100US(750);

    // if want to move north move
    if (direction[0] == 1)
    {
        // if already pointing north
        if (heading[0] == 1)
        {
            Forward();
        }
        else if (heading[1] == 1)
        {
            // if pointing east

            Left();
            Forward();
        }
        else if (heading[2] == 1)
        {
            // if pointing south

            Turn();
            Forward();
        }
        else
        {
            // if pointing west

            Right();
            Forward();
        }

        mouseY++;

        // Need to modify the heading array
        heading[0] = 1;
        heading[1] = 0;
        heading[2] = 0;
        heading[3] = 0;
    }

    // Testing
    // heading[0] = 1;
    // direction[1] = 1;

    // if east move
    if (direction[1] == 1)
    {
        // if pointing north
        if (heading[0] == 1)
        {
            Right();
            Forward();
        }
        else if (heading[1] == 1)
        {
            // if already pointing east
            Forward();
        }
        else if (heading[2] == 1)
        {
            // if pointing south
            Left();
            Forward();
        }
        else
        {
            // if pointing west
            Turn();
            Forward();
        }

        mouseX++;
        // Need to modify the heading array
        heading[0] = 0;
        heading[1] = 1;
        heading[2] = 0;
        heading[3] = 0;
    }
    // if south move
    if (direction[2] == 1)
    {
        // if pointing north
        if (heading[0] == 1)
        {
            Turn();
            Forward();
        }
        else if (heading[1] == 1)
        {
            // if pointing east
            Right();
            Forward();
        }
        else if (heading[2] == 1)
        {
            // if already pointing south
            Forward();
        }
        else
        {
            // if pointing west
            Left();
            Forward();
        }

        mouseY--;

        // Need to modify the heading array
        heading[0] = 0;
        heading[1] = 0;
        heading[2] = 1;
        heading[3] = 0;
    }

    // if west move
    if (direction[3] == 1)
    {
        // if pointing north
        if (heading[0] == 1)
        {
            Left();
            Forward();
        }
        else if (heading[1] == 1)
        {
            // if pointing east
            Turn();
            Forward();
        }
        else if (heading[2] == 1)
        {
            // if pointing south
            Right();
            Forward();
        }
        else
        {
            // if already pointing west
            Forward();
        }

        // Need to modify the heading array
        heading[0] = 0;
        heading[1] = 0;
        heading[2] = 0;
        heading[3] = 1;

        mouseX--;
        // End
    }
    Cpu_Delay100US(1000);
    return;
}

```

## Ver2.0

### Global Variable

```C
int mouseX = 0;
int mouseY = 0;

// Final Project Global Material
int map[6][13] = {{0}};
int dx[4] = {-1, 1, 0, 0}; //동서남북
int dy[4] = {0, 0, -1, 1};
```

### Algorithms

```C
// Final Project Methods
int nx, ny;
void solveMaze()
{
    pollSensors();

    //도착할 때 까지
    while ((mouseX != 3 && mouseY != 6) || (mouseX != = 4 && mouseY != 6))
    {
        ///사방 탐색
        for (int d = 0; d < 4; d++)
        {
            nx = dx[d] + mouseX; // next direction
            ny = dy[d] + mouseY;
            /* array out of index */
            if (nx < 0 || ny < 0 || nx >= 6 || ny >= 13)
            {
                map[mouseX][mouseY] = -1; //현재 위치에서 할수있는게 없음
                Turn();                   //다시 뒤로간다
                Forward();                //이전으로 돌아감
                continue;
            }
            /* dead end */
            else if (sensors[0] > 80 && sensors[1] > 90 && sensors[2] > 80)
            {
                map[mouseX][mouseY] = -1; //현재 위치에서 할수있는게 없음
                Turn();
                Forward();
                continue;
            }
            /* 한군데 이상 갈 수 있다. : 동 서 남 북 순으로 traverse하고 옮길 때 마다 1더해진다.*/
            else
            {
                //왼쪽과 앞에 벽이라면오른쪽으로 회전
                if (sensors[0] > 80 && sensors[1] > 90) Right();
                //왼쪽과 오른쪽이 벽 -> 회전할 필요 x
                else if (sensors[0] > 80 && sensors[2] > 80){}
                //앞쪽과 오른쪽이 벽 -> 왼쪽으로 회전
                else if (sensors[1] > 90 && sensors[2] > 80) Left();
                //벽이 한쪽밖에 없거나 아예없으면 벽이 없는쪽 아무대나 괜찮다
                else
                {
                    //벽이 없으면 break -> i가 1 2 3인경우로 가면 된다.
                    int i;
                    for (i = 0; i < 3; i++)
                    {
                        if (sensors[i] > 80)break;
                    }
                    if (i == 1)Left();
                    else if (i == 2){}
                    else Right();
                }
                // mouseX와 mouseY의 위치에 1을 더했다 (왔다갔다는 뜻)
                map[mouseX][mouseY]++;
                // 직진한다.
                Forward();
                Cpu_Delay100US(750);
            }
        }
    }
    return;
}
```

### Why Did I make Improvements ?

1. COL : 308 Lines -> 69 Lines
2. 3 Arrays (heading[4], map[6][13][4], mazeDist[6][13]) -> 3 Arrays(dx[4],dy[4],map[6][13])

Instead of saving headings, directions encountered in map, and distance map,

1. I used dx and dy to search East, West, North, and South

   So I was able to remove heading and simplify map

2. If robot encounters dead ends or wall, update map to -1 else add distance

   In this way, I can remove mazeDistance array.
