#include "sensors.h"
#include "DCMotors.h"

Sensors sensors;

byte s[6] = {A0,A1,A2,A3,A4,A5};

DCMotors<10,18,19,11,14,15> motors;

//float Kp=7.5, Ki=5 ,Kd=0.08;
float Kp=15, Ki=0.00015 ,Kd=10;

float P=0, D=0;
volatile float PID_value=0;
volatile float I=0;

float error=0;
float previous_error=0, previous_I=0;

bool isInProcess;

byte state = 'S';
byte last_state = 'S';
byte detect = 'N';
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  sensors.configure(s,6);
  motors.configure();
  invalide();
  isInProcess = false;
}

void loop() {
  // put your main code here, to run repeatedly:

  switch(state){
    case 'S':
      Serial.println("stopped");
      if(detect == 'G') {
        Serial.println("Detect Object");
        state = 'F';
        detect = 'N';
      }
      if(last_state == 'S') isInProcess = false;
      break;
    case 'F':
        Serial.println("Forward"); 
        isInProcess = false;
        if(detect == 'G'){
          Serial.println("Detect Object");
        }
        else {
          if(state == 'S'){
              motors.stopMoving();
              invalide();
          }
        }
        last_state = 'F';
      break;
    case 'R':
      Serial.println("Right");
      if(last_state == 'F'){
          isInProcess = true;
          turn('R');
      }
      if(last_state == 'R')
          isInProcess = true;
      last_state = 'R';
      break;
    case 'L':
    Serial.println("Left");
      if(last_state == 'F'){
          isInProcess = true;
          turn('F');
      }
      if(last_state == 'L')
          isInProcess = true;
      last_state = 'L';
      break;
    case 'W':
    Serial.println("Switch");
      sensors.switchSensors();
      isInProcess = false;
      break;
    default:
      break;     
  }

  
  if(isInProcess){
    return;
  }
  
  error = sensors.updateError(s,6);
  calculate_PID(error);
  
  Serial.print("error \t");
  Serial.println(error);
  
  Serial.print("PID \t");
  Serial.println((int)PID_value);
  
  if(sensors.isTurnRequired){
      Serial.println("detect intersection");
      state = intersection();
      isInProcess = true;  
  }else
      motors.drive((int)PID_value);
  delay(1000);
}

void invalide(){
  sensors.error = 0;
  PID_value = 0;
  I = 0;
  state = 'S';
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
    unsigned long millis_start;
byte intersection(){
    
    bool found_left = 0;
    bool found_straight = 0;
    bool found_right = 0;
    
    bool timeout = false;
    bool changed = false;    
    
    sensors.readLine(s,6);
    
    // Check for left and right exits.
    if (sensors.above_line(sensors.sensor_value[0]))
    found_left = 1;
    if (sensors.above_line(sensors.sensor_value[5]))
    found_right = 1;

    millis_start = millis();
    
    while(!changed || ! timeout){
        motors.drive(motors.speedConst / 2);
        sensors.readLine(s,6);
        if(found_left && sensors.not_above_line(sensors.sensor_value[0])){
          found_left = 0;
          changed = true; 
        }
        if(found_right && sensors.not_above_line(sensors.sensor_value[5])){
          found_right = 0;
          changed = true; 
        }
        if(50 > (millis()-millis_start)) timeout = true;
        delay(1);
    }
    
    if (sensors.above_line(sensors.sensor_value[0]))
    found_left = 1;
    if (sensors.above_line(sensors.sensor_value[5]))
    found_right = 1;    
    
    if (timeout && (sensors.above_line(sensors.sensor_value[1]) || sensors.above_line(sensors.sensor_value[2]) ||
    sensors.above_line(sensors.sensor_value[3]) || sensors.above_line(sensors.sensor_value[4]))){
      found_straight = 1;
      timeout = false;
    }
    
    byte intersectionType = 'F';
    
    if (!found_left && !found_right && !found_straight) {
    // deadend
    intersectionType = 'S';
    }
    
    else if (found_left && !found_right && !found_straight) {
    // left
    intersectionType = 'L';
    }
    
    else if (!found_left && found_right && !found_straight) {
    // right
    intersectionType = 'R';
    }
    
    else if (found_left && !found_right && found_straight) {
    // straight left
    intersectionType = 'F';
    }
    
    else if (!found_left && found_right && found_straight) {
    // straight right
    intersectionType = 'F';
    }
    
    else if (found_left && found_right && !found_straight) {
    // T junction or switch colors
    intersectionType = 'W';
    }
    /* TODO */
    else if (found_left && found_right && found_straight) {
    // T junction
    intersectionType = 7;
    }
    
    return intersectionType;
}

void turn(byte dir){
  unsigned short count = 0;
  unsigned short last_status = 0;
  switch(dir){
    case 'L':
      motors.turn(1);  
      delay(1);
      /* complete turn */
      while(count < 2)
      {
        sensors.readLine(s,6);
        count += sensors.above_line(sensors.sensor_value[0]) ^ last_status;
        last_status = sensors.above_line(sensors.sensor_value[0]);
      }
      motors.turn(1);
      invalide();
      state = 'F';
      delay(1);
      motors.stopMoving();
    break;
    case 'R':
      motors.turn(-1);
      delay(1);
      /* complete turn */
      while(count < 2)
      {
        sensors.readLine(s,6);
        count += sensors.above_line(sensors.sensor_value[5]) ^ last_status;
        last_status = sensors.above_line(sensors.sensor_value[5]);
      }
      
      motors.turn(-1);
      invalide();
      state = 'F';
      delay(1);
      motors.stopMoving();
      break;
  }
}
