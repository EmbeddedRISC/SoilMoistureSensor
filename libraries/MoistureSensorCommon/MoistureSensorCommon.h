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

#ifndef __Moisture_Sensor_Common_H__
#define __Moisture_Sensor_Common_H__

#define WATER_VAL 330.0
#define AIR_VAL   630.0

float val2percent(unsigned short val);

#endif