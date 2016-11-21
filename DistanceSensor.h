#ifndef DistanceSensor_h
#define DistanceSensor_h

#include "Arduino.h"

class DistanceSensor {
  private: 
    int pin_trig;
    int pin_echo;
    int distance_cm = 0;
    
  public:
    // Constructor
    DistanceSensor (int pin_trig, int pin_echo) {
      // Set up the pins 
      this->pin_trig = pin_trig;
      this->pin_echo = pin_echo;
      pinMode(pin_trig, OUTPUT);
      pinMode(pin_echo, INPUT);
    }
    
    // Read pulse and calculate the distance
    int getDistance(){
      // Clears the trig pin
      digitalWrite(pin_trig, LOW);
      delayMicroseconds(2);
      
      // Pulse trig pin per 10us
      digitalWrite(pin_trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(pin_trig, LOW);
      
      // Read pulse in echo pin
      unsigned long duration = pulseIn(pin_echo, HIGH);
      
      // Calculating the distance
      // Sound speed = 34300 cm/s
      // 1s = 1000000us
      distance_cm = ((duration * 34300.0) / 1000000.0) / 2;
      
      // Return distance value in cm
      return distance_cm;
    }
       
};

#endif
