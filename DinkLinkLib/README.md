#This is my library which is free to use

##Requires the Arduino library WebSockets by Markus Sattler to be installed (thanks)

##You can zip the DinkLink folder and add it to Arduino IDE using the "add zip library"

##The documentation is close to PestoLink but with changes


void begin();                       - Placed at setup to begin the WebSockets communication

void loop();                        - Placed in loop to update the following functions:

bool buttonHeld(uint8_t index);     - If the button number is held. Repeats when held

bool buttonPressed(uint8_t index);  - If the button number is pressed. Shoots once when held

float getAxis(uint8_t index);       - Gets the axis of the joystick(s) (the values of 1 and 3 are "inverted" with up being a negative value and vice versa)


#To start the server

first install nodejs then run 

node server.js

It should do the rest but you might have to install libraries

The website will be hosted on the ip's port 8080 


#How to find the ip:

The ip then will be 

localhost:8080 - For testing if it launched (doesn't work with robots (I think))

(personal ip):8080 - For local development (findable with ipconfig -> IPv4 Address. e.g. 190:080:3:34:8080)

(Your own setup ip):8080 - For things like ras pi hosting (e.g. hawktimus.local:8080)

When putting the ip into the DinkLink class you will just use the ip without the port (e.g. hawktimus.local, 190:080:3:34)
