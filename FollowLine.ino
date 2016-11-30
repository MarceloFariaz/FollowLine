// #define AVOID_OBSTACLE

#include "DistanceSensor.h"
#include "LineSensor.h"
#include "Motors.h"
#include "PID.h"

#define LOG(x) if(true) Serial.print(x)

#ifdef AVOID_OBSTACLE
  DistanceSensor distanceSensor(2, 3);
#endif

LineSensor lineSensor((byte[]){A1, A2}, 2);
Motors motors(5, 7, HIGH, 6, 8, HIGH); 
PID pid(0.175, 0.000, 0.1); //(P, I, D)

int max_speed = 100; // Set max speed for the motors

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
      motors.setSpeed(-50, -50); // Reverse the both motors for fast stop
      delay(100); // Wait a time for action
      motors.setSpeed(0, 0); // Stop motors
      LOG("Finished!\n");
      while(1); // Infinite loop
    }
  }
#endif

  // Line Position
  unsigned long line_position = lineSensor.getLine(false); // Get line position from line sensors
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





