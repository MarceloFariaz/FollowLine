#ifndef LineSensor_h
#define LineSensor_h

#include "Arduino.h"

class LineSensor {
  private:
    byte n_sensors;
    byte *pins;
    unsigned int *sensors_values;
  
  public:
    // Constructor
    LineSensor(byte *pins, byte n_sensors){
      this->n_sensors = n_sensors;

      // Alloc the memory for pins array
      if (this->pins == 0) {
        this->pins = (byte*)malloc(sizeof(byte)*this->n_sensors);
        if (this->pins == 0)
          return;
      }
      
      // Alloc the memory for pins array
      this->sensors_values = (unsigned int*)malloc(sizeof(unsigned int)*this->n_sensors);
      if (this->sensors_values == 0)
        return;

      // Save the pins
      for (int i = 0; i < this->n_sensors; i++)
        this->pins[i] = pins[i];
      
      // Set up the pins
      for (int i = 0; i < this->n_sensors; i++){
        pinMode(this->pins[i], INPUT);        
      }
    }
    
    // Read sensors and update values
    void readSensors() {
      // Read analog chanels (Sensors)
      for (int i = 0; i < n_sensors; i++){
        // Read Sensor
        int reading = analogRead(i+1);
        // Constrains the value to be within a range of [0, 1000].
        sensors_values[i] = reading;//constrain(reading, 0, 1000);
      }
    }
    
    unsigned long getLine(bool white_line){
      unsigned char i, on_line = 0;
      unsigned long avg = 0; // this is for the weighted total, which is long
                         // before division
      unsigned int sum = 0; // this is for the denominator which is <= 64000
      static int last_value = 0;
      
      readSensors();
    
      for(i = 0; i < n_sensors; i++) {
        
          int value = sensors_values[i];
          
          // Adjust the value for a white line
          if(white_line)
              value = 1000 - value;
      
          // Keep track of whether we see the line at all
          if(value > 200) {
              on_line = 1;
          }
      
          // Only average in values that are above a noise threshold
          if(value > 50) {
              avg += (long)(value) * (i * 1000);
              sum += value;
          }
      }
      
      if(! on_line) {
          // If it last read to the left of center, return 0.
          if(last_value < (n_sensors - 1) * 1000 / 2)
              return 0;
      
          // If it last read to the right of center, return the max.
          else
              return (n_sensors - 1) * 1000;
      
      }
      
      last_value = avg/sum;
      
      return last_value;
    }
    
};

#endif
