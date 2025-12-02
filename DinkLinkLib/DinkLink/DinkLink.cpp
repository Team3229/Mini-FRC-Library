#include "DinkLink.h"

DinkLink::DinkLink(const char *ssid, const char *password, const char *serverIP, const char *deviceID, const bool lowPower, uint16_t port)
	: ssid(ssid), password(password), serverIP(serverIP), path("/"), deviceID(deviceID), lowPower(lowPower), port(port) {}

void DinkLink::begin()
{
	Serial.println("Starting DinkLink...");
	Serial.print("Connecting to WiFi: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);
	if (lowPower)
	{
		WiFi.setTxPower(WIFI_POWER_8_5dBm);
	}
	int attempts = 0;
	while (WiFi.status() != WL_CONNECTED && attempts < 20)
	{
		delay(500);
		Serial.print(".");
		attempts++;
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.println("\nWiFi connected!");
		Serial.print("Local IP: ");
		Serial.println(WiFi.localIP());
	}
	else
	{
		Serial.println("\nWiFi failed to connect.");
		return;
	}

	Serial.print("Connecting to WebSocket at ");
	Serial.print(serverIP);
	Serial.print(":");
	Serial.println(port);

	webSocket.begin(serverIP, port, path);
	webSocket.onEvent([this](WStype_t type, uint8_t *payload, size_t length)
					  { this->handleEvent(type, payload, length); });
	webSocket.setReconnectInterval(5000);
}

void DinkLink::loop()
{
	webSocket.loop();
}

void DinkLink::handleEvent(WStype_t type, uint8_t *payload, size_t length)
{
	if (type == WStype_CONNECTED)
	{
		String msg = "{\"type\":\"register\",\"id\":\"" + String(deviceID) + "\"}";
		webSocket.sendTXT(msg);
	}
	else if (type == WStype_TEXT)
	{
		String data = String((char *)payload, length);

		if (data.indexOf("\"id\":\"") != -1)
		{
			int idStart = data.indexOf("\"id\":\"") + 6;
			int idEnd = data.indexOf("\"", idStart);
			String incomingID = data.substring(idStart, idEnd);
			if (incomingID != String(deviceID))
				return;
		}

		if (data.indexOf("buttons") != -1)
		{
			int start = data.indexOf("buttons");
			start = data.indexOf("[", start);
			int end = data.indexOf("]", start);
			String raw = (start != -1 && end != -1 && end > start) ? data.substring(start + 1, end) : "";

			for (int i = 0; i < 12; i++)
				buttonStates[i] = false;

			int idx = 0;
			while (raw.length() > 0 && idx < 12)
			{
				int comma = raw.indexOf(',');
				String val = (comma == -1) ? raw : raw.substring(0, comma);
				val.trim();
				buttonStates[idx++] = (val == "1");
				if (comma == -1)
					break;
				raw = raw.substring(comma + 1);
			}
			for (; idx < 12; idx++)
				buttonStates[idx] = false;
		}

		if (data.indexOf("axes") != -1)
		{
			int start = data.indexOf("axes");
			start = data.indexOf("[", start);
			int end = data.indexOf("]", start);
			String raw = (start != -1 && end != -1 && end > start) ? data.substring(start + 1, end) : "";

			int idx = 0;
			while (raw.length() > 0 && idx < 4)
			{
				int comma = raw.indexOf(',');
				String val = (comma == -1) ? raw : raw.substring(0, comma);
				val.trim();
				axes[idx++] = val.toFloat();
				if (comma == -1)
					break;
				raw = raw.substring(comma + 1);
			}
			for (; idx < 4; idx++)
				axes[idx] = 0;
		}
	}
}

bool DinkLink::buttonHeld(uint8_t index)
{
	if (index >= 12)
		return false;
	return buttonStates[index];
}

bool DinkLink::buttonPressed(uint8_t index)
{
	if (index >= 12)
		return false;
	bool pressed = buttonStates[index] && !prevButtonStates[index];
	prevButtonStates[index] = buttonStates[index];
	return pressed;
}

float DinkLink::getAxis(uint8_t index)
{
	if (index >= 4)
		return 0;
	return axes[index];
}
