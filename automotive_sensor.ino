#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <DHT.h>
#include <DHT_U.h>

#define SD_CS_PIN 10
#define DHT_PIN 2
#define DHT_TYPE DHT11

DHT_Unified dht(DHT_PIN, DHT_TYPE);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(9600);
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  dht.begin();
  accel.begin();

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Timestamp, Temperature (C), Acceleration X (m/s^2), Acceleration Y (m/s^2), Acceleration Z (m/s^2)");
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.txt");
  }
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  float temp = dht.readTemperature();

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(millis());
    dataFile.print(", ");
    dataFile.print(temp);
    dataFile.print(", ");
    dataFile.print(event.acceleration.x);
    dataFile.print(", ");
    dataFile.print(event.acceleration.y);
    dataFile.print(", ");
    dataFile.println(event.acceleration.z);
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.txt");
  }

  delay(1000);
}
