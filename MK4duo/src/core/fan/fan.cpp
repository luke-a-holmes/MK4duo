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
 * fan.cpp
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#include "../../../MK4duo.h"
#include "sanitycheck.h"

#if MAX_FAN > 0
  Fan* fans[MAX_FAN] = { nullptr };
#endif

/** Public Function */
void Fan::init() {

  speed               = 0;
  paused_speed        = 0;
  scaled_speed        = 128;
  kickstart           = 0;
  pwm_soft_pos        = 0;
  pwm_soft_count      = 0xFF;

  setIdle(false);

  #if ENABLED(TACHOMETRIC)
    data.tacho.init(data.ID);
  #endif

  if (printer.isRunning()) return; // All running not reinitialize

  if (data.pin > 0) HAL::pinMode(data.pin, isHWinvert() ? OUTPUT_HIGH : OUTPUT_LOW);

}

void Fan::set_speed(const uint8_t new_speed) {
  #if ENABLED(FAN_KICKSTART_TIME)
    if (kickstart == 0 && new_speed > speed) {
      if (speed)  kickstart = FAN_KICKSTART_TIME / 10;
      else        kickstart = FAN_KICKSTART_TIME;
    }
  #endif

  speed = new_speed;

  LIMIT(speed, data.speed_limit.min, data.speed_limit.max);
}

void Fan::set_auto_monitor(const int8_t h) {
  if (WITHIN(h, 0, HOTENDS - 1) || h == 6 || h == 7)
    SBI(data.auto_monitor, (uint8_t)h);
  else if (h == -1)
    data.auto_monitor = 0;
  else
    SERIAL_EM(MSG_HOST_INVALID_HOTEND);
  spin();
}

void Fan::set_output_pwm() {

  const uint8_t new_Speed = isHWinvert() ? 255 - actual_speed() : actual_speed();

  if (USEABLE_HARDWARE_PWM(data.pin))
    HAL::analogWrite(data.pin, new_Speed, data.freq);
  else {
    // Now set the pin high (if not 0)
    if (pwm_soft_count == 0 && data.pin > NoPin && ((pwm_soft_pos = (new_Speed & SOFT_PWM_MASK)) > 0))
        HAL::digitalWrite(data.pin, HIGH);

    // If it's a valid pin turn off the channel
    if (data.pin > NoPin && pwm_soft_pos == pwm_soft_count && pwm_soft_pos != SOFT_PWM_MASK)
      HAL::digitalWrite(data.pin, LOW);

    pwm_soft_count += SOFT_PWM_STEP;
  }

}

void Fan::spin() {

  static short_timer_t controller_fan_timer;

  if (data.auto_monitor != 0) {

    // Check for Hotend temperature
    LOOP_HOTEND() {
      if (TEST(data.auto_monitor, h)) {
        if (hotends[h]->deg_current() > data.trigger_temperature) {
          speed = data.speed_limit.max;
          break;
        }
        else
          speed = data.speed_limit.min;
      }
    }

    // Check for Controller fan
    if (TEST(data.auto_monitor, 7)) {

      // Check Heaters
      if (thermalManager.heaters_isActive()) controller_fan_timer.start();

      #if HAS_MCU_TEMPERATURE
        // Check MSU
        if (thermalManager.mcu_current_temperature >= 50) controller_fan_timer.start();
      #endif

      // Check Motors
      if (stepper.driver_is_enable()) controller_fan_timer.start();

      // Fan off if no steppers or heaters have been enabled for CONTROLLERFAN_SECS seconds
      if (!controller_fan_timer.isRunning() || controller_fan_timer.expired((CONTROLLERFAN_SECS) * 1000, false)) {
        speed = data.speed_limit.min;
      }
      else
        speed = data.speed_limit.max;
    }

  }

  speed = speed ? constrain(speed, data.speed_limit.min, data.speed_limit.max) : 0;

}

void Fan::print_M106() {
  bool found_auto = false;
  SERIAL_LM(CFG, "Fans: P<Fan> U<Pin> L<Min Speed> X<Max Speed> F<Freq> I<Hardware Inverted 0-1> H<Auto mode> T<Trig Temp>");
  SERIAL_SMV(CFG, "  M106 P", (int)data.ID);
  SERIAL_MV(" U", data.pin);
  SERIAL_MV(" L", data.speed_limit.min);
  SERIAL_MV(" X", data.speed_limit.max);
  SERIAL_MV(" F", data.freq);
  SERIAL_MV(" I", isHWinvert());
  SERIAL_MSG(" H");
  LOOP_HOTEND() {
    if (TEST(data.auto_monitor, h)) {
      SERIAL_VAL((int)h);
      SERIAL_MV(" T", data.trigger_temperature);
      found_auto = true;
      break;
    }
  }
  if (!found_auto) {
    if (TEST(data.auto_monitor, 7))
      SERIAL_CHR('7');
    else
      SERIAL_MSG("-1");
  }
  SERIAL_EOL();
}

#if ENABLED(TACHOMETRIC)
  void tacho_interrupt0() { fans[0]->data.tacho.interrupt(); }
  #if FAN_COUNT > 1
    void tacho_interrupt1() { fans[1]->data.tacho.interrupt(); }
    #if FAN_COUNT > 2
      void tacho_interrupt2() { fans[2]->data.tacho.interrupt(); }
      #if FAN_COUNT > 3
        void tacho_interrupt3() { fans[3]->data.tacho.interrupt(); }
        #if FAN_COUNT > 4
          void tacho_interrupt4() { fans[4]->data.tacho.interrupt(); }
          #if FAN_COUNT > 5
            void tacho_interrupt5() { fans[5]->data.tacho.interrupt(); }
          #endif
        #endif
      #endif
    #endif
  #endif
#endif
