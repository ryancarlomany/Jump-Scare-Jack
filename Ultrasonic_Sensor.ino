//constants
const byte trigPin = 6;
const byte echoPin = 5;

//variables
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);   //Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    //Sets the echoPin as an Input
  Serial.begin(9600);         //Starts the serial communication
}

void loop() {
  distance = proximityRead(trigPin, echoPin, distance, duration);
  
  if (distance < 30) {
    //Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
  } 
  
}

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
