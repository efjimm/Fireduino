#!/bin/sh
FQBN="arduino:samd:mkr1000"
PORT="/dev/ttyACM0"

WIFI_SSID="note"
WIFI_PASS="123456789"
NTP_SERVER="ie.pool.ntp.org"

CPPFLAGS="-DWIFI_SSID=\"${WIFI_SSID}\" -DNTP_SERVER=\"${NTP_SERVER}\""
if [ "${WIFI_PASS}" != "" ]
then
	CPPFLAGS="${CPPFLAGS} -DWIFI_PASS=\"${WIFI_PASS}\""
fi

arduino-cli lib install Time WiFi101
arduino-cli compile sketch --fqbn "${FQBN}" --build-property "compiler.cpp.extra_flags=${CPPFLAGS}"
sudo arduino-cli upload sketch --fqbn "${FQBN}" --port="${PORT}"
