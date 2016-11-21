#ifndef PID_h
#define PID_h

class PID {
public:
  double error;
  double sample;
  double lastSample;
  double kp, ki, kd;      
  double p, i, d;
  double pid;
  double setPoint;
  long lastProcess;
  
  PID(double kp, double ki, double kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
  }
  
  void addNewSample(double sample){
    this->sample = sample;
  }
  
  void setSetPoint(double setPoint){
    this->setPoint = setPoint;
  }
  
  double process(){
    error = setPoint - sample;
    p = error * kp;
    i = i + (error * ki);
    d = (sample - lastSample) * kd ;
    lastSample = sample;
    pid = p + i + d;
    return pid;
   }
};

#endif
