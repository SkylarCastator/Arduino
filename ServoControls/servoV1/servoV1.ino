#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

int servo_Angle[] = {60,60,60,60,60,60,60};

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
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
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
  }
}

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

