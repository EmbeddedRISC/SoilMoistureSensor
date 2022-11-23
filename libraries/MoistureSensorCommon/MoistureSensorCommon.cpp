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

#include "MoistureSensorCommon.h"

float val2percent(unsigned short val) {
  float fval = (float) 100.0 * (1 - ((val - WATER_VAL) / (AIR_VAL - WATER_VAL)));
  if (fval > 100.0) {
    return 100.0;
  } else if (fval  < 0.0) {
    return 0.0;
  }
  return fval;
}