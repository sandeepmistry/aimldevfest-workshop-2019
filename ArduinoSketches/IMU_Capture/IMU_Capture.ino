#include <Arduino_LSM9DS1.h>

const int buttonPin = 3;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int numSamples = 119;

int previousButtonState = HIGH;
int samplesRead = numSamples;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input with pullup:
  pinMode(buttonPin, INPUT_PULLUP);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState != previousButtonState) {
    if (buttonState == LOW) {
      if (samplesRead == numSamples) {
        // pressed
        samplesRead = 0;
      }
    } else {
      // released
    }

    previousButtonState = buttonState;
  }

  if (samplesRead < numSamples) {
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      float aX, aY, aZ, gX, gY, gZ;

      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++;

      Serial.print(aX, 3);
      Serial.print(',');
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();

      if (samplesRead == numSamples) {
        Serial.println();
      }
    }
  }
}
