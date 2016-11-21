#ifndef Motors_h
#define Motors_h

#include "Arduino.h"

class Motors {
  private:
    int pin_pwm_left;
    int pin_dir_left;
    bool foward_left;
    int speed_left = 0;
    int pin_pwm_right;
    int pin_dir_right;
    bool foward_right;
    int speed_right = 0;
    
  public:
    Motors (int pin_pwm_left, int pin_dir_left, int foward_left, int pin_pwm_right, int pin_dir_right, int foward_right){
      // Set up the pins 
      this->foward_left = foward_left;
      this->foward_right = foward_right;
      this->pin_pwm_left = pin_pwm_left;
      this->pin_dir_left = pin_dir_left;
      this->pin_pwm_right = pin_pwm_right;
      this->pin_dir_right = pin_dir_right;
      pinMode(pin_pwm_left, OUTPUT);
      pinMode(pin_dir_left, OUTPUT);
      pinMode(pin_pwm_right, OUTPUT);
      pinMode(pin_dir_right, OUTPUT);
      // Stop motors
      setSpeed(0, 0);
    }
    
    void setSpeedE(int speed_left){
      this->speed_left = speed_left;
      // Constrains the speed to be within a range of [-100, 100]
      this->speed_left = constrain(this->speed_left, -100, 100); 
      
      // Map the range of [-100, 100] to a range of [-255, 255]
      this->speed_left = map(this->speed_left, -100, 100, -255, 255);
      
      // Set the direction of the motor
      digitalWrite(pin_dir_left, this->speed_left > 0 ? foward_left : ! foward_left);
      
      // Set the speed of the motor 
      analogWrite(pin_pwm_left, abs(this->speed_left));
    }
    
    
    void setSpeedD(int speed_right){
      this->speed_right = speed_right;
      // Constrains the speed to be within a range of [-100, 100].
      this->speed_right = constrain(this->speed_right, -100, 100); 
      
      // Map the range of [-100, 100] to a range of [-255, 255]
      this->speed_right = map(this->speed_right, -100, 100, -255, 255);
      
      // Set the direction of the motor
      digitalWrite(pin_dir_right, this->speed_right > 0 ? foward_right : ! foward_right);
      
      // Set the speed of the motor 
      analogWrite(pin_pwm_right, abs(this->speed_right));
    }
    
    void setSpeed(int speedE, int speedD){
      setSpeedE(speedE);
      setSpeedD(speedD);
    }
      
};

#endif
