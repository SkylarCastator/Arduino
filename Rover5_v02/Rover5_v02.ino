

// 2 is right motors 1 is left motors
int PinPWM_b1 = 11;   // ch1 3
int PinPWM_b2 = 10;   //ch2 5
int PinPWM_y1 = 5;   //ch3 10
int PinPWM_y2 = 3;  //ch4 11
int PinRot_g1 = 12;   //ch1 4
int PinRot_g2 = 8;   //ch2 7
int PinRot_r1 = 7;    //ch3 8 
int PinRot_r2 = 4;   //ch4 12

void setup() {
  // put your setup code here, to run once:
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

void loop() 
{
   // put your main code here, to run repeatedly:
   digitalWrite(PinRot_r2,HIGH);   // turn right motors 
   digitalWrite(PinRot_g2,LOW);  
   digitalWrite(PinRot_r1,LOW);   // turn left motors
   digitalWrite(PinRot_g1,HIGH); 
        
   analogWrite(PinPWM_y2,200);   // provide power to right motors
   analogWrite(PinPWM_b2,200);  
   analogWrite(PinPWM_y1,230);   // provide power to left motors
   analogWrite(PinPWM_b1,230);
   delay(1000);
}
