#define in1 13
#define in2 9
#define en1 10
#define chA 6
#define chB 7

// Pulses sensed
long counter = 0;

int direction = 0;

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(chA, INPUT_PULLUP);
  pinMode(chB, INPUT_PULLUP);

  // Interrupt calls
  attachInterrupt(digitalPinToInterrupt(chA), encoderPinA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(chB), encoderPinB, CHANGE);
}

int mtrspd = 150;

void loop() {
  // Motor settings
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(en1, mtrspd);

  Serial.println(digitalRead(chA));

  if (direction)
    Serial.println("CL");
  else
    Serial.println("Anti-CL");
}

void encoderPinA(void) {
  if (digitalRead(chA) != digitalRead(chB)) {
    counter++;
    direction = 1;
  } else {
    counter--;
    direction = 0;
  }
}

void encoderPinB(void) {
  if (digitalRead(chA) == digitalRead(chB)) {
    counter++;
    direction = 1;
  } else {
    counter--;
    direction = 0;
  }
}