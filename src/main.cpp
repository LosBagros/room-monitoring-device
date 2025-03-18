// TODO: Change LED behavior
#include <BH1750.h>            // https://github.com/claws/BH1750
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <Arduino.h>
#include <SensirionI2CScd4x.h> // https://github.com/Sensirion/arduino-i2c-scd4x
#include "Adafruit_SHT4x.h"    // https://github.com/adafruit/Adafruit_SHT4X
#include <WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/PubSubClient
#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <Wire.h>
#include <SPI.h>
#include <WiFiManager.h> //https://github.com/hraph/ESP32WiFiManager/ WiFi Configuration Magic

#include <secret.h>

#define DEV_BOARD // comment if using custom PCB

#ifdef DEV_BOARD
#define LED_PIN 8
#define NUMPIXELS 1
#define SDA 2
#define SCL 1
#else
#define LED_PIN 7
#define NUMPIXELS 2
#define SDA 5
#define SCL 6
#endif

char mqttTopic[50];

WiFiClient espClient;
PubSubClient mqttClient(espClient);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
SensirionI2CScd4x scd4x;
#ifdef DEV_BOARD
BH1750 lightMeter;
#endif

void setColor(uint8_t r, uint8_t g, uint8_t b)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

int mqttTries = 0;
void connectToMQTTBroker()
{
  mqttTries = 0;
  while (!mqttClient.connected())
  {
    Serial.print("Connecting to MQTT...");
    mqttClient.setServer(mqttServer, mqttPort);
    if (mqttClient.connect(sensorId, mqttUser, mqttPassword))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print(" failed with state ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
    mqttTries++;
    if (mqttTries > 30)
    {
      ESP.restart();
    }
  }
}

void setup()
{
  Serial.begin(115200);
  strip.begin();
  setColor(10, 10, 10);

  WiFi.mode(WIFI_STA);

#ifndef DEV_BOARD
  WiFi.setTxPower(WIFI_POWER_5dBm);
#endif

  WiFiManager wm;

  bool res;
  res = wm.autoConnect("RoomMonitorAP", "12345678"); // password protected ap
  if (!res)
  {
    Serial.println("Failed to connect");
  }

  Wire.begin(SDA, SCL);

#ifdef DEV_BOARD
  lightMeter.begin();
#endif

  if (!sht4.begin())
  {
    while (1)
    {
      Serial.println("Couldn't find SHT4x");
      delay(1000);
    }
  }
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  scd4x.begin(Wire);
  uint16_t error;
  char errorMessage[256];

  error = scd4x.stopPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  error = scd4x.startPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  strncpy(mqttTopic, mqttTopicBase, sizeof(mqttTopic) - 1);
  strncat(mqttTopic, sensorId, sizeof(mqttTopic) - 1);
}

sensors_event_t hum, temp;
uint32_t timestamp = 0;
uint16_t co2 = 0;
float co2Temperature = 0.0f;
float co2Humidity = 0.0f;
uint16_t error = 0;
char errorMessage[256];
bool isDataReady = false;
float lux = 0;

StaticJsonDocument<256> jsonDoc;

void loop()
{
  hum.relative_humidity = 0.0f;
  temp.temperature = 0.0f;
  timestamp = 0;
  co2 = 0;
  co2Temperature = 0.0f;
  co2Humidity = 0.0f;
  error = 0;
  isDataReady = false;

  error = scd4x.getDataReadyFlag(isDataReady);
  if (error)
  {
    Serial.print("Error trying to execute getDataReadyFlag(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    Serial.println();
    delay(1000);
    return;
  }
  if (!isDataReady)
  {
    Serial.println("Data not ready yet");
    setColor(0, 0, 100);
    delay(1000);
    return;
  }

  error = scd4x.readMeasurement(co2, co2Temperature, co2Humidity);
  if (error)
  {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  else
  {
    Serial.print("Co2: ");
    Serial.print(co2);
    Serial.print("\t");
    Serial.print("Temperature: ");
    Serial.print(co2Temperature);
    Serial.print("C ");
    Serial.print("Humidity: ");
    Serial.print(co2Humidity);
    Serial.print("%");
  }
  Serial.println();

  timestamp = millis();
  sht4.getEvent(&hum, &temp);
  timestamp = millis() - timestamp;

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.print("C ");
  Serial.print("Humidity: ");
  Serial.print(hum.relative_humidity);
  Serial.print("% ");
  Serial.print(" Read duration (ms): ");
  Serial.print(timestamp);
  Serial.println();
#ifdef DEV_BOARD
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.println();

#endif

  if (co2 > 2000)
  {
    setColor(100, 0, 0);
  }
  else if (co2 > 1000)
  {
    setColor(100, 50, 0);
  }
  else if (co2 < 1000 && co2 > 1)
  {
    setColor(0, 100, 0);
  }
  if (co2 <= 0)
  {
    Serial.println("CO2 Reading failed");
    return;
  }

  if (!mqttClient.connected())
  {
    connectToMQTTBroker();
  }

#ifdef DEV_BOARD
  jsonDoc["light"] = round(lux * 100) / 100.0;
#endif
  jsonDoc["temperature"] = round(temp.temperature * 100) / 100.0;
  jsonDoc["humidity"] = round(hum.relative_humidity * 100) / 100.0;
  jsonDoc["co2"] = co2;
  jsonDoc["co2_temperature"] = round(co2Temperature * 100) / 100.0;
  jsonDoc["co2_humidity"] = round(co2Humidity * 100) / 100.0;

  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  mqttClient.publish(mqttTopic, jsonBuffer);
  mqttClient.loop();
  delay(10000);
}
