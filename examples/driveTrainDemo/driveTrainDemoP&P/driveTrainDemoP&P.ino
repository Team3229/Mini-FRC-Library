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

// motor initialization (change ports to match)
NoU_Motor frontLeftMotor(8);
NoU_Motor frontRightMotor(1);
NoU_Motor rearLeftMotor(7);
NoU_Motor rearRightMotor(2);

NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);

// Variables (play with)
const float speed = 0.8;
const float deadzone = 0.2;
const bool mobile = false;

void setup() {
  Serial.begin(115200);
  dink.begin();
  NoU3.begin();
  // Needed if every motor faces the center (your robot probably)
  frontRightMotor.setInverted(true); 
  rearRightMotor.setInverted(true);
  // Serial debug
  Serial.println("Started");
}

void loop() {
  dink.loop();

  // Init the drivetrain values
  float x = 0;
  float y = 0;

  // If one or more joysticks (controller mode has the left stick up/down being forward/backward and right stick left/right being counterclockwise/clockwise)
  if (mobile) {
    x = dink.getAxis(0);
    y = -dink.getAxis(1); // DinkLink y is inverted so -reading is best fit
  } else {
    x = dink.getAxis(2);
    y = -dink.getAxis(1); // DinkLink y is inverted so -reading is best fit
  }

  float throttle = 0;
  float rotation = 0;

  // If joystick is outside the deadzone
  if (abs(x) > deadzone) {
    rotation = x * speed;
    Serial.print("Rotation set: ");
    Serial.println(rotation);
  }

  // If joystick is outside the deadzone
  if (abs(y) > deadzone) {
    throttle = y * speed; 
    Serial.print("Throttle set: ");
    Serial.println(throttle);
  }

  drivetrain.arcadeDrive(throttle, rotation);
}
