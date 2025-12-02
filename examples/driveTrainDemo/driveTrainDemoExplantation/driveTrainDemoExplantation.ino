#include <Alfredo_NoU3.h>

/*
WARNING THIS SCRIPT HAS INTENTIONAL ERRORS, read me:
- This script doesn't have libraries like PestoLink or DinkLink setup
- Any "replaceMe" variables should have an explanation and example
- Activation keys should use a 3rd party controller like PestoLink or DinkLink to activate a robot function 
- Anything with "this is a debug printer that can be removed" can be removed (wow)
- Motors such as wheels can run away and break themselves so please control them
PLEASE read the user manual as it might cover bugs or needed actions
*/

/*
User manual (a list of what the user (you) needs to fix/implement):
- Motor port
- Axis value
- Wheel initialization

Maybe fix (I don't know if this was just my issue or a wide issue):
- Nothing (yippee)

How to implement (this covers simple modules which the user can fix):
- Axis value
  The best way to do this is read the documentation of the link system you are using
  For example the values for DinkLink and PestoLink are: 
  float x = dink.getAxis(0); 
  float y = -dink.getAxis(1); (DinkLink Y is inverted)

  float x = PestoLink.getAxis(0); 
  float y = PestoLink.getAxis(1);

- Deadzone and speed
  These values are mainly for adjusting the motor to the "wanted angle"
  a higher speed and deadzone may make the motor unaccurate but have no jitter
  A lower speed and deadzone may make the motor accurate but be slow or rapidly adjust small unneeded angles (I will call this jitter from now on)
  A higher speed and lower deadzone may make the motor accurate but rapidly jitter
  A lower speed and higher deadzone may make the motor less accurate but have no jitter
  The perfect balance can be found by playing with the values but the given values are most likely ok

- Wheel init
  This is mainly human error and can be solved by testing with inversions and ports
  Make sure you check every wire and port
  You might have to play with which side is inverted
*/

// Motor initialization
NoU_Motor frontLeftMotor(replaceMe);    // Replace with encoder port (e.g. 1-8)
NoU_Motor frontRightMotor(replaceMe);   // Replace with encoder port (e.g. 1-8)
NoU_Motor rearLeftMotor(replaceMe);     // Replace with encoder port (e.g. 1-8)
NoU_Motor rearRightMotor(replaceMe);    // Replace with encoder port (e.g. 1-8)

// Creates a sync with every motor
NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);

// Initialization variables (not needed and can be changed per user) (meant for user to adjustability)
const float speed = 0.8; // Adjustable
const int deadzone = 0.1; // Optional but adjust smoothness and prevents motor jitter/controller drift

void setup() {
  Serial.begin(115200); // Begin debug print (press "Serial Monitor" to use while plugged into a board)
  NoU3.begin(); // Motor/board initialization
  frontRightMotor.setInverted(true); // Needed if every motor faces the center (your robot probably)
  rearRightMotor.setInverted(true);  // Needed if every motor faces the center (your robot probably)
  Serial.println("Started"); // Line print to show the code has started (this is a debug printer that can be removed)
}

void loop() {
  // Axis initialization
  float x = replaceMe; // Axis reading (PestoLink get axis or DinkLink get axis)
  float y = replaceMe; // Axis reading (PestoLink get axis or DinkLink get axis (Remember DinkLink Y is inverted)) 

  // Init the drivetrain values
  float throttle = 0;
  float rotation = 0;

  // If axis is outside the deadzone then set the rotation to the axis value times the speed
  if (abs(x) > deadzone) {
    rotation = x * speed;
    Serial.print("Rotation set: ");
    Serial.println(rotation);
  }

  // If axis is outside the deadzone then set the throttle to the axis value times the speed
  if (abs(y) > deadzone) {
    throttle = y * speed;
    Serial.print("Throttle set: ");
    Serial.println(throttle);
  }

  // Move using the initialized drivetrain
  drivetrain.arcadeDrive(throttle, rotation);
}
