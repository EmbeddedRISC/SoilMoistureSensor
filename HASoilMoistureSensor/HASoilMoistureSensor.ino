/*************************************************************************
 * Arduino Code for a Home-Assistant Arduino Based Soil Moisture Sensor
 * Copyright (C) 2022  Benjamin Davis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <ESP8266WiFi.h>
#include <ArduinoHA.h>
#include <Wire.h>

#include "MoistureSensorCommon.h"

#include "secrets.h"

/////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////

#define SERIAL_DEBUG

#define SLAVE_I2C_ADDR   69

#define POLL_RATE_MS     30000
#define MOISTURE_SENSORS 2

/////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////

#define VERSION "1.0.0-Alpha"

// Global Definitions for HAss Connection Stuff
WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);

// Gloabl Definitions for the sensors
HASensorNumber moistureSensor0("Moisture_Sensor_0", HASensorNumber::PrecisionP2);
HASensorNumber moistureSensor1("Moisture_Sensor_1", HASensorNumber::PrecisionP2);

unsigned short sensor0_val;
unsigned short sensor1_val;

// I2C Helpers
unsigned long lastReadTime;
void updateSensors();

/////////////////////////////////////////////////////////////
// Sketch Entry Point
/////////////////////////////////////////////////////////////
void setup() {

  #ifdef SERIAL_DEBUG
    Serial.begin(500000);
  #endif

  // Configure as I2C Slave
  Wire.begin(D2, D1);
  lastReadTime = millis();

  // Setup and Connect to the WiFi
  byte mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);
  device.setUniqueId(mac, sizeof(mac));

  WiFi.begin(MY_SSID, MY_WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    #ifdef SERIAL_DEBUG
      Serial.println("Trying to connect to WiFi");
    #endif
    delay(500);
  }

  #ifdef SERIAL_DEBUG
    Serial.println("WiFi Connected");
  #endif

  // Setup the device information
  device.setName("ERISC_Soil_Moisture_Sensor");
  device.setSoftwareVersion(VERSION);
  device.setManufacturer("EmbeddedRISC");
  device.setModel("HAss_SMSensor");
  device.enableSharedAvailability();
  device.enableLastWill();

  // Setup the Sensor Defaults
  moistureSensor0.setName("Moisture Sensor 0");
  moistureSensor0.setUnitOfMeasurement("%");
  moistureSensor1.setName("Moisture Sensor 1");
  moistureSensor1.setUnitOfMeasurement("%");

  // Connect to the MQTT Server
  mqtt.begin(MQTT_IP_ADDR, MQTT_USER_NAME, MQTT_USER_PWD);
}

/////////////////////////////////////////////////////////////
// Sketch Main Loop
/////////////////////////////////////////////////////////////
void loop() {
  mqtt.loop();

  if (lastReadTime + POLL_RATE_MS < millis()) {
    updateSensors();
    lastReadTime = millis();
  } else {
    delay(5);
  }
}

/////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////
void updateSensors() {
  int i = 0;
  unsigned short sensorVals[MOISTURE_SENSORS];
  byte rawVals[MOISTURE_SENSORS*2];

  // Perform the I2C Read
  Wire.requestFrom(SLAVE_I2C_ADDR, sizeof(rawVals));
  while(Wire.available()) {
    rawVals[i] = Wire.read();
    i++;
  }

  // Convert Bytes into 10-bit sensor values
  for (i = 0; i < MOISTURE_SENSORS; i++) {
    sensorVals[i] = (rawVals[i*2] << 8) | rawVals[i*2+1];
  }

  // Update the HAss Sensor Values
  moistureSensor0.setValue(val2percent(sensorVals[0]));
  moistureSensor1.setValue(val2percent(sensorVals[1]));
  #ifdef SERIAL_DEBUG
    Serial.println(sensorVals[0]);
    Serial.println(val2percent(sensorVals[0]));
    Serial.println(val2percent(sensorVals[1]));
  #endif


}
