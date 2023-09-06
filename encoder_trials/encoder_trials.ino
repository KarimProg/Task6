/*
 * File: encoder_trials.ino
 * Author: Karim Abogharbia
 * Description: This program interfaces with a motor's encoder to get the distance after being filtered
 *              using a second order butterworth filter.
 * Date: [6/9/2023]
 */

/* Macros */
#define in1 13
#define in2 9
#define en1 10
#define chA 2
#define chB 3

// Pulses sensed
long counter = 0;

int direction = 0;  // Direction of motor
int mtrspd = 150;   // Speed of motor
int k = 0;          // Counter for serial monitor frequency

float x[] = {0,0,0};
float y[] = {0,0,0};

void setup() {
  // Begin serial communication at default rate(9600)
  Serial.begin(9600);

  // Pin setup
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(chA, INPUT_PULLUP);
  pinMode(chB, INPUT_PULLUP);

  // Interrupt calls
  attachInterrupt(digitalPinToInterrupt(chA), encoderPinA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(chB), encoderPinB, CHANGE);
}


void loop() {
  // Motor settings
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(en1, mtrspd);

  // Filter Intake
  x[0] = counter;

  // Filter coeffecients
  float b[] = { 0.25573591, 0.51147183, 0.25573591 };
  float a[] = { 0.15275728, -0.17570093 };

  // Second order butterworth filter eqn
  y[0] = a[0] * y[1] + a[1] * y[2] + b[0] * x[0] + b[1] * x[1] + b[2] * x[2];

  if(k % 3 ==0)
  {
    Serial.print(2*x[0]);
    Serial.print(" ");
    Serial.println(2*y[0]);
  }

  // Prints direction of motor
  if (direction)
    Serial.println("CL");
  else
    Serial.println("Anti-CL");

  // Delay so that loop updates at 1kHz
  delay(1);
  
  // Rewrite the values
  for(int i = 1; i >= 0; i--){
    x[i+1] = x[i]; // store xi
    y[i+1] = y[i]; // store yi
  }

  k++;
}


/* Function Definitions */

/**
 * @brief increments/decrements counter when signal from channel A is received.
 */
void encoderPinA(void) {
  if (digitalRead(chA) != digitalRead(chB)) {
    counter++;
    direction = 1;
  } else {
    counter--;
    direction = 0;
  }
}

/**
 * @brief increments/decrements counter when signal from channel B is received.
 */
void encoderPinB(void) {
  if (digitalRead(chA) == digitalRead(chB)) {
    counter++;
    direction = 1;
  } else {
    counter--;
    direction = 0;
  }
}