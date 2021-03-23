//constants
const int trigPin = 6;
const int echoPin = 5;

//variables
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);   //Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    //Sets the echoPin as an Input
  Serial.begin(9600);         //Starts the serial communication
}

void loop() {
  //Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //Sets the trigPin HIGH for 10 microseconds to intialize Ultrasonic pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  //Reads the echoPin, returns the second wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance:
  distance = duration/58; //Note: 0.034 represents the speed of sound in cm/us
                               //Note: We divide by two because the pulse travels back and forth which is double the distance we want

  if (distance < 10) {
    //Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
  } 
  
}
