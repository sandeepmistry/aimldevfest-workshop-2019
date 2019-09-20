//  Hardware Test
//
//  Pressing the button turns on the built-in LED. The sketch also
//  loads the LSM9DS1 IMU library to enure the IDE is setup correctly.

#include <Arduino_LSM9DS1.h>

const int buttonPin = 3; 
const int ledPin = LED_BUILTIN;

int buttonState = 0;  
int previousButtonState = 1;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.print("Arduino ML Workshop Hardware Test");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the push button pin as an input with (internal) pullup:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // read the state of the push button pin:
  buttonState = digitalRead(buttonPin);

  // HIGH and LOW are opposite because of we are using an internal pullup resistor.
  // LOW is pressed. HIGH is released.
  
  if (buttonState == LOW) {
    // Button is pressed, turn the LED on
    digitalWrite(ledPin, HIGH);
    if (buttonState != previousButtonState) {
      Serial.println("LED is ON");      
    }
  } else {
    // Button is released, turn the LED off
    digitalWrite(ledPin, LOW);
    if (buttonState != previousButtonState) {
      Serial.println("LED is OFF");      
    }
  }

  // save the previous state of the button since we only print
  // the LED status when the state changes
  previousButtonState = buttonState;
}
