#include <DinkLink.h>

const char* WIFI_SSID = "HufflePuff";
const char* WIFI_PASS = "Marcus2909!";
const char* SERVER_IP = "192.168.4.74";  // Replace with your PC's IP
const char* DEVICE_ID = "test";
bool NoUBoard = true;

DinkLink dink(WIFI_SSID, WIFI_PASS, SERVER_IP, DEVICE_ID, NoUBoard);  // server port 8080

void setup() {
  Serial.begin(115200);
  dink.begin();
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  Serial.println("Started");
}

void loop() {
  dink.loop();

  // Print held buttons
  for (uint8_t i = 0; i < 12; i++) {
    if (dink.buttonHeld(i)) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" is held");
    }
  }

  // Print joystick axes
  float x = dink.getAxis(0);
  float y = dink.getAxis(1);

  if (x != 0 || y != 0) {
    Serial.print("Axes: X=");
    Serial.print(x, 2);
    Serial.print(" Y=");
    Serial.println(y, 2);
  }

  // delay(100);  // Adjust for smoother logging
}
