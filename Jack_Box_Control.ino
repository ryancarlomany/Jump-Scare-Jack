// Constants for pin declarations
const byte limInterruptPin1 = 2;
const byte limInterruptPin2 = 3;
const int speakerPin = 4;
const byte motorB1 = 9;
const byte motorB2 = 8;

// Variables
byte receivedBit = 0;
byte stateFlag = 0;
volatile byte lim1Flag = 0;
volatile byte lim2Flag = 0;

void loop() {
  if (Serial.available() > 0) {
    receivedBit = Serial.read();
    if (receivedBit == 49) {
      openLid(motorB1, motorB2);
      yell(speakerPin, 1000, 3000, 10);
      stateFlag = 2;
    }
  }
  if (lim1Flag == 1 && stateFlag == 2) {
    stopMotor(motorB1, motorB2);
    delay(5000);
    closeLid(motorB1, motorB2);
    stateFlag = 1;
  }
  if (lim2Flag == 1 && stateFlag == 1) {
    stopMotor(motorB1, motorB2);
    stateFlag = 0;
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

  // Setup the Interrupts
  attachInterrupt(digitalPinToInterrupt(limInterruptPin1), limit1Hit, FALLING);
  attachInterrupt(digitalPinToInterrupt(limInterruptPin2), limit2Hit, FALLING);

  // Initialize the motor to be stopped
  stopMotor(motorB1, motorB2);
  
  Serial.begin(9600);
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
void openLid(byte motorIn1, byte motorIn2) {
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
}
// Move the motor counterclockwise
void closeLid(byte motorIn1, byte motorIn2) {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
}
//-------------------------------------------------------------------------------------

// Speaker Function

void yell(byte outputPin, int minFreq, int maxFreq, int delayTime) {
  int freqStep = (maxFreq - minFreq) / 100;
  
  for (int freq = minFreq; freq < maxFreq; freq += freqStep) {
    tone(outputPin, freq); // Outputs a tone of the specified frequency to a speaker
    delay(delayTime);
  }
  noTone(outputPin);
}
