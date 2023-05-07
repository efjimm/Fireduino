#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

#include <stdarg.h>
#include <stdint.h>
#include <time.h>

static int serialPrintf(const char *fmt, ...);
static time_t getTime();

WiFiUDP udp;
const uint16_t LOCAL_PORT = 2390;

uint32_t current_time = 0;
char msg[48] = {0};

void
setup(void) {
	Serial.begin(9600);

	int status = WL_IDLE_STATUS;
	while (status != WL_CONNECTED) {
		serialPrintf("Attempting to connect to ssid: '%s'\nlast status: %d\n", WIFI_SSID, status);

#ifdef WIFI_PASS
		status = WiFi.begin(WIFI_SSID, WIFI_PASS);
#else
		status = WiFi.begin(WIFI_SSID);
#endif

		delay(2000);
	}

	Serial.println("Connected");

	udp.begin(LOCAL_PORT);

	setSyncProvider(getTime);
	setSyncInterval(3600); // Resync every hour
}

void
loop(void) {
	static char buf[64];
	const time_t temp = now();
	const auto time = ctime(&temp);
	serialPrintf("%s", time);

	delay(1000);
}

void
pingTimeServer(IPAddress ip) {
	memset(msg, 0, sizeof(msg));

	msg[0] = 0b11100011;   // LI, Version, Mode
	msg[1] = 0;     // Stratum, or type of clock
	msg[2] = 6;     // Polling Interval
	msg[3] = 0xEC;  // Peer Clock Precision

	Serial.print("Pinging ntp server at ");
	Serial.println(ip);

	udp.beginPacket(ip, 123);
	udp.write(msg, 48);
	udp.endPacket();
}

time_t
getTime() {
	IPAddress ip;
	WiFi.hostByName(NTP_SERVER, ip);
	pingTimeServer(ip);

	const auto begin = millis();
	uint32_t packet_size;
	while (millis() - begin < 2000 && (packet_size = udp.parsePacket()) == 0);
	if (packet_size > 0 || udp.parsePacket() > 0) {
		udp.read(msg, sizeof(msg));

		const uint32_t seconds = msg[40] << 24 | msg[41] << 16 | msg[42] << 8 | msg[43];
		return seconds - 2208988800UL;
	}

	return 0;
}

// A wrapper around sprintf and Serial.print. Max buffer size of 256 bytes.
int
serialPrintf(const char *fmt, ...) {
	static char buf[256];
	va_list args;
	va_start(args, fmt);

	const int ret = vsnprintf(buf, sizeof(buf), fmt, args);

	va_end(args);

	Serial.print(buf);

	return ret;
}
