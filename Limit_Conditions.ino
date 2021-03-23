// Constants for pin declarations
const byte limInterruptPin1 = 2;
const byte limInterruptPin2 = 3;
const byte motor_b1 = 9;
const byte motor_b2 = 8;

// Variables
byte motor_direction = 0;
byte state_flag = 0;
volatile byte lim1Flag = 0;
volatile byte lim2Flag = 0;

void loop() {
  if (Serial.available() > 0) {
    motor_direction = Serial.read();
    if (motor_direction == 49) {
      openLid(motor_b1, motor_b2);
      state_flag = 2;
    }
  }
  if (lim1Flag == 1 && state_flag == 2) {
    stopMotor(motor_b1, motor_b2);
    delay(5000);
    closeLid(motor_b1, motor_b2);
    state_flag = 1;
  }
  if (lim2Flag == 1 && state_flag == 1) {
    stopMotor(motor_b1, motor_b2);
    state_flag = 0;
  }

}

//-------------------------------------------------------------------------------------
// Setup Function

void setup() {
  // Set the limit switches as inputs to the Arduino and use the Arduino's internal pullup resistor
  pinMode(limInterruptPin1, INPUT_PULLUP);
  pinMode(limInterruptPin2, INPUT_PULLUP);

  // Set the output pins from the Arduino to the motor driver board
  pinMode(motor_b1, OUTPUT);
  pinMode(motor_b2, OUTPUT);

  // Setup the Interrupts
  attachInterrupt(digitalPinToInterrupt(limInterruptPin1), limit1Hit, FALLING);
  attachInterrupt(digitalPinToInterrupt(limInterruptPin2), limit2Hit, FALLING);

  // Initialize the motor to be stopped
  stopMotor(motor_b1, motor_b2);
  
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
