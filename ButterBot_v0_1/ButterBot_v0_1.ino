#include <math.h> 
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

#define DEBUG true // set to false to stop serial messages

int servo_Angle[] = {60,60,60,60,60,60,60};
int leftSpd, rightSpd;

// 2 is right motors 1 is left motors
int PinPWM_b1 = 11;   // ch1 3
int PinPWM_b2 = 10;   //ch2 5
int PinPWM_y1 = 5;   //ch3 10
int PinPWM_y2 = 3;  //ch4 11
int PinRot_g1 = 12;   //ch1 4
int PinRot_g2 = 8;   //ch2 7
int PinRot_r1 = 7;    //ch3 8 
int PinRot_r2 = 4;   //ch4 12


void setup()
{            
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY); // 60
   
   for (int i = 0; i < 5; i++)
   {
      pwm.setPWM(i, 0, pulseWidth(servo_Angle[i]));
   }
  
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
     byte proc;
  // check if data has been sent from the computer:
  if (Serial.available()) 
  {
    // read the most recent byte (which will be from 0 to 255):
    proc = Serial.read();
    if (proc == 24) //y------->Servo 1
    {
      MoveServoArm(0, 5);
    }
    else if (proc == 20) //u
    {
      MoveServoArm(0, -5);
    }
    else if (proc == 6) //g------>Servo 2
    {
      MoveServoArm(1, 5);
    }
    else if (proc == 7) //h
    {
      MoveServoArm(1, -5);
    }
    else if (proc == 21) //v------>Servo 3
    {
      MoveServoArm(2, 5);
    }
    else if (proc == 1) //b
    {
      MoveServoArm(2, -5);
    }
    else if (proc == 8) //i------>Servo 4
    {
      MoveServoArm(3, 5);
    }
    else if (proc == 14) //o
    {
      MoveServoArm(3, -5);
    }
    else if (proc == 9) //j------>Servo 5
    {
      MoveServoArm(4, 5);
    }
    else if (proc == 10) //k
    {
      MoveServoArm(4, -5);
    }
    else if (proc == 13) //n------>Servo 6
    {
      MoveServoArm(5, 5);
    }
    else if (proc == 12) //m
    {
      MoveServoArm(5, -5);
    }
    else if (proc == 0) //a------>Motors
    {
      if (leftSpd < 255)
      {
        leftSpd += 5;
      }
    }
    else if (proc == 18) //s
    {
      if (leftSpd > -255)
      {
        leftSpd -= 5;
      }
      if (rightSpd > -255)
      {
        rightSpd -= 5;
      }
    }
    else if (proc == 3) //d
    {
      if (rightSpd < 255)
      {
        rightSpd += 5;
      }
    }
    else if (proc == 22) //w
    {
      if (rightSpd < 255)
      {
        rightSpd += 5;
      }
      if (leftSpd < 255)
      {
        leftSpd += 5;
      }
    }

    DriveMotors();
  }
}

//------------------------------------------
//Pulse Width from desired angle
//------------------------------------------
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
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

      if(rightSpd > 0)
      {
        rightSpd -= 1;
      }
      else
      {
        rightSpd += 1;
      }

      if(leftSpd > 0)
      {
        leftSpd -= 1;
      }
      else
      {
        leftSpd += 1;
      }
}

//------------------------------------------
//Move Selected Servo Arm
//------------------------------------------
void MoveServoArm(int selectedServoNumber, int value)
{
    servo_Angle[selectedServoNumber] += value;
    
    if (servo_Angle[selectedServoNumber] < 0)
      {
        servo_Angle[selectedServoNumber] = 0;
      }

      if (servo_Angle[selectedServoNumber] > 120)
      {
        servo_Angle[selectedServoNumber] = 120;
      }

       pwm.setPWM(selectedServoNumber, 0, pulseWidth(servo_Angle[selectedServoNumber]));
}
