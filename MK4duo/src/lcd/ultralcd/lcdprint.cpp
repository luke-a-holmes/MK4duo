/**
 * lcdprint.cpp
 */

#include "../../../MK4duo.h"

#if HAS_SPI_LCD

#include "lcdprint.h"

/**
 * lcd_put_u8str_ind_P
 * Print a string with an index substituted within it
 */
uint8_t lcd_put_u8str_ind_P(PGM_P const pstr, const uint8_t idx, const uint8_t maxlen/*=LCD_WIDTH*/) {
  uint8_t n = maxlen;
  n -= lcd_put_u8str_max_P(pstr, n);
  if (idx != NO_INDEX) {
    n -= lcd_put_wchar(' ');
    n -= lcd_put_wchar(DIGIT(idx));
  }
  return n;
}

#endif // HAS_SPI_LCD
