/*
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:


int led = 13;
const int MAXCOMMAND = 50;
char commandStr[MAXCOMMAND];
char* commandParams;
int commandchar = 0;
int command = 0;
boolean lighton = true;
int newspeed = 50;
int curspeed = 50;
int newsteer = 90;
int cursteer = 90;

#include "printf_helper.h"
#include <Servo.h>

Servo steercontrol;  // create servo object to control a servo
Servo motorcontrol;  // create servo object to control a servo
                // a maximum of eight servo objects can be created

void arm(){
  // arm the speed controller, modify as necessary for your ESC
  p("Calibrate %d!\n", 100);
  setSpeed(100);
  delay(2000);
  p("Calibrate %d!\n", 0);
  setSpeed(0);
  delay(2000);
  setSpeed(50);
  //delay(2000);
}

void setSpeed(int speed){
  // speed is from 0 to 100 where 0 is off and 100 is maximum speed
  //the following maps speed values of 0-100 to angles from 0-180,
  // some speed controllers may need different values, see the ESC instructions
  int angle = map(speed, 0, 100, 0, 180);
  motorcontrol.write(angle);
}

void setDirection(int angle){
  // angle 90 is straight
  steercontrol.write(angle);
}


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  motorcontrol.attach(9);  // attaches the servo on pin 9 to the servo object
  steercontrol.attach(10);  // attaches the servo on pin 10 to the servo object
  arm();
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  p("Setup done\n");
}


int parse_command(char* command) {
  if (strstr(command, "light") != NULL) {
    commandParams = &(command[6]);
    return 1;
  }
  else if (strstr(command, "speed") != NULL) {
    commandParams = &(command[6]);
    return 2;
  }
  else if (strstr(command, "steer") != NULL) {
    commandParams = &(command[6]);
    return 3;
  }
  else {
    return -1;
  }
}
int read_command() {
  if (Serial.available() > 0) {
    // read the incoming bytes
    for (int i=0; i<Serial.available(); i++)
    {
      commandStr[commandchar] = Serial.read();
      // skip \r
      if (commandStr[commandchar] == 10) {
        commandchar = 0;
        break;        
      }
      // end command on \n
      if (commandStr[commandchar] == 13) {
        commandStr[commandchar] = 0;
        // say what you got:
        debug(1, "I received: %s\n", commandStr);
        commandchar = 0;
        return parse_command(commandStr);
      }
      commandchar++;
    }
  }
  return 0;
}

// the loop routine runs over and over again forever:
void loop() {
  debug_level = 1;

  command = read_command();
  if (command > 0) {
    p("Got command %d param: %s!\n", command, commandParams);
    switch (command) {
      case 1:
        if (strcmp(commandParams, "on") == 0)
          lighton = true;
        else if (strcmp(commandParams, "off") == 0)
          lighton = false;
        debug(1, "Setting light %d\n", lighton);
        break;
      case 2:
        newspeed = atoi(commandParams);
        debug(1, "Setting speed %d\n", newspeed);
        break;
      case 3:
        newsteer = atoi(commandParams);
        debug(1, "Setting steer %d\n", newsteer);
        break;
    }      
  }
  if (newspeed != curspeed) {
    setSpeed(newspeed);
    curspeed = newspeed;
  }
  if (newsteer != cursteer) {
    setDirection(newsteer);
    cursteer = newsteer;
  }
  if (lighton) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  }
  delay(100);               // wait for a convenient time
}
