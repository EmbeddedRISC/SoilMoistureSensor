/*************************************************************************
 * Arduino Code for a Home-Assistant Arduino Based Soil Moisture Sensor
 * Copyright (C) <year>  <name of author>
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

#include "secrets.h"

#define VERSION "1.0.0-Alpha"

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);

HASensorNumber moistureSensor0("Moisture_Sensor_0", HASensorNumber::PrecisionP2);
HASensorNumber moistureSensor1("Moisture_Sensor_1", HASensorNumber::PrecisionP2);

void setup() {

  Serial.begin(500000);

  byte mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);
  device.setUniqueId(mac, sizeof(mac));

  WiFi.begin(MY_SSID, MY_WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Trying to connect to WiFi");
    delay(500);
  }
  Serial.println("WiFi Connected");

  device.setName("ERISC_Soil_Moisture_Sensor");
  device.setSoftwareVersion(VERSION);
  device.setManufacturer("EmbeddedRISC");
  device.setModel("HAss_SMSensor");
  device.enableSharedAvailability();
  device.enableLastWill();

  moistureSensor0.setName("Moisture Sensor 0");
  moistureSensor0.setUnitOfMeasurement("%");
  moistureSensor0.setValue((float)69.69);

  moistureSensor1.setName("Moisture Sensor 1");
  moistureSensor1.setUnitOfMeasurement("%");
  moistureSensor1.setValue((float)42.0);
  
  mqtt.begin(MQTT_IP_ADDR, MQTT_USER_NAME, MQTT_USER_PWD);
}

void loop() {
  mqtt.loop();
}