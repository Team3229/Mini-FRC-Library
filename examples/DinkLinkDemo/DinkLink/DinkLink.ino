#include <DinkLink.h>

// idk if this works get back to me

const char* WIFI_SSID = "Put Wifi name";
const char* WIFI_PASS = "Put Wifi password";
const char* SERVER_IP = "Put IP here";
const char* DEVICE_ID = "test"; // put your id here (it will be used on the link page)
bool NoUBoard = true; // Debug power mode, ignore

DinkLink dink(WIFI_SSID, WIFI_PASS, SERVER_IP, DEVICE_ID, NoUBoard);  // server port 8080

void setup() {
  Serial.begin(115200);
  dink.begin();
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

  // delay(100);  // Adjust for smoother logging and stuff
}
