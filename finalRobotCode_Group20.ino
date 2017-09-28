
////////////////////////////
// ELEC299 - Winter2015
////////////////////////////
// “finalRobotCode_Group20”
//
//
// Group #: 20
//
// Group Members:
//    Maytha Nassor (10085970)
//    Johan Cornelissen (10098176)
//    Muhammad Usman Majeed  (10086980)
//
////////////////////////////

#include <QSerial.h> //serial for IR bug eye
QSerial myIRserial;
#include <Servo.h> 
Servo claw; //parameters for servo movement
Servo vertical; 
Servo horizontal; 
int LB = 11; //left bumper digital pin
int RB = 12; //right bumper digital pin
int pin=10; //force sensor digital pin
int txpin = -1; //two parameters for beacon sensing
int rxpin = 9; 
int LD = 4; //left direction digital pin
int LS = 5; //left speed digital pin
int RD = 7; //right direction digital pin
int RS = 6; //right speed digital pin
int L = A0; //left sensor digital pin
int M = A1; //middle sensor digital pin
int R = A2; //right sensor digital pin

//Threshold values for IR 
#define LTHRESH 480 
#define CTHRESH 480  
#define RTHRESH 480 

void setup (){
  //Defining input and output pins
  pinMode(LD, OUTPUT);
  pinMode(LS, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(L, INPUT);
  pinMode(M, INPUT);
  pinMode(R, INPUT);
  pinMode(LB, INPUT);
  pinMode(RB, INPUT);
  pinMode(pin, INPUT);

  myIRserial.attach(rxpin, txpin); //Initiating the bug eye sensor
  Serial.begin(9600);

  horizontal.attach(3); //Servo assignments
  vertical.attach(8);
  claw.attach(2);

}

void loop (){
  vertical.write(180);   
  horizontal.write(85);
  // Case determining 
  int caseV = caseReturn();  
  if(caseV == 1) 
    caseOne(); 
  if(caseV == 2) 
    caseTwo(); 
  if(caseV == 0) 
    caseZero(); 
  backToCenter(); // Bring robot back to starting position 
}

// Returns an integer value representing the corresponding location of the ball 
int caseReturn()
{
  vertical.write(80); // Sets vertical servo flat 
  int positionh = 180; 
  int positiont; 
  int caseR;
  while(!beaconFinder()) // While beacon hasn't been found 
  {
    if(positionh < -10) // Accounts for slighly uncalibrated servo positioning, resets servo angle 
      positionh = 180;
    horizontal.write(positionh); // Sets servo position
    delay(200);
    positiont = positionh;
    positionh = positionh - 95; // Decrement servo angle 
  }
  // Assigns case based on last position of servo angle 
  if (positiont == 180) 
    caseR = 0; 
  else if (positiont == 85 || positionh == 180) 
    caseR = 1; 
  else
    caseR = 2; 
  horizontal.write(85);
  return caseR; 
} 

// Beacon scanning function - returns true if found, false otherwise 
boolean beaconFinder(){
  int count = 1; 
  boolean result; 
  while(count < 6) // Scans each location 5 times 
  {
    int val = myIRserial.receive(200);
    delay(100);
    if(val != 0 )
      return true;     
    delay(100); 
    count++;
  }
  return false; 
}

// Routine for ball located at location 1 
void caseOne (){
  // Brings robot to ball location 
  horizontal.write(80);
  vertical.write(180); 
  lineFollow2(160); // Line follows at speed specified 
  mBackward(); 
  delay(300); 
  mStop(); 
  delay(10);
  claw.write(0); 
  for(int pos1 = 180; pos1 > 70  ; pos1 -= 1)  // Brings vertical arm down to grab the ball 
  {
    vertical.write(pos1);              
    delay(10);  
  }
  int x = 0; 
  int pinRead = 1; 
  // Closes claw until force sensor is triggered 
  if(digitalRead(pin) != 0){
    for(int pos = 0; pos < 110 && pinRead == 1  ; pos += 1)  // goes from 0 degrees to 180 degrees 
    { 
      if(digitalRead(pin) == 0)
        pinRead = 0;                               
      claw.write(pos);              
      delay(30);  
      x = pos; 
    }
  }
  // Brings robot infront of goal, stopping when all IR sensors detect black (Integrated in lineFollow)
  mRotateNew2(1); 
  vertical.write(140);              
  lineFollow(160);
  mForward(); 
  delay(200); 
  lineFollow(160); 
  delay(10);
  // Drives forward until bump sensors are triggered 
  while((digitalRead(LB) == 1 && digitalRead(RB) == 1)){
    digitalWrite(RD, 0);
    digitalWrite(LD, 0);
    analogWrite(RS, 100);
    analogWrite(LS, 100);
  } 
  mStop(); 
  claw.write(10); // Opens claw to drop the ball 
}

// Routine for ball located at location 1 
void caseTwo (){
  // Brings robot to ball location 
  vertical.write(180);
  claw.write(0);           
  mRotateNew(0);
  delay(10);
  lineFollow2(160); 
  mBackward(); 
  delay(300); 
  mStop(); 
  delay(10);
  for(int pos1 = 80; pos1 > 70  ; pos1 -= 1)  // Brings vertical arm down to grab the ball 
  {                                 
    vertical.write(pos1);              
    delay(10);  
  }
  int x = 0; 
  int pinRead = 1; 
  // Closes claw until force sensor is triggered 
  if(digitalRead(pin) != 0){
    for(int pos = 0; pos < 110 && pinRead == 1  ; pos += 1) 
    { 
      if(digitalRead(pin) == 0)
        pinRead = 0;                               
      claw.write(pos);               
      delay(30);  
      x = pos; 
    }
  }
  // Brings robot infront of goal, stopping when all IR sensors detect black (Integrated in lineFollow)
  mRotateNew2(1); 
  vertical.write(140);              
  lineFollow(160); 
  mForward(); 
  delay(350); 
  mStop(); 
  delay(10); 
  mRotateNew(1);
  lineFollow(160); 
  delay(10);
  // Drive forward until bump sensors are triggered 
  while((digitalRead(LB) == 1 && digitalRead(RB) == 1)){
    digitalWrite(RD, 0);
    digitalWrite(LD, 0);
    analogWrite(RS, 100);
    analogWrite(LS, 100);
  } 
  mStop(); 
  claw.write(10); // Open claw 
} 

// Routine for ball located at location 1 
void caseZero (){
  // Brings robot to ball location
  vertical.write(180);
  claw.write(0);             
  mRotateNew(1);
  delay(10);
  lineFollow2(160); 
  mBackward(); 
  delay(300); 
  mStop(); 
  delay(10);
  for(int pos1 = 80; pos1 > 70  ; pos1 -= 1)  // Brings vertical arm down to grab ball 
  {                                
    vertical.write(pos1);             
    delay(10);  
  }
  int x = 0; 
  int pinRead = 1; 
  // Closes claw until force sensor is triggered 
  if(digitalRead(pin) != 0){
    for(int pos = 0; pos < 110 && pinRead == 1  ; pos += 1)  
    { 
      if(digitalRead(pin) == 0)
        pinRead = 0;                            
      claw.write(pos);              
      delay(30);  
      x = pos; 
    }
  }
  // Brings robot infront of goal, stopping when all IR sensors detect black (Integrated in lineFollow)
  mRotateNew2(1); 
  vertical.write(140);              
  lineFollow(160); 
  mForward(); 
  delay(350); 
  mStop(); 
  delay(10); 
  mRotateNew(0);
  lineFollow(150); 
  delay(10);
  // Drive forward until bump sensors are triggered 
  while((digitalRead(LB) == 1 && digitalRead(RB) == 1)){
    digitalWrite(RD, 0);
    digitalWrite(LD, 0);
    analogWrite(RS, 100);
    analogWrite(LS, 100);
  } 
  mStop(); 
  claw.write(10); // Open claw to drop ball 
} 

// Brings robot back to center starting position 
void backToCenter(){
  mBackward(); 
  delay(300);  
  mRotateNew2(1); 
  lineFollow(160); 
  mForward(); 
  delay(250); 
  mStop();
  delay(100);
} 

// Line following function that stops when bump sensors are triggered 
void lineFollow2(int speed2)
{
  delay(10);
  while((digitalRead(LB) == 1 && digitalRead(RB) == 1)){
    if (analogRead(L) <= LTHRESH){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, speed2);
      analogWrite(LS, 0);
    }
    else if (analogRead(R) <= RTHRESH){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, 0);
      analogWrite(LS, speed2);
    }
    else if (analogRead(M) <= CTHRESH ){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, speed2);
      analogWrite(LS, speed2);
    }
    delay(10);
    while(analogRead(L) < LTHRESH && analogRead(R) < RTHRESH){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, 0);
      analogWrite(LS, 0);
      delay(10); 
      break;
    }
    digitalWrite(RD, LOW);
    digitalWrite(LD, LOW);
    analogWrite(RS, 0);
    analogWrite(LS, 0);
  }
  digitalWrite(RD, LOW);
  digitalWrite(LD, LOW);
  analogWrite(RS, 0);
  analogWrite(LS, 0); 
}  

// Line follow function that is stopped when all three IR sensors detect black 
void lineFollow(int speed2)
{
  delay(10);
  while(analogRead(L) > LTHRESH || analogRead(R) > RTHRESH || analogRead(M) > CTHRESH){
    if (analogRead(L) <= LTHRESH){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, speed2);
      analogWrite(LS, 0);

    }
    else if (analogRead(R) <= RTHRESH){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, 0);
      analogWrite(LS, speed2);
    }
    else if (analogRead(M) <= CTHRESH ){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, speed2);
      analogWrite(LS, speed2);
    }
    delay(10);
    while(analogRead(L) < LTHRESH && analogRead(R) < RTHRESH){
      digitalWrite(RD, LOW);
      digitalWrite(LD, LOW);
      analogWrite(RS, 0);
      analogWrite(LS, 0);
      delay(10); 
      break;
    }
    digitalWrite(RD, LOW);
    digitalWrite(LD, LOW);
    analogWrite(RS, 0);
    analogWrite(LS, 0);
  }
  digitalWrite(RD, LOW);
  digitalWrite(LD, LOW);
  analogWrite(RS, 0);
  analogWrite(LS, 0);

}  

// Rotates the robot until middle IR sensor senses black line, intended for 90 degree turn based on delays 
void mRotateNew(int direction1)
{
  if(direction1 == 1) // Rotates robot clockwise 
  {
    digitalWrite(RD, 1);
    digitalWrite(LD, 0);
    analogWrite(RS, 100);
    analogWrite(LS, 100);
    delay(400); 
    while( analogRead(M) > CTHRESH )
    {
      delay(10);
      digitalWrite(RD, 1);
      digitalWrite(LD, 0);
      analogWrite(RS, 90);
      analogWrite(LS, 90);
    }
    digitalWrite(RD, 1);
    digitalWrite(LD, 0);
    analogWrite(RS, 0);
    analogWrite(LS, 0);
  } 
  else 
  {
    digitalWrite(RD, 0);
    digitalWrite(LD, 1);
    analogWrite(RS, 90);
    analogWrite(LS, 90);
    delay(400);
    while( analogRead(M) > CTHRESH )
    {
      delay(10);
      digitalWrite(RD, 0);
      digitalWrite(LD, 1);
      analogWrite(RS, 90);
      analogWrite(LS, 90);
    }
    digitalWrite(RD, 1);
    digitalWrite(LD, 0);
    analogWrite(RS, 0);
    analogWrite(LS, 0);
  }
} 

// Rotates the robot until middle IR sensor senses black line, intended for 180 degree turn
void mRotateNew2(int direction1)
{
  if(direction1 == 1) 
  {
    digitalWrite(RD, 1);
    digitalWrite(LD, 0);
    analogWrite(RS, 100);
    analogWrite(LS, 100);
    delay(800); // longer delay compared to 90 degree turn 
    while( analogRead(M) > CTHRESH )
    {
      delay(10);
      digitalWrite(RD, 1);
      digitalWrite(LD, 0);
      analogWrite(RS, 80);
      analogWrite(LS, 80);
    }
    digitalWrite(RD, 1);
    digitalWrite(LD, 0);
    analogWrite(RS, 0);
    analogWrite(LS, 0);
  } 
  else 
  {
    digitalWrite(RD, 0);
    digitalWrite(LD, 1);
    analogWrite(RS, 80);
    analogWrite(LS, 80);
    delay(800);
    while( analogRead(M) > CTHRESH )
    {
      delay(10);
      digitalWrite(RD, 0);
      digitalWrite(LD, 1);
      analogWrite(RS, 80);
      analogWrite(LS, 80);
    }
    digitalWrite(RD, 1);
    digitalWrite(LD, 0);
    analogWrite(RS, 0);
    analogWrite(LS, 0);
  }
} 

// Stops robot motors 
void mStop(){
  digitalWrite(RD, 1);
  digitalWrite(LD, 0);
  analogWrite(RS, 0);
  analogWrite(LS, 0);

}

// Drives robot forwards 
void mForward(){
  digitalWrite(RD, 0);
  digitalWrite(LD, 0);
  analogWrite(RS, 100);
  analogWrite(LS, 100);
}

// Drives robot backwards 
void mBackward(){
  digitalWrite(RD, 1);
  digitalWrite(LD, 1);
  analogWrite(RS, 100);
  analogWrite(LS, 100);
}






