/*
  IMU Classifier

  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU, once enough samples are read, it then uses a
  TensorFlow Lite (Micro) model to try to classify the movement as a known gesture.

  Note: The use of C/C++ pointers and variable types is generally discouraged
        in Arduino examples, and in the future the TensorFlowLite library might
        change to make the sketch simpler.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.
  - Button connected to pin 3 and GND.

  Created by Don Coleman, Sandeep Mistry

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "model.h"

const int buttonPin = 3;     // the number of the pushbutton pin
const int numSamples = 119;

int previousButtonState = HIGH;
int samplesRead = numSamples;

// global variables used for TensorFlow Lite (Micro)
tflite::ErrorReporter* g_error_reporter = nullptr;
const tflite::Model* g_model = nullptr;
tflite::MicroInterpreter* g_interpreter = nullptr;
TfLiteTensor* g_input = nullptr;
TfLiteTensor* g_output = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are usings
constexpr int g_tensor_arena_size = 8 * 1024;
uint8_t g_tensor_arena[g_tensor_arena_size];

const char* GESTURES[] = {
  "punch",
  "flex"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

// The name of this function is important for Arduino compatibility.
void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input with pullup:
  pinMode(buttonPin, INPUT_PULLUP);

  // initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // print out the samples rates of the IMUs
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();

  // Set up TFLM logging via the error reporter
  static tflite::MicroErrorReporter micro_error_reporter;
  g_error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  // import the model from the model.h tab
  g_model = tflite::GetModel(model);
  if (g_model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  // pull in all the TFLM ops, you can remove this line and
  // only in the TFLM ops if would like to reduce the compile
  // size of the sketch.
  static tflite::ops::micro::AllOpsResolver resolver;

  // Create an interpreter to run the model
  static tflite::MicroInterpreter interpreter(g_model, resolver, g_tensor_arena, g_tensor_arena_size, g_error_reporter);
  g_interpreter = &interpreter;

  // Allocate memory for the model's input and output tensors
  g_interpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  g_input = g_interpreter->input(0);
  g_output = g_interpreter->output(0);
}

void loop() {
  // read the state of the push button pin:
  int buttonState = digitalRead(buttonPin);

  // compare the button state to the previous state
  // to see if it has changed
  if (buttonState != previousButtonState) {
    if (buttonState == LOW) {
      if (samplesRead == numSamples) {
        // pressed       }
        samplesRead = 0;
      }
    } else {
      // released
    }

    // store the state as the previous state, for the next loop
    previousButtonState = buttonState;
  }

  // check if the all the required samples have been read since
  // the last time the button has been pressed
  if (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      float aX, aY, aZ, gX, gY, gZ;

      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      // normalize the IMU data between 0 to 1 and store in the model's
      // input tensor
      g_input->data.f[samplesRead * 6 + 0] = (aX + 4.0) / 8.0;
      g_input->data.f[samplesRead * 6 + 1] = (aY + 4.0) / 8.0;
      g_input->data.f[samplesRead * 6 + 2] = (aZ + 4.0) / 8.0;
      g_input->data.f[samplesRead * 6 + 3] = (gX + 2000.0) / 4000.0;
      g_input->data.f[samplesRead * 6 + 4] = (gY + 2000.0) / 4000.0;
      g_input->data.f[samplesRead * 6 + 5] = (gZ + 2000.0) / 4000.0;

      samplesRead++;

      if (samplesRead == numSamples) {
        // Run inferencing
        TfLiteStatus invoke_status = g_interpreter->Invoke();
        if (invoke_status != kTfLiteOk) {
          g_error_reporter->Report("Invoke failed on x_val: %f\n", static_cast<double>(0.0));
          while (1);
          return;
        }

        // Read the predicted y value from the model's output tensor
        float y_val = g_output->data.f[0];

        for (int i = 0; i < NUM_GESTURES; i++) {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(g_output->data.f[i], 6);
        }
        Serial.println();
      }
    }
  }
}
