/*#include "sensors.h"




void Sensors::configure(){
    pinMode(s0,INPUT);
    pinMode(s1,INPUT);
    pinMode(s2,INPUT);
    pinMode(s3,INPUT);
    pinMode(s4,INPUT);
}

void Sensors::calibrate(){

}

float Sensors::updateError(){
        read();
            
}

void Sensors::printSensor(){
    Serial.print(sensor_value[0]);
    Serial.print("\t");
    Serial.print(sensor_value[1]);
    Serial.print("\t");
    Serial.print(sensor_value[2]);
    Serial.print("\t");
    Serial.print(sensor_value[3]);
    Serial.print("\t");
    Serial.println(sensor_value[4]);
}

void Sensors::read(){
    sensor_value[0] = analogRead(s0);
    sensor_value[1] = analogRead(s1);
    sensor_value[2] = analogRead(s2);
    sensor_value[3] = analogRead(s3);
    sensor_value[4] = analogRead(s4);
}


*/