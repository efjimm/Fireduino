#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char WIFI_SSID[] = SECRET_WIFI_SSID;    // Network SSID (name)
const char WIFI_PASS[] = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char NTP_SERVER[] = "ie.pool.ntp.org";

String date;

void initProperties(){
  ArduinoCloud.addProperty(date, READ, 1 * SECONDS, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PASS);
