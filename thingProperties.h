#include "arduino_secrets.h"
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

static void onBuzzerTempLowChange(void) {}
static void onBuzzerTempHighChange(void) {}
static void onBuzzerTempLowPitchChange(void) {}
static void onBuzzerTempHighPitchChange(void) {}

const char WIFI_SSID[] = SECRET_WIFI_SSID;    // Network SSID (name)
const char WIFI_PASS[] = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char NTP_SERVER[] = "ie.pool.ntp.org";

CloudTemperatureSensor temperature;
float buzzer_temp_low = -100;
float buzzer_temp_high = 100;
int buzzer_temp_low_pitch = 512;
int buzzer_temp_high_pitch = 4096;

void initProperties(){
  ArduinoCloud.addProperty(buzzer_temp_low, READWRITE, ON_CHANGE, onBuzzerTempLowChange);
  ArduinoCloud.addProperty(buzzer_temp_high, READWRITE, ON_CHANGE, onBuzzerTempHighChange);
  ArduinoCloud.addProperty(buzzer_temp_low_pitch, READWRITE, ON_CHANGE, onBuzzerTempLowPitchChange);
  ArduinoCloud.addProperty(buzzer_temp_high_pitch, READWRITE, ON_CHANGE, onBuzzerTempHighPitchChange);
  ArduinoCloud.addProperty(temperature, READ, 5 * SECONDS, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PASS);
