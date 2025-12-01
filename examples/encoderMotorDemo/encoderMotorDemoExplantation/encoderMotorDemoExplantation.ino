#include <Alfredo_NoU3.h>

/*
WARNING THIS SCRIPT HAS INTENTIONAL ERRORS, read me:
- This script doesn't have libraries like PestoLink or DinkLink setup
- Any "replaceMe" variables should have an explanation and example
- Activation keys should use a 3rd party controller like PestoLink or DinkLink to activate a robot function 
- Anything with "this is a debug printer that can be removed" can be removed (wow)
PLEASE read the user manual as it might cover bugs or needed actions
*/

/*
User manual (a list of what the user (you) needs to fix/implement):
- Motor port
- Full rotation amount
- Button activation

Maybe fix (I don't know if this was just my issue or a wide issue):
- Motor speed signs
- Pestolink not working with button activation as it doesn't have a "buttonPressed", leading to rapid activation (aka bad)

How to implement (this covers simple modules which the user can fix):
- Button activation
  To create the "Button activation" replace the comment with something like 
  dink.buttonPressed(0) == true
  Which tells the robot to only spin the motor when button 0 is pressed

- fullRotation setting
  To calculate the "fullRotation" value boot up this example script, make sure you can read the Serial (aka console), and spin the motor one rotation
  The Serial will show you the approximated angle of the motor (it doesn't have to be perfect but try and be precise)
  This adjust the encoder angle to about one full rotation but we are not done yet
  I suggest putting a type of marker to easily read the angle from now on
  Now you are going to change the value of fullRotation to the approximated angle from before and deploy this new code
  Now spin the motor using the button activation, if it goes over a full rotation then turn it down, if it goes under a full rotation then turn it up, turn the motor to the start position and reset the bot (remember the amount it spun)
  Repeat this last step until the motor noticeably can perform multiable rotations and still be facing the same angle, this is your final "fullRotation" value

- Deadzone and speed
  These values are mainly for adjusting the motor to the "wanted angle"
  a higher speed and deadzone may make the motor unaccurate but have no jitter
  A lower speed and deadzone may make the motor accurate but be slow or rapidly adjust small unneeded angles (I will call this jitter from now on)
  A higher speed and lower deadzone may make the motor accurate but rapidly jitter
  A lower speed and higher deadzone may make the motor less accurate but have no jitter
  The perfect balance can be found by playing with the values but the given values are most likely ok
*/

// Motor initialization
NoU_Motor encoderedMotor(replaceMe); // Replace with encoder port (e.g. 2, 3, 4, 5, 6, 7)

// Debug variables (not needed and can be deleted in final production) (meant for user readability)
unsigned long lastPrintTime = 0;
float wantedAngle = 0;

// Initialization variables (not needed and can be changed per user) (meant for user to adjustability)
const float speed = 0.5; // Adjustable
const int deadzone = 5; // Optional but adjust smoothness and prevents motor jitter
const int fullRotation = 815; // Needed to be tuned to every motor as different motors might have different results

void setup() {
  Serial.begin(115200); // Begin debug print (press "Serial Monitor" to use while plugged into a board)
  NoU3.begin(); // Motor/board initialization
  encoderedMotor.beginEncoder(); // Encoder motor initialization
  Serial.println("Started"); // Line print to show the code has started (this is a debug printer that can be removed)
}

void loop() {
  // Gets the rotation of the encoded motor
  int rotation = encoderedMotor.getPosition();

  // Prints the current angle and wanted angle every 100 ms (1/10 of a second) (this is a debug printer that can be removed)
  if (lastPrintTime + 100 < millis()) {
    Serial.print("Motor pos: ");
    Serial.print(encoderedMotor.getPosition());
    Serial.print("    Wanted angle pos: ");
    Serial.println(wantedAngle);
    lastPrintTime = millis();
  }

  // Does a full rotation of a motor
  if (dink.buttonPressed(0) /*Button activation (PestoLink button held or DinkLink button pressed)*/) {
    wantedAngle += fullRotation;
    Serial.println("Changed angle"); // this is a debug printer that can be removed
  } 

  // Adjust the motor to align with the "wanted angle"
  // WARNING, if the motor constantly spins then you might need to switch which speed is negative. I had a bunch of problems with this so this code might be inverted
  if (rotation > wantedAngle + deadzone) {
    encoderedMotor.set(speed);
  } else if (rotation < wantedAngle - deadzone) {
    encoderedMotor.set(-speed);
  } else {
    encoderedMotor.set(0);
  }
}
