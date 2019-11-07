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
 * mcode
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#define CODE_M122

/**
 * M122: Enable, Disable, and/or Report software endstops
 *
 * Usage: M122 S1 to enable, M122 S0 to disable, M122 alone for report
 */
inline void gcode_M122() {

  #if HAS_SOFTWARE_ENDSTOPS
    if (parser.seen('S')) endstops.setSoftEndstop(parser.value_bool());
    SERIAL_STR(ECHO);
    SERIAL_EONOFF(MSG_HOST_SOFT_ENDSTOPS, endstops.isSoftEndstop());
  #else
    SERIAL_MSG(MSG_HOST_SOFT_ENDSTOPS);
    SERIAL_MSG(MSG_LCD_OFF);
  #endif

  #if !(IS_KINEMATIC)
    SERIAL_MSG(MSG_HOST_SOFT_MIN);
    SERIAL_MV(    MSG_HOST_X, LOGICAL_X_POSITION(endstops.soft_endstop.min.x));
    SERIAL_MV(" " MSG_HOST_Y, LOGICAL_Y_POSITION(endstops.soft_endstop.min.y));
    SERIAL_MV(" " MSG_HOST_Z, LOGICAL_Z_POSITION(endstops.soft_endstop.min.z));
    SERIAL_MSG(MSG_HOST_SOFT_MAX);
    SERIAL_MV(    MSG_HOST_X, LOGICAL_X_POSITION(endstops.soft_endstop.max.x));
    SERIAL_MV(" " MSG_HOST_Y, LOGICAL_Y_POSITION(endstops.soft_endstop.max.y));
    SERIAL_MV(" " MSG_HOST_Z, LOGICAL_Z_POSITION(endstops.soft_endstop.max.z));
    SERIAL_EOL();
  #endif
}
