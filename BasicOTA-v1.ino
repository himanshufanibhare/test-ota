// OTA_DEBUG must be BEFORE the include

#include <WiFi.h>
#include <ESP32GithubOTA.h>

#define CURRENT_VERSION 3

const char *WIFI_SSID = "myssid";
const char *WIFI_PASSWORD = "password";

const char *VERSION_URL = "https://raw.githubusercontent.com/himanshufanibhare/test-ota/master/var.txt";
const char *FIRMWARE_URL = "https://raw.githubusercontent.com/himanshufanibhare/test-ota/master/build/esp32.esp32.esp32doit-devkit-v1/BasicOTA-v1.ino.bin";

unsigned long lastBlink = 0;
bool ledState = false;

ESP32GithubOTA ota;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected: " + WiFi.localIP().toString());
    ota.setVersion(CURRENT_VERSION);
    ota.setUrls(VERSION_URL, FIRMWARE_URL);
    ota.setAutoUpdate(true);
    ota.onVersionChecked([](int current, int remote)
                         {
    Serial.printf(
        "Version Check -> Current: %d Remote: %d\n",
        current,
        remote
    );

    if (remote > current)
    {
        Serial.println("Update Available");
    }
    else
    {
        Serial.println("Already Up To Date");
    } });

    ota.onProgress([](int percent)
    {
        Serial.printf("OTA Progress: %d%%\n", percent);
    });

    ota.begin();
    // ota.checkVersion();
    ota.updateNow();
    
    
}

void loop()
{
    ota.handle();
    
    if (millis() - lastBlink >= 5000)
    {
        lastBlink = millis();
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
        Serial.println("LED BLINk...");
    }
}