This is my library which is free to use

Requires the Arduino library WebSockets by Markus Sattler to be installed (thanks)

The documentation is close to PestoLink but with changes

void begin();                       - Placed at setup to begin the WebSockets communication
void loop();                        - Placed in loop to update the following functions:
bool buttonHeld(uint8_t index);     - If the button number is held. Repeats when held
bool buttonPressed(uint8_t index);  - If the button number is pressed. Shoots once when held
float getAxis(uint8_t index);       - Gets the axis of the joystick(s) (the values of 1 and 3 are "inverted" with up being a negative value and vice versa)