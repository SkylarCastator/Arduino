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

//Sensors
int xAccelerometerValue;
int yAccelerometerValue;
int zAccelerometerValue;
int xGyroscopeValue;
int yGyroscopeValue;

int throttle, steering;
int xraw, yraw, leftSpd, rightSpd;

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
  while(1)
  {
    ReadAccelerometerValues();
    ReadGyroscopeValues();

    if (DEBUG)
    { 
      KeyboardController();
    }
    else
    {
      // capture joystick position
      // yraw = analogRead(JOYSTICK_Y_PIN);
      // xraw = analogRead(JOYSTICK_X_PIN);
    }
    
    CalculateSpeed();
    DriveMotors();

    if (DEBUG)
    { 
      PrintJoystickControls();
      delay(250);
    }
  }
}

//------------------------------------------
//Drive Controller (Move to own class)
//------------------------------------------
void CalculateSpeed()
{
   // scale x and y axis so that both are zeroed at natural center of potentiometer, implement deadband
    if(yraw > YRAW_CENTERED + HALF_DEADBAND)//forward
    {
      throttle = map(yraw, YRAW_CENTERED + HALF_DEADBAND, 1023, 0, 255);      
      if(xraw > XRAW_CENTERED + HALF_DEADBAND)//left
      {
        steering = map(xraw, XRAW_CENTERED + HALF_DEADBAND, 1023, -0, -255);// reverse pot coordinates to agree with cartesian        
      }
      else if(xraw < XRAW_CENTERED - HALF_DEADBAND)// right
      {
        steering = map(xraw, XRAW_CENTERED - HALF_DEADBAND, 0, 0, 255);// reverse pot coordinates to agree with cartesian        
      }
      else // in X deadband
      {
        steering = 0;
      }
    }
    else if(yraw < YRAW_CENTERED - HALF_DEADBAND)//reverse
    {
      throttle = map(yraw, YRAW_CENTERED - HALF_DEADBAND, 0, -0, -255);      
      if(xraw > XRAW_CENTERED + HALF_DEADBAND)//left
      {
        steering = map(xraw, XRAW_CENTERED + HALF_DEADBAND, 1023, -0, -255);// reverse pot coordinates to agree with cartesian        
      }
      else if(xraw < XRAW_CENTERED - HALF_DEADBAND)// right
      {
        steering = map(xraw, XRAW_CENTERED - HALF_DEADBAND, 0, 0, 255);// reverse pot coordinates to agree with cartesian        
      }
      else// in X deadband
      {
        steering = 0;
      }
    }
    else //in Y deadband
    {
      throttle = 0;
      if(xraw > XRAW_CENTERED + HALF_DEADBAND)//left
      {
        steering = map(xraw, XRAW_CENTERED + HALF_DEADBAND, 1023, -0, -255);// reverse pot coordinates to agree with cartesian        
      }
      else if(xraw < XRAW_CENTERED - HALF_DEADBAND)// right
      {
        steering = map(xraw, XRAW_CENTERED - HALF_DEADBAND, 0, 0, 255);// reverse pot coordinates to agree with cartesian        
      }
      else// in X deadband
      {
        steering = 0;
      }
    }
    
      //transfer from x,y to l,r
      leftSpd = throttle + steering;
      rightSpd = throttle - steering;
      //leftSpd and rightSpd will contain a number from -255 to 255 that gets updated each time the loop runs.
      //-255 equates to full reverse. 0 equates to anywhere within the deadband.  255 is full forward.
}

void PrintJoystickControls()
{
  Serial.print("x joystick value is: ");
  Serial.println(xraw);
  Serial.print("y joystick value is: ");
  Serial.println(yraw);
  Serial.print("throttle is: ");
  Serial.println(throttle);
  Serial.print("steering is: ");
  Serial.println(steering);
  Serial.print("leftSpd is: ");
  Serial.println(leftSpd);
  Serial.print("rightSpd is: ");
  Serial.println(rightSpd);
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

//------------------------------------------
//IMU Controller (Move to own class)
//------------------------------------------
void ReadAccelerometerValues()
{
  xAccelerometerValue = analogRead(0);       // read analog input pin 0
  yAccelerometerValue = analogRead(1);       // read analog input pin 1
  zAccelerometerValue = analogRead(2);       // read analog input pin 1
  Serial.print("accelerations are x, y, z: ");
  Serial.print(xAccelerometerValue, DEC);    // print the acceleration in the X axis
  Serial.print(" ");       // prints a space between the numbers
  Serial.print(yAccelerometerValue, DEC);    // print the acceleration in the Y axis
  Serial.print(" ");       // prints a space between the numbers
  Serial.println(zAccelerometerValue, DEC);  // print the acceleration in the Z axis
}

void ReadGyroscopeValues()
{
  //xGyroscopeValue = analogRead(0);       // read analog input pin 0
  //yGyroscopeValue = analogRead(1);       // read analog input pin 1
  //Serial.print("rotational rates are x, y: ");
  //Serial.print(xGyroscopeValue, DEC);    // print the rotational rate in the X axis
  //Serial.print(" ");       // prints a space between the numbers
  //Serial.println(yGyroscopeValue, DEC);  // print the rotational rate in the Y axis
}

//------------------------------------------
//Keyboard Controller (Move to own class)
//------------------------------------------
void KeyboardController()
{
   if (Serial.available())
   {
     char ch = Serial.read();
 
     if (ch=='a')
     {
       if (xraw > 0)
       {
         xraw -= 1;
       }
     }
     else
     {
       if (xraw < XRAW_CENTERED)
       {
         xraw += 1;
       }
     }
     if (ch=='d')
     {
       if (xraw < 1023)
       {
         xraw += 1;
       }
     }
     else
     {
       if (xraw > XRAW_CENTERED)
       {
         xraw -= 1;
       }
     }
     if (ch=='s')
     {
       if (yraw > 0)
       {
         yraw -= 1;
       }
     }
     else
     {
       if (yraw < YRAW_CENTERED)
       {
         yraw += 1;
       }
     }
     if (ch=='w')
     {
       if (yraw < 1023)
       {
         yraw += 1;
       }
     }
     else
     {
       if (yraw > YRAW_CENTERED)
       {
         yraw -= 1;
       }
     }
   }
}

