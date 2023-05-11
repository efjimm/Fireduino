#!/bin/sh
FQBN="arduino:samd:mkr1000"
PORT="/dev/ttyACM0"

arduino-cli lib install Time ArduinoIoTCloud
arduino-cli compile . --fqbn "${FQBN}"
sudo arduino-cli upload . --fqbn "${FQBN}" --port="${PORT}"
