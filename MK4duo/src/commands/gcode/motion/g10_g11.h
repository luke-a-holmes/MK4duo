/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
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

/**
 * gcode.h
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#if ENABLED(FWRETRACT)

#define CODE_G10
#define CODE_G11

/**
 * G10 - Retract filament according to settings of M207
 */
inline void gcode_G10() {
  const bool rs = parser.boolval('S');
  fwretract.retract(true, rs);
}

/**
 * G11 - Recover filament according to settings of M208
 */
inline void gcode_G11() { fwretract.retract(false); }

#endif // FWRETRACT
