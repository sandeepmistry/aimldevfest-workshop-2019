# Exercise 2: Assemble the Hardware

There's no wiring for this project. 

1. Remove the Arduino Nano 33 Sense board from the box
1. Hold the Arduino with the USB connector toward the right
1. Plug the Arduino into the top of the mini solderless breadboard
1. You should see 4 rows of holes below the board
1. Take the button and gently insert it into the breadboard
1. The left side of the button should be in the ground pin, which is denoted by a white mark
1. The right side of the button should be in pin D3
1. Unfortuantely the pin labels are on the bottom of the board, so match your setup with the picture above.

![Picture of assembled hardware](../images/assembled-hardware.png)

## Hardware Test

1. Plug the micro USB cable into the board and your computer
1. Open arduino/HardwareTest/HardwareTest.ino in the Arduino IDE
1. Choose the board _Tools -> Board -> Arduino Nano 33 BLE_
1. Choose the port _Tools -> Port -> COM5 (Arduino Nano 33 BLE)_ Note that the actual port may be different on your computer
1. Upload the code to the board _Sketch -> Upload_
1. The LED should be a solid color
1. Open the serial monitor _Tools -> Serial Monitor_
1. **TODO FIX NEXT 2 STEPS**
1. The LED should start blinking and you should see temperature, humidity, CdS, button, & servo data in the serial console
1. To stop the program press the RST button on the board.
    * To restart the test, close the _Serial Monitor_ dialog and open it again.

![Arduino serial monitor with hardware test results](../images/hardware-test.png)

## LSM9DS1 Examples (Optional)

You can try the example sketches that came with the LSM9DS1 library.

1. Open the Simple Accelerometer sketch using _File -> Examples -> Arduino_LSM9DS1 -> SimpleAccelerometer_
1. Upload the sketch to the board using the _Sketch -> Upload_ menu or the right arrow button from the tool bar.
1. Open the Serial Monitor _Tools -> Serial Monitor_ to view the text output
1. Open the Serial Plotter _Tools -> Serial Plotter_ to view the output on a graph

![Arduino LSM9DS1 Examples](../images/lsm9ds1-examples.png)

![Arduino Serial Plotter Output with Accelerometer Data](../images/accelerometer-example-serial-plotter.png)

Next [Exercise 3: Visualize the IMU Data](exercise3.md)
