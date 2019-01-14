#include <math.h> 

#define HALF_DEADBAND 3 //adjust till desired deadband is achieved
#define XRAW_CENTERED 525 // set to whatever "x joystick value is: " on the serial monitor
#define YRAW_CENTERED 493 // set to whatever "y joystick value is: " on the serial monitor
#define DEBUG true // set to false to stop serial messages

// 2 is right motors 1 is left motors
int PinPWM_b1 = 11;   // ch1 3
int PinPWM_b2 = 10;   //ch2 5
int PinPWM_y1 = 5;   //ch3 10
int PinPWM_y2 = 3;  //ch4 11
int PinRot_g1 = 12;   //ch1 4
int PinRot_g2 = 8;   //ch2 7
int PinRot_r1 = 7;    //ch3 8 
int PinRot_r2 = 4;   //ch4 12

int throttle, steering;

void setup()
{ 
  if (DEBUG)              
  Serial.begin(38400);
  
  yraw = YRAW_CENTERED;
  xraw = XRAW_CENTERED;
  
  pinMode(PinPWM_b1,OUTPUT);    // back left motor pwm
  pinMode(PinPWM_b2,OUTPUT);   // back right motor pwm
  pinMode(PinPWM_y1,OUTPUT);   // front left motor pwm
  pinMode(PinPWM_y2,OUTPUT);    // front right motor pwm
  
  pinMode(PinRot_g1,OUTPUT);     // back left motor dir
  pinMode(PinRot_g2,OUTPUT);     // back right motor dir
  pinMode(PinRot_r1,OUTPUT);     // front left motor dir
  pinMode(PinRot_r2,OUTPUT);     // front right motor dir
}

void loop()                     
{ 
    CalculateSpeed();
    DriveMotors();
}

//------------------------------------------
//Drive Controller (Move to own class)
//------------------------------------------
void CalculateSpeed()
{
   
}

//------------------------------------------
//Motor Controller (Move to own class)
//------------------------------------------
void DriveMotors()
{
      //write direction pin
      if (leftSpd < 0)
      {
        digitalWrite(PinRot_g1,LOW);
        digitalWrite(PinRot_r1,LOW); 
      }
      else if (leftSpd > 0)
      {
        digitalWrite(PinRot_g1,HIGH);
        digitalWrite(PinRot_r1,HIGH); 
      } 
      if (rightSpd < 0)
      {
        digitalWrite(PinRot_r2,LOW);
        digitalWrite(PinRot_g2,LOW);  
      }
      else if (rightSpd > 0)
      {
        digitalWrite(PinRot_r2,HIGH);
        digitalWrite(PinRot_g2,HIGH);  
      }
      
      analogWrite(PinPWM_b1,abs(leftSpd));  
      analogWrite(PinPWM_b2,abs(rightSpd));
      analogWrite(PinPWM_y1,abs(leftSpd));
      analogWrite(PinPWM_y2,abs(rightSpd));
}
