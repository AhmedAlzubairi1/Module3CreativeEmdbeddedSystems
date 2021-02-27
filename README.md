# Lab3- Maker Valentine Box: By My Valentine Version 2: Wireless!
Lab 3 for creative embedded systems

## Sources:
Go to https://git.io/JTz3m to download the master file. 
The bluetooth setup code I used is found at the code located here: Freenove_Ultimate_Starter_Kit_for_ESP32-master\Freenove_Ultimate_Starter_Kit_for_ESP32-master\C\Sketches\Sketch_29.1_BluetoothByPCM5102A

The code relating to making the rainbow sequence of the RGB LED module is found here:  https://github.com/Freenove/Freenove_WS2812_Lib_for_ESP32/blob/master/examples/Rainbow/Rainbow.ino

The documentation for both sources of code is found in this pdf file:  Freenove_Ultimate_Starter_Kit_for_ESP32-master\Freenove_Ultimate_Starter_Kit_for_ESP32-master\C\C_Tutorial

## What is it?

It is a maker inspired chocolate box, but this time I added music and made it portable. 
Connect the components under the chocolate box to give a unique chocolate box to your significant other this Valentine's day.
Once the enclosure has been created, you can send "left" string via bluetooth to turn on the LEDs in a clockwise direction. "Send "right" to turn the LEDs in a counter clockwise direction. Send "off" to turn off all the lights. Send "rainbow" to turn on the RGB Led module. If you make sure that you connect through bluetooth, you can play music by playing the music through your phone.

## Using the App (NEEDED)
First download the app here: https://www.appsapk.com/serial-bluetooth-terminal/
On your phone's bluetooth settings, pair with the device. The device is named "ESP32"
Launch the app. From the app, click the 3 lines in the top left and click devices. Under "bluetooth classic", click ESP32"
There, it should launch you to the terminal. Send the commands via text. The commands you can send are "right", "left", "off", "rainbow" to interact with the device.

## Installation 
NOTE: See the Using the APP for communicating with the enclosure 

Freenove_WS2812_Lib_for_ESP32.h was installed
To install this library, just go to your arduino IDE and click tools -> manage libraries and search "Freenove WS2812 Lib for ESP32". Download that library.

## Setting up Wiring
Components used:

  - 4 Red LED
  - Freenove 8 RGB LED Module
  - Speaker
  - PCM5102A chip audio converter and amplifier 
  - ESP32-WROVER
  - 3.7v 650mAh battery

For the specific wiring, here is the digital connection layout for all the components:

    - Connect the 4 Red LEDs to pins 33, 32, 15, 0
    - Connect battery to ground and VCC/5V of ESP32
    - Connect RGB LED module signal cable to 2, and connect the power to 3.3v and ground to ground
    - Connect Speaker + to L+ on the PCM5102A
    - Connect Speaker - to L- on the PCM5102A
    - Connect the VCC and GND of the PCM5102A to the GND and 5v of the ESP32
    - Connect the LCK of the PCM5102A to pin 25
    - Connect the DIN of the PCM5102A to pin 22
    - Connect the BCK of the PCM5102A to pin 26

## Running the Program
This enclosure was created using a **ESP32-WROVER** and doesn't require a script to run specicially. Open the Sketch_29.1_BluetoothByPCM5102A.ino file using the Arduino IDE and select the ESP32-WROVER board as one of the options and then launch the code file.
Also, Go to tools tab and make these changes: 
Set the Flash Mode to DOUT
Set the Partition Scheme to Huge App
Set the flash frequecy to 80MHz

## More Documentation

The code has been very well documented through comments. See for more info. 

## Demo:
https://www.youtube.com/watch?v=415s_Y9ncGs&ab_channel=AhmedAlzubairi


## Bugs:

No bugs were found during the process. 
