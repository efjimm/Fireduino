#include "arduino_secrets.h"
#include <SPI.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ArduinoIoTCloud.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h>

#include <stdarg.h>
#include <stdint.h>
#include <time.h>

#include "thingProperties.h"

#define SEVENTY_YEARS 2208988800UL

static int serialPrintf(const char *fmt, ...);
static time_t fetchTime(void);

hd44780_pinIO lcd(12, 11, 2, 3, 4, 5);
WiFiUDP udp;
const uint16_t LOCAL_PORT = 2390;

static char msg[48] = {0};
const auto buzzer_pin = 6;
const int thermistor_pin = A1;

uint8_t degree_symbol[8] = {
	0b00100,
	0b01010,
	0b00100,
};

void
setup(void) {
	Serial.begin(9600);
	pinMode(thermistor_pin, INPUT);
	
	// Arduino Cloud
	initProperties();
	ArduinoCloud.begin(ArduinoIoTPreferredConnection);

	Serial.println("Connecting to the Arduino cloud...");

	while (!ArduinoCloud.connected()) {
		ArduinoCloud.update();
		delay(1000);
	}

	analogWrite(A3, 50);
	lcd.begin(16, 2);
	lcd.createChar(0, degree_symbol);
	lcd.print("Connecting...");

	const uint32_t begin = millis();
	while (millis() - begin < 10000) {
		ArduinoCloud.update();
		delay(1000);
	}

	udp.begin(LOCAL_PORT);

	setSyncProvider(fetchTime);
	setSyncInterval(3600); // Resync every hour

	tone(buzzer_pin, 512, 500);
}

void
loop(void) {
	ArduinoCloud.update();

	static char buf[32];
	const auto temp = now();
	strftime(buf, sizeof(buf), "%F %H:%M:%S", gmtime(&temp));

	const float voltage = analogRead(thermistor_pin) * 3.3 / 1024;
	temperature = (voltage - 0.55) * 100;

	if (temperature >= buzzer_temp_high) {
		tone(buzzer_pin, buzzer_temp_high_pitch, 250);
	} else if (temperature <= buzzer_temp_low) {
		tone(buzzer_pin, buzzer_temp_low_pitch, 250);
	}

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(temperature);
	lcd.write((uint8_t)0);
	lcd.print("C");

	lcd.setCursor(0, 1);
	lcd.print(buf);

	Serial.print(temperature);
	serialPrintf(" : %s\n", buf);

	ArduinoCloud.update();
	delay(1000);
}

void
pingTimeServer(IPAddress ip) {
	memset(msg, 0, sizeof(msg));

	msg[0] |= 0b11000000; // bits 1-2: Set leap second indicator to 3 (unsynchronised)
	msg[0] |= 0b00100000; // bits 3-5: Set NTP version to 4
	msg[0] |= 0b00000011; // bits 6-8: Set NTP packet mode to 3 (client)

	msg[1] = 0; // Leave stratum unspecified in client packet
	msg[2] = 6; // Poll interval
	msg[3] = 0xEC; // Peer Clock Precision

	Serial.print("Pinging ntp server at ");
	Serial.println(ip);

	udp.beginPacket(ip, 123);
	udp.write(msg, 48);
	udp.endPacket();
}

time_t
fetchTime(void) {
	IPAddress ip;
	WiFi.hostByName(NTP_SERVER, ip);
	pingTimeServer(ip);

	const auto begin = millis();
	uint32_t packet_size;

	// Wait either until we get a response or 10 seconds has passed
	while (millis() - begin < 10000 && (packet_size = udp.parsePacket()) == 0);

	// Got a response
	if (packet_size > 0 || udp.parsePacket() > 0) {
		udp.read(msg, sizeof(msg));
		// The received timestamp is big endian - convert to little endian.
		const uint32_t seconds = msg[40] << 24 | msg[41] << 16 | msg[42] << 8 | msg[43];

		// NTP returns the number of seconds since January 1 1900. The C99 time functions work with
		// seconds since the unix epoch (January 1 1970).
		return seconds - SEVENTY_YEARS;
	}

	return 0;
}

// A wrapper around sprintf and Serial.print. Max buffer size of 256 bytes.
int
serialPrintf(const char *fmt, ...) {
	static char buf[128];
	va_list args;
	va_start(args, fmt);

	const int ret = vsnprintf(buf, sizeof(buf), fmt, args);

	va_end(args);

	Serial.print(buf);

	return ret;
}
