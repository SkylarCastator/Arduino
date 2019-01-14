#include <math.h> 

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

//Robot Characteristics
//MaxSpeed 10 in/s (25 cm/s)
//Specifications:
//Motor rated voltage: 7.2V
//Motor stall current: 2.5A
//Output shaft stall torque: 10Kg/cm
//Gearbox ratio: 86.8:1
//Encoder type: Quadrature
//Encoder resolution: 1000 state changes per 3 wheel rotations
//Speed: 1Km/hr
float maxSpeed = 25;
float minSpeed = 1;
float maxRotationSpeed = 5;
float minRotationSpeed = 3;

float currentYaw = 0;
float currentPitch = 0;
float currentRoll = 0;
float currentSpeed = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(PinPWM_b1,OUTPUT);    // back left motor pwm
  pinMode(PinPWM_b2,OUTPUT);   // back right motor pwm
  pinMode(PinPWM_y1,OUTPUT);   // front left motor pwm
  pinMode(PinPWM_y2,OUTPUT);    // front right motor pwm
  
  pinMode(PinRot_g1,OUTPUT);     // back left motor dir
  pinMode(PinRot_g2,OUTPUT);     // back right motor dir
  pinMode(PinRot_r1,OUTPUT);     // front left motor dir
  pinMode(PinRot_r2,OUTPUT);     // front right motor dir
}

bool driveForwardEnabled = true;
int counterLoopDriveForword = 0;
int driveForwardMax = 10;

bool driveReverseEnabled = false;
int counterLoopDriveReverse = 0;
int driveReverseMax = 10;

bool driveInCircleEnabled = false;
int counterLoopDriveCircle = 0;
int driveCircleMax = 10;

void loop()
{
  ReadAccelerometerValues();
  ReadGyroscopeValues();

   MotorSpeedController(0, 0.5, 0);

  delay(1000);
} 

void MotorSpeedController(int direction, float robotSpeed, float angleRotation)
{
    if (direction == 0)
    {
      digitalWrite(PinRot_g1,HIGH);   // turn all motors backwards
      digitalWrite(PinRot_g2,HIGH);  
      digitalWrite(PinRot_r1,HIGH); 
      digitalWrite(PinRot_r2,HIGH);
    }
    else
    {
      //Drive Reverse Tire
      digitalWrite(PinRot_g1,LOW);   // turn all motors backwards
      digitalWrite(PinRot_g2,LOW);  
      digitalWrite(PinRot_r1,LOW); 
      digitalWrite(PinRot_r2,LOW);
    }
    
    int analogControl = (255) * (robotSpeed);
    Serial.print("AnalogControl : " + String(analogControl));
    analogWrite(PinPWM_b1,analogControl);     // provide power to all motors
    analogWrite(PinPWM_b2,analogControl);
    analogWrite(PinPWM_y1,analogControl);
    analogWrite(PinPWM_y2,analogControl);
}

void RotateTowardsDirection(float rotationDirection)
{
    float rotationDifference = abs(currentYaw - rotationDirection);
    if (rotationDifference > 180)
    {
     //TurnLeft
      digitalWrite(PinRot_g1,LOW);   // turn all motors backwards
      digitalWrite(PinRot_g2,HIGH);  
      digitalWrite(PinRot_r1,HIGH); 
      digitalWrite(PinRot_r2,LOW); 
    }
    else
    {
    //TurnRight
      digitalWrite(PinRot_g1,HIGH);   // turn all motors backwards
      digitalWrite(PinRot_g2,LOW);  
      digitalWrite(PinRot_r1,LOW); 
      digitalWrite(PinRot_r2,HIGH);
    }

    Serial.print("Changing Direction: " + String(rotationDifference));
    Serial.println("");
}

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
  xGyroscopeValue = analogRead(0);       // read analog input pin 0
  yGyroscopeValue = analogRead(1);       // read analog input pin 1
  Serial.print("rotational rates are x, y: ");
  Serial.print(xGyroscopeValue, DEC);    // print the rotational rate in the X axis
  Serial.print(" ");       // prints a space between the numbers
  Serial.println(yGyroscopeValue, DEC);  // print the rotational rate in the Y axis
}
