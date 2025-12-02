#include <Alfredo_NoU3.h>
#include <DinkLink.h>

// DinkLink initialization (you may need to adjust)
// If this lib doesn't work then get the library WebSockets by Markus Sattler by going into the library manager on the left side and searching "WebSockets"
const char* WIFI_SSID = "Put Wifi name";
const char* WIFI_PASS = "Put Wifi password";
const char* SERVER_IP = "Put IP here";
const char* DEVICE_ID = "test"; // put your id here (it will be used on the link page)
bool NoUBoard = true; // Debug power mode, ignore

DinkLink dink(WIFI_SSID, WIFI_PASS, SERVER_IP, DEVICE_ID, NoUBoard);  // Server port 8080 (e.g. http://xxx.xxx.x.xx:8080/ or if the pc is hosting http://localhost:8080/)

// Motor initialization (replace with your port)
NoU_Motor encoderedMotor(2);

// Variables
unsigned long lastPrintTime = 0;
float wantedAngle = 0;
const float speed = 0.5;
const int deadzone = 5;
const int fullRotation = 815; // May need to change (see explantation for "fullRotation")

void setup() {
  Serial.begin(115200);
  NoU3.begin();
  encoderedMotor.beginEncoder();
  dink.begin();
  Serial.println("Started");
}

void loop() {
  dink.loop();

  int rotation = encoderedMotor.getPosition();

  if (lastPrintTime + 100 < millis()) {
    Serial.print("Motor pos: ");
    Serial.print(rotation);
    Serial.print("    Wanted angle pos: ");
    Serial.println(wantedAngle);
    lastPrintTime = millis();
  }

  // Press B0 to rotate one full rotation
  if (dink.buttonPressed(0)) {
    wantedAngle += fullRotation;
    Serial.println("Changed angle");
  } 

  // May be broken (see explantation at these lines in explantation (wow)) 
  if (rotation > wantedAngle + deadzone) {
    encoderedMotor.set(speed);
  } else if (rotation < wantedAngle - deadzone) {
    encoderedMotor.set(-speed);
  } else {
    encoderedMotor.set(0);
  }
}
