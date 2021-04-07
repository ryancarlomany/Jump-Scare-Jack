// Constants for pin declarations
const byte limInterruptPin1 = 2;
const byte limInterruptPin2 = 3;
const byte speakerPin = 4;
const byte echoPin = 5;
const byte trigPin = 7;
const byte motorB1 = A0;
const byte motorB2 = A1;
const byte motorEnable = 6;

// Variables
byte proximityFlag = 0;
byte stateFlag = 0;
byte closingSpeed = 100;
byte openSpeed = 255;
byte triggerDistance = 100;
volatile byte lim1Flag = 0;
volatile byte lim2Flag = 0;
long duration;
int distance;

//-------------------------------------------------------------------------------------
// Loop Function

void loop() {
  if (proximityFlag == 0) {
    distance = proximityRead(trigPin, echoPin, distance, duration);
    
    if (distance < triggerDistance) {
    proximityFlag = 1;
    } 
  }
  
  if (proximityFlag == 1) {
    openLid(motorB1, motorB2, openSpeed);
    yell(speakerPin, 1000, 3000, 10);
    stateFlag = 2;
    proximityFlag = 2;
  }
  if (lim1Flag == 1 && stateFlag == 2) {
    stopMotor(motorB1, motorB2);
    delay(5000);
    closeLid(motorB1, motorB2, closingSpeed);
    closingYell(speakerPin, 10);
    stateFlag = 1;
  }
  if (lim2Flag == 1 && stateFlag == 1) {
    stopMotor(motorB1, motorB2);
    stateFlag = 0;
    proximityFlag = 0;
    delay(5000);
  }

}

//-------------------------------------------------------------------------------------
// Setup Function

void setup() {
  // Set the limit switches as inputs to the Arduino and use the Arduino's internal pullup resistor
  pinMode(limInterruptPin1, INPUT_PULLUP);
  pinMode(limInterruptPin2, INPUT_PULLUP);

  // Set the output pins from the Arduino to the motor driver board
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  // Set the output pin to the operational amp that outputs to a speaker
  pinMode(speakerPin, OUTPUT);

  // Setup the pins for the ultrasonic sensor
  pinMode(trigPin, OUTPUT);   //Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    //Sets the echoPin as an Input

  // Setup the Interrupts
  attachInterrupt(digitalPinToInterrupt(limInterruptPin1), limit1Hit, FALLING);
  attachInterrupt(digitalPinToInterrupt(limInterruptPin2), limit2Hit, FALLING);

  // Initialize the motor to be stopped
  stopMotor(motorB1, motorB2);

  randomSeed(analogRead(5)); //Uses noise from the unconnected A5 pin to generate a random seed number for the Jack Box scream
}

//-------------------------------------------------------------------------------------
// Interrupt Service Routines

void limit1Hit() {
  lim1Flag = 1;
  lim2Flag = 0;
}
void limit2Hit() {
  lim1Flag = 0;
  lim2Flag = 1;
}
//-------------------------------------------------------------------------------------

// Motor Functions
/*
Truth Table for the h-bridge Motor Driver
Input 1            Input 2            Direction
   0                   0              Motor OFF
   1                   0              Clockwise
   0                   1              Counterclockwise
   1                   1              Motor OFF 
*/
// Stop the motor
void stopMotor(byte motorIn1, byte motorIn2) {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
}
// Move the motor clockwise
void openLid(byte motorIn1, byte motorIn2, byte motorSpeed) {
  analogWrite(motorEnable, motorSpeed);
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
}
// Move the motor counterclockwise
void closeLid(byte motorIn1, byte motorIn2, byte motorSpeed) {
  analogWrite(motorEnable, motorSpeed);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
}

//-------------------------------------------------------------------------------------

// Speaker Function

void yell(byte outputPin, int minFreq, int maxFreq, int delayTime) {
  int freqStep = (maxFreq - minFreq) / 50;
  
  for (int freq = minFreq; freq < maxFreq; freq += freqStep) {
    tone(outputPin, freq); // Outputs a tone of the specified frequency to a speaker
    delay(delayTime);
  }
  noTone(outputPin);
}

void closingYell(byte outputPin, int delayTime) {
  for (int incr = 0; incr < 100; incr += 1) {
    tone(outputPin, random(100, 500));
    delay(delayTime);
  }
  noTone(outputPin);
}
//-------------------------------------------------------------------------------------

// Ultra Sonic Sensor Function

int proximityRead(byte trigOutputPin, byte echoOutputPin, int dist, long dur) {
  //Clear the trigPin
  digitalWrite(trigOutputPin, LOW);
  delayMicroseconds(2);

  //Sets the trigPin HIGH for 10 microseconds to intialize Ultrasonic pulse
  digitalWrite(trigOutputPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigOutputPin, LOW);

  //Reads the echoPin, returns the second wave travel time in microseconds
  dur = pulseIn(echoOutputPin, HIGH);

  //Calculate the distance:
  dist = dur/58; //Note: 0.034 represents the speed of sound in cm/us
                               //Note: We divide by two because the pulse travels back and forth which is double the distance we want
                               
  return dist;
}
