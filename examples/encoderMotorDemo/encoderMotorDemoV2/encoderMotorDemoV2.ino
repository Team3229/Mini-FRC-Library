#include <Alfredo_NoU3.h>
#include <DinkLink.h>

// DinkLink initialization (you may need to adjust)
// If this lib doesn't work then get the library WebSockets by Markus Sattler by going into the library manager on the left side and searching "WebSockets"
const char* WIFI_SSID = "HawktimusDotNet";    // Wifi name
const char* WIFI_PASS = "Hawk3229";           // Wifi password
const char* SERVER_IP = "hawktimus.local";    // Server ip that is running DinkLink
const char* DEVICE_ID = "Your Bot Name Here"; // Put your bots name here, this will be used later to connect
bool NoUBoard = true;                         // If you're using a NoU* board (only for debugging with any other boards. Keep this true)

DinkLink dink(WIFI_SSID, WIFI_PASS, SERVER_IP, DEVICE_ID, NoUBoard);  // Server port 8080 (e.g. http://xxx.xxx.x.xx:8080/ or if the pc is hosting http://localhost:8080/)

// Motor initialization (replace with your port)
NoU_Motor encoderMotor(5);

// Variables
unsigned long lastPrintTime = 0;
float wantedAngle = 0;
const float speed = 0.5;
const int deadzone = 5;
const int fullRotation = 815; // May need to change (see explantation for "fullRotation")

void setup() {
  Serial.begin(115200);
  NoU3.begin();
  encoderMotor.beginEncoder();
  dink.begin();
  Serial.println("Started");
}

void adjustAngle(NoU_Motor* motor, int angle, float wanted, int deadzone) {
  // Calculate error (flip sign if your encoder direction is reversed)
  int error = (wanted - angle) * -1;

  // Stop if within deadzone
  if (abs(error) <= deadzone) {
    motor->set(0);
    return;
  }

  // Proportional gain constant (tune this)
  float kP = 0.001; // try values between 0.001 and 0.005

  // Scale speed proportionally to error
  float adjustedSpeed = error * kP;

  // Clamp to max speed
  if (adjustedSpeed > speed) adjustedSpeed = speed;
  if (adjustedSpeed < -speed) adjustedSpeed = -speed;

  // Apply a minimum speed floor so the motor doesn’t stall
  if (abs(adjustedSpeed) < 0.2) {
    adjustedSpeed = (adjustedSpeed > 0 ? 0.2 : -0.2);
  }

  motor->set(adjustedSpeed);
}

void loop() {
  dink.loop();

  int rotation = encoderMotor.getPosition();

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

  adjustAngle(&encoderMotor, rotation, wantedAngle, deadzone);
}
