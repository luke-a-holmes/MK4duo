/**
 * MK4duo 3D Printer Firmware
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2013 - 2016 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

// User-defined table 2
// Dummy Thermistor table.. It will ALWAYS read a fixed value.
#ifndef DUMMY_THERMISTOR_999_VALUE
  #define DUMMY_THERMISTOR_999_VALUE 25
#endif

const short temptable_999[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, DUMMY_THERMISTOR_999_VALUE },
  { 1023 * OVERSAMPLENR, DUMMY_THERMISTOR_999_VALUE }
};
