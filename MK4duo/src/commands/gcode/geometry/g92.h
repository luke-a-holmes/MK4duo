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

#define CODE_G92

/**
 * G92: Set current position to given X Y Z E
 */
inline void gcode_G92(void) {

  bool didE = false;
  #if IS_SCARA || DISABLED(WORKSPACE_OFFSETS)
    bool didXYZ = false;
  #else
    constexpr bool didXYZ = false;
  #endif

  #if USE_GCODE_SUBCODES
    const uint8_t subcode_G92 = parser.subcode;
  #else
    constexpr uint8_t subcode_G92 = 0;
  #endif

  switch (subcode_G92) {
    #if HAS_SD_RESTART
      case 9: {
        LOOP_XYZE(i) {
          if (parser.seenval(axis_codes[i])) {
            mechanics.current_position[i] = parser.value_axis_units((AxisEnum)i);
            #if IS_SCARA || DISABLED(WORKSPACE_OFFSETS)
              if (i == E_AXIS) didE = true; else didXYZ = true;
            #endif
          }
        }
      } break;
    #endif
    default:
    case 0: {
      LOOP_XYZE(i) {
        if (parser.seenval(axis_codes[i])) {
          const float l = parser.value_axis_units((AxisEnum)i),
                      v = (i == E_AXIS) ? l : mechanics.logical_to_native(l, (AxisEnum)i),
                      d = v - mechanics.current_position[i];

          if (!NEAR_ZERO(d)) {
            #if IS_SCARA || DISABLED(WORKSPACE_OFFSETS)
              if (i == E_AXIS) didE = true;
              else didXYZ = true;
              mechanics.current_position[i] = v;        // For SCARA just set the position directly
            #elif ENABLED(WORKSPACE_OFFSETS)
              if (i == E_AXIS)
                mechanics.current_position[E_AXIS] = v; // When using coordinate spaces, only E is set directly
              else {
                mechanics.position_shift[i] += d;       // Other axes simply offset the coordinate space
                endstops.update_software_endstops((AxisEnum)i);
              }
            #else
              mechanics.current_position[i] = v;
            #endif
          }
        }
      }
    } break;
  }

  if    (didXYZ)  mechanics.sync_plan_position();
  else if (didE)  mechanics.sync_plan_position_e();

  mechanics.report_current_position();
}
