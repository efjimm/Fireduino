#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

static void onBuzzerTempLowChange(void) {}
static void onBuzzerTempHighChange(void) {}

const char WIFI_SSID[] = SECRET_WIFI_SSID;    // Network SSID (name)
const char WIFI_PASS[] = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char NTP_SERVER[] = "ie.pool.ntp.org";

CloudTemperatureSensor temperature;
float buzzer_temp_low = INT_MIN;
float buzzer_temp_high = INT_MAX;

void initProperties(){
  ArduinoCloud.addProperty(buzzer_temp_low, READWRITE, ON_CHANGE, onBuzzerTempLowChange);
  ArduinoCloud.addProperty(buzzer_temp_high, READWRITE, ON_CHANGE, onBuzzerTempHighChange);
  ArduinoCloud.addProperty(temperature, READ, 5 * SECONDS, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PASS);
