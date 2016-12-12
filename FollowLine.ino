// #define AVOID_OBSTACLE

#include "DistanceSensor.h"
#include "LineSensor.h"
#include "Motors.h"
#include "PID.h"
#include "Communication.h"
#include <EEPROM.h>

#define LOG(x) if(false) Serial.print(x)

#ifdef AVOID_OBSTACLE
  DistanceSensor distanceSensor(2, 3);
#endif

LineSensor lineSensor((byte[]){A1, A2}, 2);
Motors motors(5, 7, HIGH, 6, 8, HIGH); 
PID pid(0.1300, 0.0000, 0.0700); //(P, I, D)

int max_speed = 95; // Set max speed for the motors

void setup() {
  
  // Set up the Serial and print a welcome message
  Serial.begin(9600);
  LOG("Follow Line Connected!\n");
  
  // Stop the motors
  motors.setSpeed(0, 0);

  // Set up the set point to PID controller
  // 500 because it's the center line value
  pid.setSetPoint(500);
  
}

void loop() {
  
#ifdef AVOID_OBSTACLE
  // Distance
  int distance_cm = distanceSensor.getDistance(); // Get distance from ultrasonic sensor
  LOG(distance_cm); LOG("\t"); // Log the distance value
  if (distance_cm < 5) { // Check if distance is less than 5 cm
    delay(100); // Debounce
    if (distance_cm < 5){ // Check if distance is than 5 cm again
      motors.setSpeed(-50, -50);
      delay(100);
      motors.setSpeed(0, 0); // Stop motors
      LOG("Finished!\n");
      while(1); // Infinite loop
    }
  }
#endif

  // Line Position
  unsigned long line_position = lineSensor.getLine(true); // Get line position from line sensors
  LOG(line_position); LOG("\t"); // Log the line position value
  
  // PIC Controller
  pid.addNewSample(line_position); // Add a new sample for PID controller
  double pid_value = pid.process(); // Process PID and save the return
  LOG(pid_value); LOG("\t"); // Log the pid value

  // Speed Difference
  // Constrains the speed difference to be within a range of [-max_speed, max_speed]
  int speed_difference = constrain(pid_value, -max_speed, max_speed);
  LOG(speed_difference); LOG("\t"); // Log the speed difference value

  // Motors speeds
  int speed_left = speed_difference > 0 ? max_speed - speed_difference : max_speed; 
  int speed_right = speed_difference > 0 ? max_speed : max_speed + speed_difference;
  motors.setSpeed(speed_left, speed_right); // Set the motors speeds
  LOG(speed_left); LOG("\t"); // Log the left speed value
  LOG(speed_right); LOG("\t"); // Log the right speed value
  
  LOG("\n"); // Next line
  
}

void serialEvent() {
  char data = Serial.read();

  switch(data) {
    case 'A':
        pid.setKp(pid.getKp()+0.005);
      break;
      
    case 'B':
        pid.setKi(pid.getKi()+0.0005);
      break;
      
    case 'C':
        pid.setKd(pid.getKd()+0.005);
      break;
      
    case 'D':
        pid.setKp(pid.getKp()-0.005);
      break;
      
    case 'E':
        pid.setKi(pid.getKi()-0.0005);
      break;
      
    case 'F':
        pid.setKd(pid.getKd()-0.005);
      break;

    case 'G':
        max_speed+=1;
      break;
      
    case 'H':
        max_speed-=1;
      break;
      
    case 'I': 
        
      break;
  }
  printGains();
  data = 0;
}

void printGains(){
  Serial.print(pid.getKp(), 4); Serial.print("\t");
  Serial.print(pid.getKi(), 4); Serial.print("\t");
  Serial.print(pid.getKd(), 4); Serial.print("\t");
  Serial.print(max_speed); Serial.print("\t");
  Serial.println();
}
