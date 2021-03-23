const int speakerPin = 4;

int speaker = 0;

void loop() {
  if (Serial.available() > 0) {
    speaker = Serial.read();
    if (speaker == 48) {
      noTone(speakerPin);
    }
    else if (speaker == 49) {
      yell(speakerPin, 1000, 3000, 10);
    }
  }
  
}

//-------------------------------------------------------------------------------------
// Setup Function

void setup() {
  // Set the output pin to the operational amp that outputs to a speaker
  pinMode(speakerPin, OUTPUT);

  Serial.begin(9600);
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
