#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <Arduino.h>

template <byte s0, byte s1, byte s2, byte s3, byte s4, byte s5 >

class Sensors{
    public:

        /*
        * configure
        * configure and init sensors
        */ 
        void configure(){
            pinMode(s0,INPUT);
            pinMode(s1,INPUT);
            pinMode(s2,INPUT);
            pinMode(s3,INPUT);
            pinMode(s4,INPUT);
            pinMode(s5,INPUT);
        };

        /*
        * updateSensors 
        * update Error value
        * return calculated error 
        */
        int updateError(){
            int iRead = 0;
            int iActive = 0;
            
            readValues();
            printSensors();
            
            for(int i=0; i<=5; i++) {        
              if(analogRead(sensor_value[i]) < min_threshold) {
                iRead += (i+1)*1000;
                iActive++;
              }
            }

            iRead = map(iRead/iActive, 0, 6000, 0, 1023);
            if(!iRead)
                error = iLastRead - stable;
            else {
                iLastRead = iRead;
                error = iRead - stable;
            }
            return error;            
        };
        /* TODO */
        /*
        * calibaret
        * init sensors
        */
        void calibrate();
        
        /*
        * Print
        * Print value of sensors
        */ 
        void printSensors(){
            Serial.print(sensor_value[0]);
            Serial.print("\t");
            Serial.print(sensor_value[1]);
            Serial.print("\t");
            Serial.print(sensor_value[2]);
            Serial.print("\t");
            Serial.print(sensor_value[3]);
            Serial.print("\t");
            Serial.println(sensor_value[4]);
            Serial.print("\t");
            Serial.println(sensor_value[5]);
        };
        
        void readValues(){
            sensor_value[0] = analogRead(s0);
            sensor_value[1] = analogRead(s1);
            sensor_value[2] = analogRead(s2);
            sensor_value[3] = analogRead(s3);
            sensor_value[4] = analogRead(s4);
            sensor_value[4] = analogRead(s5);
        };    
        
        byte sensor_value[6] = {0,0,0,0,0};
        
        volatile int error;
        
        int8_t max_threshold = 800;
        int8_t min_threshold = 120;
        
        volatile bool turn;
        
        int stable = map(3500,0,6000,0,1023); 
        int iLastRead;

};

#endif
