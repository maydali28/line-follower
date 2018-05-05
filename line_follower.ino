#include "sensors.h"
#include "DCMotors.h"


Sensors<A0,A1,A2,A3,A4,A5> sensors;
DCMotors<10,18,19,11,14,15> motors;

//float Kp=7.5, Ki=5 ,Kd=0.08;
float Kp=15, Ki=0.00015 ,Kd=10;

float P=0, D=0;
volatile float PID_value=0;
volatile float I=0;
float error=0;
float previous_error=0, previous_I=0;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  sensors.configure();
  motors.configure();
  invalide();
}

void loop() {
  // put your main code here, to run repeatedly:
  error = sensors.updateError();
  calculate_PID(error);
  Serial.print("error \t");
  Serial.println(error);
  
  Serial.print("PID \t");
  Serial.println((int)PID_value);
  
  /*
  motors.turn(error);
          
  motors.drive((int)PID_value);
  */
  delay(100);
}

void invalide(){
  sensors.error = 0;
  PID_value = 0;
  I = 0;
}

void calculate_PID(int8_t error){
    Serial.print("\t");
    Serial.println(error);
    
    P = error;
    //I = I + error*0.001;
    I = I + error;
    //D = (error - previous_error)/.001;
    D = error - previous_error;
    
    PID_value = (Kp*P) + (Ki*I) + (Kd*D);

    previous_error = error;
}

