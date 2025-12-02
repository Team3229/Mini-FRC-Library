#pragma once
#include <WiFi.h>
#include <WebSocketsClient.h>

class DinkLink
{
public:
	DinkLink(const char *ssid, const char *password, const char *serverIP, const char *deviceID, bool lowPower, uint16_t port = 8080);
	void begin();
	void loop();
	bool buttonHeld(uint8_t index);
	bool prevButtonStates[12];
	bool buttonPressed(uint8_t index);
	float getAxis(uint8_t index);

private:
	WebSocketsClient webSocket;
	const char *ssid;
	const char *password;
	const char *serverIP;
	const char* path;
	bool lowPower;
	uint16_t port;
	const char *deviceID;
	bool buttonStates[12];
	float axes[4];
	void handleEvent(WStype_t type, uint8_t *payload, size_t length);
};
