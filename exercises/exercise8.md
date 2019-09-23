# Excercise 8: Next Steps

Now that you have this working... 

- Load the code to record gestures. Create additional CSV files with more gestures. Retrain the model in colab. Load the new model back onto the Arduino.
  - Note: you'll need to edit the code to add the names of the new geture files

- Try increasing and decreasing the number of recordings per gesture, how does this impact performance?

- Try to only use the accelerometer or gyroscope data (not both), how does this impact performance?

- Tweak the model structure and parameters
  - Can you get better results?
  - Can you reduce the size and still get "good" results

- Try quantizaton to reduce the model size and/or avoid floating point math: https://www.tensorflow.org/lite/performance/post_training_quantization

- Grab a [board](https://store.arduino.cc/usa/nano-33-ble-sense) and the [TinyML book](http://shop.oreilly.com/product/0636920254508.do) to continue at home.

