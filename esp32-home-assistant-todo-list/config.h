#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> // Required for the String class

// Wi-Fi configuration
extern const char *ssid;       // Wi-Fi SSID (network name)
extern const char *password;   // Wi-Fi password

// Home Assistant API configuration
extern const String baseURL;    // Base URL of the Home Assistant API
extern const String listName;   // Name of the To-Do list in Home Assistant
extern const String accessToken; // Home Assistant API access token

#endif