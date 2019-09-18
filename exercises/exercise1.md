# Exercise 1 - Development Environment

Set up your computer for Arduino development.

## Arduino IDE
Install the Arduino IDE from https://arduino.cc/downloads

![Arduino IDE Download](../images/ArduinoIDE.png)
        
### Arduino SAMD Board Definitions
Use the Arduino Boards Manager to install the Arduino SAMD Board definitions. Open the Boards Manager using the menu _Tools -> Boards -> Boards Manager_

![Arduino Boards Manager](../images/BoardManager-Menu.png)

Search for "1010" and install the Arduino SAMD Boards (32-bit ARM Cortex-M0+) definitions.

![Arduino SAMD Board Definitions](../images/BoardsManager.png)
        
### Arduino Libraries        
Install the following Arduino Libraries using the Library manager. 

* WiFiNINA library (search for "NINA")
* Arduino MQTT Client library (search for "ArduinoMQTTClient")
* Arduino BearSSL library (search for "ECCX")
* Arduino ECCX08  library (search for "ECCX")
* DHT sensor library by Adafruit (search for "DHT")
* Adafruit Unified Sensor library (search for "adafruit abstraction")

Open the library manager using the menu _Sketch -> Include Libraries -> Manage Libraries_

![Arduino Library Manager Menu](../images/ManageLibraries.png)

Search for "NINA" and install the WiFiNINA library.

![Arduino Library Manager WiFiNINA](../images/library-wifinina.png)

Search for "ArduinoMQTTClient" and install the Arduino MQTT Client library.

![Arduino Library Manager MQTT](../images/library-arduinomqttclient.png)

Search for "ECCX" and install the ArduinoBearSSL library and the ArduinoECCX08 library.

![Arduino Library Manager Search ECCX](../images/library-eccx.png)

Search for "DHT" and install the DHT sensor library by Adafruit.

![Arduino Library Manager Adafruit DHT](../images/library-dht.png)

Search for "adafruit abstraction" and install the Adafruit Unified Sensor library.

![Arduino Library Manager Adafruit Unified Sensor](../images/library-AdafruitUnifiedSensor.png)

Next [Exercise 2: Assemble the Hardware](exercise2.md)