#ifndef DCMOTORS_HPP_
#define DCMOTORS_HPP_

#include <Arduino.h>


template <byte enL, byte inpL1, byte inpL2, byte enR, byte inpR1, byte inpR2 >

class DCMotors{
    public:

        void configure(){
          pinMode(enL, OUTPUT);
          pinMode(enR, OUTPUT);
          pinMode(inpL1, OUTPUT);
          pinMode(inpL2, OUTPUT);
          pinMode(inpR1, OUTPUT);
          pinMode(inpR2, OUTPUT);
          stopMoving();
        };
        
        void drive(int cor){
          LMSpeed = speedConst - cor;
          RMSpeed = speedConst + cor;

          if(LMSpeed < 0) LMSpeed = 0;
          if(LMSpeed > 255) LMSpeed = 255;
          
          if(RMSpeed < 0) RMSpeed = 0;
          if(RMSpeed > 255) RMSpeed = 255;
                
          analogWrite(enL,LMSpeed);
          analogWrite(enR,RMSpeed);
          digitalWrite(inpL1,LOW);
          digitalWrite(inpL2,HIGH);
          digitalWrite(inpR1,HIGH);
          digitalWrite(inpR2,LOW);
        };

        void turn(int cor){
            stopMoving();
            delay(1);
            
            if(cor<0){
            /*
             * TODO
             * change speedConst-1
             */
            analogWrite(enL,speedTurn);
            analogWrite(enR,speedConst-1);
            digitalWrite(inpL1,HIGH);
            digitalWrite(inpL2,LOW);
            digitalWrite(inpR1,HIGH); //change
            digitalWrite(inpR2,LOW);
            
            }
            else if(cor>0){
            analogWrite(enL,speedConst-1);
            analogWrite(enR,speedTurn);
            digitalWrite(inpL1,LOW); // change
            digitalWrite(inpL2,HIGH);
            digitalWrite(inpR1,LOW);
            digitalWrite(inpR2,HIGH);
            }          
        };

        void stopMoving(){
          analogWrite(enL,0);
          analogWrite(enR,0);
          digitalWrite(inpL1,LOW);
          digitalWrite(inpL2,LOW);
          digitalWrite(inpR1,LOW);
          digitalWrite(inpR2,LOW);
        };

        byte speedConst = 56;
        byte speedTurn = 35;

        int LMSpeed, RMSpeed;
};

#endif
