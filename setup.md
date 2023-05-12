---
layout: default
title: Setup
---

# Setup

### Circuit

Set up the circuit like so:

![Circuit](fireduino_circuit.png)

There are many connections here - verify that they are correct before proceeding.

### Arduino Cloud

Set up your Arduino to use the Arduino Cloud. To do so, sign in to
[Arduino Cloud](https://cloud.arduino.cc). Go to "IoT Cloud", and under the "devices" tab, add a
device. Download and install the Arduino Create Agent and run it, and connect your Arduino to your
PC via USB. The Arduino should be detected and you can add it as a device. Proceed to create a new
"thing" under the things tab, and associate the device with it.

As far as I can tell, there is no way to import the full configuration for a "thing" into the
Arduino Cloud, so you must set the variables manually. Create 5 variables:

- *temperature*, of type **CloudTemperatureSensor**, read only, updated every 5 seconds
- *buzzer_temp_low*, of type **float**, read & write, updated on change
- *buzzer_temp_high*, of type **float**, read & write, updated on change
- *buzzer_temp_low_pitch*, of type **float**, read & write, updated on change
- *buzzer_temp_high_pitch*, of type **float**, read & write, updated on change

You can view and manipulate these variables in a dashboard.

### Uploading the Sketch

Note that this must be done *after* you have set up your device for Arduino Cloud.

The easiest way to upload the sketch is to run the provided "upload.sh" script. You may need to
change the **PORT** variable in this script to the correct port for your device. The script prompts
for the root password using sudo, as this is required to write data over the port.

You can also upload the sketch via the Arduino web editor. This requires having the Arduino Create
Agent running. To import the sketch into the web editor, you can click the import button near the
top left of the screen. It requires a zip file containing the following files:

- sketch.json
- fireduino.ino
- thingProperties.h
- arduino_secrets.h

Once this is imported, you can verify & upload the sketch to the device via the web editor. You may
be required to set the wifi ssid and password again.