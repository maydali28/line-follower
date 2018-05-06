#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <Arduino.h>

#define SENSOR_THRESHOLD 600
#define NO_LINE_THRESHOLD 50

//template <byte s0, byte s1, byte s2, byte s3, byte s4, byte s5 >
class Sensors{
    public:

        
        /*
        * configure
        * configure and init sensors
        */ 
        void configure(byte * sensor,byte size){
            for(int i =0;i< size;i++)
                pinMode(sensor[i],INPUT);
            /*pinMode(s1,INPUT);
            pinMode(s2,INPUT);
            pinMode(s3,INPUT);
            pinMode(s4,INPUT);
            pinMode(s5,INPUT);
            */
        };

        /*
        * updateSensors 
        * update Error value
        * return calculated error 
        */
        int updateError(byte * sensor,byte size){
            int iRead = 0;
            int iActive = 0;
            
            readLine(sensor,size);
            delay(1);
            printSensors();

            if(Sensors::not_above_line(sensor_value[0]) && Sensors::above_line(sensor_value[5]))
                  isTurnRequired = true;
            else if(Sensors::not_above_line(sensor_value[5]) && Sensors::above_line(sensor_value[0]))
                  isTurnRequired = true;
            for(int i=0; i<6; i++) {        
              if(Sensors::not_above_line(sensor_value[i])) {
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
            Serial.print(sensor_value[4]);
            Serial.print("\t");
            Serial.println(sensor_value[5]);
        };
        
        void readLine(byte * sensor,byte size){
          for(int i =0;i< size;i++)
              sensor_value[i] = analogRead(sensor[i]);
          /*
            sensor_value[0] = analogRead(s0);
            sensor_value[1] = analogRead(s1);
            sensor_value[2] = analogRead(s2);
            sensor_value[3] = analogRead(s3);
            sensor_value[4] = analogRead(s4);
            sensor_value[5] = analogRead(s5);
            */
        };    

        void switchSensors(){
          if(limitMin == SENSOR_THRESHOLD)
              limitMin = NO_LINE_THRESHOLD;
          else if(limitMax == NO_LINE_THRESHOLD)
              limitMin = SENSOR_THRESHOLD;
        };

        byte sensor_value[6] = {0,0,0,0,0,0};
        volatile int error;
        volatile bool isTurnRequired;
        int stable = map(3500,0,6000,0,1023); 
        int iLastRead;

        int limitMin = SENSOR_THRESHOLD, limitMax = NO_LINE_THRESHOLD;
        
        static bool above_line(byte sensor) { return sensor > SENSOR_THRESHOLD; };
        static bool not_above_line(byte  sensor) { return sensor > NO_LINE_THRESHOLD; };          
};

#endif
