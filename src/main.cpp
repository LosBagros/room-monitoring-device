// TODO: Change LED behavior
#include <BH1750.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include "Adafruit_SHT4x.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <time.h>
#include <TaskScheduler.h>

#include <secret.h>

#define PIN 7
#define NUMPIXELS 2
#define SDA 5
#define SCL 6

// Callback methods prototypes
void ledsCallback();

Task t1(1000, TASK_FOREVER, &ledsCallback);

Scheduler runner;

char mqttTopic[50];

WiFiClient espClient;
PubSubClient mqttClient(espClient);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
SensirionI2CScd4x scd4x;
// TODO: Light sensor is not visible in I2C

void setupTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
  setenv("TZ", Timezone, 1);
  tzset();
  delay(100);
}

int getCurrentHour()
{
  time_t now = time(NULL);
  if (now == -1)
    return -1;
  struct tm *currentTime = localtime(&now);
  return currentTime->tm_hour;
}

void setColor(uint8_t r, uint8_t g, uint8_t b)
{
  int hour = getCurrentHour();

  if (!enableLedsInNight && hour != -1 && (hour > sleepHour || hour < wakeupHour))
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      strip.setPixelColor(i, strip.Color(r / 255, g / 255, b / 255));
    }
    strip.show();
    return;
  }

  for (int i = 0; i < NUMPIXELS; i++)
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void ledsCallback()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    if (co2 > 2000)
      fadeSingleColor(255, 0, 0, 255, 1000);
    else if (co2 > 1000)
      fadeSingleColor(255, 100, 0, 255, 1000);
    else if (co2 < 1000 && co2 > 1)
      fadeSingleColor(0, 255, 0, 255, 1000);
  }
  else if (!mqttClient.connected())
  {
    if (co2 > 2000)
      fadeSingleColor(255, 0, 0, 255, 2000);
    else if (co2 > 1000)
      fadeSingleColor(255, 100, 0, 255, 2000);
    else if (co2 < 1000 && co2 > 1)
      fadeSingleColor(0, 255, 0, 255, 2000);
  }
  else
  {
    if (co2 > 2000)
      setColor(255, 0, 0);
    else if (co2 > 1000)
      setColor(255, 100, 0);
    else if (co2 < 1000 && co2 > 1)
      setColor(0, 255, 0);
  }
}

void fadeSingleColor(int r, uint8_t g, int b, int maxBrightness, int lightDuration)
{
  if (maxBrightness < 0)
    return;
  if (maxBrightness > 255)
    maxBrightness = 255;

  for (int brightness = 0; brightness <= maxBrightness; brightness += 10)
  {
    setColor((r * brightness) / 255, (g * brightness) / 255, (b * brightness) / 255);
    delay(100);
  }

  delay(lightDuration);

  for (int brightness = maxBrightness; brightness >= 0; brightness -= 10)
  {
    setColor((r * brightness) / 255, (g * brightness) / 255, (b * brightness) / 255);
    delay(100);
  }
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

int mqttTries = 0;
void connectToMQTTBroker()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi not connected");
    return;
  }
  Serial.print("Connecting to MQTT...");
  mqttClient.setServer(mqttServer, mqttPort);
  if (mqttClient.connect(sensorId, mqttUser, mqttPassword))
  {
    Serial.println("connected");
    mqttTries = 0;
  }
  else
    Serial.println(" failed with state " + mqttClient.state());
}

void setup()
{
  Serial.begin(115200);
  strip.begin();
  setColor(255, 255, 255);
  setupTime();
  runner.init();
  t1.enable();
  Wire.begin(SDA, SCL);

  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_5dBm);
  WiFi.begin(ssid, password);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  if (!sht4.begin())
  {
    Serial.println("Couldn't find SHT4x");
    while (1)
      delay(1);
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
uint16_t co2 = 0;
float co2Temperature = 0.0f;
float co2Humidity = 0.0f;
uint16_t error = 0;
char errorMessage[256];
bool isDataReady = false;
unsigned long lastMillis = 0;

StaticJsonDocument<256> jsonDoc;

void loop()
{
  runner.execute();
  hum.relative_humidity = 0.0f;
  temp.temperature = 0.0f;
  co2 = 0;
  co2Temperature = 0.0f;
  co2Humidity = 0.0f;
  error = 0;
  isDataReady = false;

  if (millis() - lastMillis < 10000)
  {
    return;
  }
  lastMillis = millis();
  error = scd4x.getDataReadyFlag(isDataReady);
  if (error)
  {
    Serial.print("Error trying to execute getDataReadyFlag(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    Serial.println();
    return;
  }
  if (!isDataReady)
  {
    Serial.println("Data not ready yet");
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

  sht4.getEvent(&hum, &temp);

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.print("C ");
  Serial.print("Humidity: ");
  Serial.print(hum.relative_humidity);
  Serial.print("% ");
  Serial.println();

  if (co2 <= 0)
  {
    Serial.println("CO2 Reading failed");
    return;
  }

  if (!mqttClient.connected())
  {
    connectToMQTTBroker();
  }

  // jsonDoc["light"] = round(lux * 100) / 100.0; // NOTE: Light sensor is not working
  jsonDoc["temperature"] = round(temp.temperature * 100) / 100.0;
  jsonDoc["humidity"] = round(hum.relative_humidity * 100) / 100.0;
  jsonDoc["co2"] = co2;
  jsonDoc["co2_temperature"] = round(co2Temperature * 100) / 100.0;
  jsonDoc["co2_humidity"] = round(co2Humidity * 100) / 100.0;

  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  mqttClient.publish(mqttTopic, jsonBuffer);
  mqttClient.loop();
}
