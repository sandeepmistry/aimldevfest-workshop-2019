#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "model.h"

const int buttonPin = 3;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int numSamples = 119;

int previousButtonState = HIGH;
int samplesRead = numSamples;

// Globals, used for compatibility with Arduino-style sketches.
tflite::ErrorReporter* g_error_reporter = nullptr;
const tflite::Model* g_model = nullptr;
tflite::MicroInterpreter* g_interpreter = nullptr;
TfLiteTensor* g_input = nullptr;
TfLiteTensor* g_output = nullptr;
int g_inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
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

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();

  // Set up logging
  static tflite::MicroErrorReporter micro_error_reporter;
  g_error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  g_model = tflite::GetModel(model);
  if (g_model->version() != TFLITE_SCHEMA_VERSION) {
    g_error_reporter->Report(
      "Model provided is schema version %d not equal "
      "to supported version %d.\n",
      g_model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need
  static tflite::ops::micro::AllOpsResolver resolver;

  // Build an interpreter to run the model with
  static tflite::MicroInterpreter interpreter(
    g_model, resolver, g_tensor_arena, g_tensor_arena_size, g_error_reporter);
  g_interpreter = &interpreter;

  // Allocate memory from the tensor_arena for the model's tensors
  g_interpreter->AllocateTensors();

  // Obtain pointers to the model's input and output tensors
  g_input = g_interpreter->input(0);
  g_output = g_interpreter->output(0);


  // Keep track of how many inferences we have performed
  g_inference_count = 0;
}

// The name of this function is important for Arduino compatibility.
void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState != previousButtonState) {
    if (buttonState == LOW) {
      if (samplesRead == numSamples) {
        // pressed       }
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

      g_input->data.f[samplesRead * 6 + 0] = aX;
      g_input->data.f[samplesRead * 6 + 1] = aY;
      g_input->data.f[samplesRead * 6 + 2] = aZ;
      g_input->data.f[samplesRead * 6 + 3] = gX;
      g_input->data.f[samplesRead * 6 + 4] = gY;
      g_input->data.f[samplesRead * 6 + 5] = gZ;

      samplesRead++;

      if (samplesRead == numSamples) {
        // Run inference, and report any error
        TfLiteStatus invoke_status = g_interpreter->Invoke();
        if (invoke_status != kTfLiteOk) {
          g_error_reporter->Report("Invoke failed on x_val: %f\n",
                                   static_cast<double>(0.0));
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
