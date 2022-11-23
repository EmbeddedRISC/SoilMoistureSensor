/*************************************************************************
 * Arduino Code for a Local (LCD Based) Soil Moisture Sensor
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

#include <Wire.h>

#include "MoistureSensorCommon.h"

/////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////

// #define SERIAL_DEBUG

#define SLAVE_I2C_ADDR 69

#define POLL_RATE_MS     30000

#define SENSOR_BITS      10
#define MOISTURE_SENSORS 2

/////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////
unsigned short sensor_vals[MOISTURE_SENSORS];

void requestEvent();

void refreshSensors();

/////////////////////////////////////////////////////////////
// Sketch Entry Point
/////////////////////////////////////////////////////////////
void setup() {

  #ifdef SERIAL_DEBUG
    Serial.begin(500000);
  #endif

  // Collect Initial Analog Values
  refreshSensors();

  // Configure as I2C Master
  Wire.begin(SLAVE_I2C_ADDR);
  Wire.onRequest(requestEvent);
}

/////////////////////////////////////////////////////////////
// Sketch Main Loop
/////////////////////////////////////////////////////////////
void loop() {

  // Read analog
  refreshSensors();

  // Wait 1 second
  delay(POLL_RATE_MS);
}

/////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////
void refreshSensors() {
  for (int i = 0; i < MOISTURE_SENSORS; i++) {
    sensor_vals[i] = analogRead(A0 + i);

    #ifdef SERIAL_DEBUG
      Serial.print("Sensor val-");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(sensor_vals[i]);
    #endif
  }
}

void requestEvent() {
  #ifdef SERIAL_DEBUG
    Serial.println("Data Requested!");
  #endif

  byte send_data[MOISTURE_SENSORS * 2];
  for (int i = 0; i < MOISTURE_SENSORS; i++) {
    send_data[i*2] = sensor_vals[i] >> 8;
    send_data[i*2 + 1] = sensor_vals[i] & 0xFF;
  }

  Wire.write(send_data, sizeof(send_data));
}
