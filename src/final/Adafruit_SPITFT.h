/*!
 * @file Adafruit_SPITFT.h
 *
 * Part of Adafruit's GFX graphics library. Originally this class was
 * written to handle a range of color TFT displays connected via SPI,
 * but over time this library and some display-specific subclasses have
 * mutated to include some color OLEDs as well as parallel-interfaced
 * displays. The name's been kept for the sake of older code.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries,
 * with contributions from the open source community.
 *
 * BSD license, all text here must be included in any redistribution.
 */

#ifndef _ADAFRUIT_SPITFT_H_
#define _ADAFRUIT_SPITFT_H_

#if !defined(__AVR_ATtiny85__) // Not for ATtiny, at all

#include "Adafruit_GFX.h"
#include <SPI.h>

// HARDWARE CONFIG ---------------------------------------------------------

#if defined(__AVR__)
typedef uint8_t ADAGFX_PORT_t;       ///< PORT values are 8-bit
#define USE_FAST_PINIO               ///< Use direct PORT register access
#elif defined(ARDUINO_STM32_FEATHER) // WICED
typedef class HardwareSPI SPIClass;        ///< SPI is a bit odd on WICED
typedef uint32_t ADAGFX_PORT_t;            ///< PORT values are 32-bit
#elif defined(__arm__)
#if defined(ARDUINO_ARCH_SAMD)
// Adafruit M0, M4
typedef uint32_t ADAGFX_PORT_t; ///< PORT values are 32-bit
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
#elif defined(CORE_TEENSY)
// PJRC Teensy 4.x
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
typedef uint32_t ADAGFX_PORT_t; ///< PORT values are 32-bit
                                // PJRC Teensy 3.x
#else
typedef uint8_t ADAGFX_PORT_t; ///< PORT values are 8-bit
#endif
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
#else
// Arduino Due?
typedef uint32_t ADAGFX_PORT_t; ///< PORT values are 32-bit
// USE_FAST_PINIO not available here (yet)...Due has a totally different
// GPIO register set and will require some changes elsewhere (e.g. in
// constructors especially).
#endif
#else                                      // !ARM
// Probably ESP8266 or ESP32. USE_FAST_PINIO is not available here (yet)
// but don't worry about it too much...the digitalWrite() implementation
// on these platforms is reasonably efficient and already RAM-resident,
// only gotcha then is no parallel connection support for now.
typedef uint32_t ADAGFX_PORT_t; ///< PORT values are 32-bit
#endif                                     // end !ARM
typedef volatile ADAGFX_PORT_t *PORTreg_t; ///< PORT register type

#if defined(__AVR__)
#define DEFAULT_SPI_FREQ 8000000L ///< Hardware SPI default speed
#else
#define DEFAULT_SPI_FREQ 16000000L ///< Hardware SPI default speed
#endif

#if defined(ADAFRUIT_PYPORTAL) || defined(ADAFRUIT_PYPORTAL_M4_TITANO) ||      \
    defined(ADAFRUIT_PYBADGE_M4_EXPRESS) ||                                    \
    defined(ADAFRUIT_PYGAMER_M4_EXPRESS) ||                                    \
    defined(ADAFRUIT_MONSTER_M4SK_EXPRESS) || defined(NRF52_SERIES) ||         \
    defined(ADAFRUIT_CIRCUITPLAYGROUND_M0)
#define USE_SPI_DMA ///< Auto DMA
#else
                                           //#define USE_SPI_DMA ///< If set,
                                           // use DMA if available
#endif
// Another "oops" name -- this now also handles parallel DMA.
// If DMA is enabled, Arduino sketch MUST #include <Adafruit_ZeroDMA.h>
// Estimated RAM usage:
// 4 bytes/pixel on display major axis + 8 bytes/pixel on minor axis,
// e.g. 320x240 pixels = 320 * 4 + 240 * 8 = 3,200 bytes.

#if defined(USE_SPI_DMA) && (defined(__SAMD51__) || defined(ARDUINO_SAMD_ZERO))
#include <Adafruit_ZeroDMA.h>
#endif

// This is kind of a kludge. Needed a way to disambiguate the software SPI
// and parallel constructors via their argument lists. Originally tried a
// bool as the first argument to the parallel constructor (specifying 8-bit
// vs 16-bit interface) but the compiler regards this as equivalent to an
// integer and thus still ambiguous. SO...the parallel constructor requires
// an enumerated type as the first argument: tft8 (for 8-bit parallel) or
// tft16 (for 16-bit)...even though 16-bit isn't fully implemented or tested
// and might never be, still needed that disambiguation from soft SPI.
/*! For first arg to parallel constructor */
enum tftBusWidth { tft8bitbus, tft16bitbus };

// CLASS DEFINITION --------------------------------------------------------

/*!
  @brief  Adafruit_SPITFT is an intermediary class between Adafruit_GFX
          and various hardware-specific subclasses for different displays.
          It handles certain operations that are common to a range of
          displays (address window, area fills, etc.). Originally these were
          all color TFT displays interfaced via SPI, but it's since expanded
          to include color OLEDs and parallel-interfaced TFTs. THE NAME HAS
          BEEN KEPT TO AVOID BREAKING A LOT OF SUBCLASSES AND EXAMPLE CODE.
          Many of the class member functions similarly live on with names
          that don't necessarily accurately describe what they're doing,
          again to avoid breaking a lot of other code. If in doubt, read
          the comments.
*/
class Adafruit_SPITFT : public Adafruit_GFX {

public:
  // CONSTRUCTORS --------------------------------------------------------

  // Software SPI constructor: expects width & height (at default rotation
  // setting 0), 4 signal pins (cs, dc, mosi, sclk), 2 optional pins
  // (reset, miso). cs argument is required but can be -1 if unused --
  // rather than moving it to the optional arguments, it was done this way
  // to avoid breaking existing code (-1 option was a later addition).
  Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t cs, int8_t dc, int8_t mosi,
                  int8_t sck, int8_t rst = -1, int8_t miso = -1);

  // Hardware SPI constructor using the default SPI port: expects width &
  // height (at default rotation setting 0), 2 signal pins (cs, dc),
  // optional reset pin. cs is required but can be -1 if unused -- rather
  // than moving it to the optional arguments, it was done this way to
  // avoid breaking existing code (-1 option was a later addition).
  Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t cs, int8_t dc,
                  int8_t rst = -1);

#if !defined(ESP8266) // See notes in .cpp
  // Hardware SPI constructor using an arbitrary SPI peripheral: expects
  // width & height (rotation 0), SPIClass pointer, 2 signal pins (cs, dc)
  // and optional reset pin. cs is required but can be -1 if unused.
  Adafruit_SPITFT(uint16_t w, uint16_t h, SPIClass *spiClass, int8_t cs,
                  int8_t dc, int8_t rst = -1);
#endif // end !ESP8266

  // Parallel constructor: expects width & height (rotation 0), flag
  // indicating whether 16-bit (true) or 8-bit (false) interface, 3 signal
  // pins (d0, wr, dc), 3 optional pins (cs, rst, rd). 16-bit parallel
  // isn't even fully implemented but the 'wide' flag was added as a
  // required argument to avoid ambiguity with other constructors.
  Adafruit_SPITFT(uint16_t w, uint16_t h, tftBusWidth busWidth, int8_t d0,
                  int8_t wr, int8_t dc, int8_t cs = -1, int8_t rst = -1,
                  int8_t rd = -1);

  // DESTRUCTOR ----------------------------------------------------------

  ~Adafruit_SPITFT(){};

  // CLASS MEMBER FUNCTIONS ----------------------------------------------

  // These first two functions MUST be declared by subclasses:

  /*!
      @brief  Display-specific initialization function.
      @param  freq  SPI frequency, in hz (or 0 for default or unused).
  */
  virtual void tft_begin(uint32_t freq) = 0;

  /*!
      @brief  Set up the specific display hardware's "address window"
              for subsequent pixel-pushing operations.
      @param  x  Leftmost pixel of area to be drawn (MUST be within
                 display bounds at current rotation setting).
      @param  y  Topmost pixel of area to be drawn (MUST be within
                 display bounds at current rotation setting).
      @param  w  Width of area to be drawn, in pixels (MUST be >0 and,
                 added to x, within display bounds at current rotation).
      @param  h  Height of area to be drawn, in pixels (MUST be >0 and,
                 added to x, within display bounds at current rotation).
  */
  virtual void tft_setAddrWindow(uint16_t x, uint16_t y, uint16_t w,
                             uint16_t h) = 0;


};

#endif // end __AVR_ATtiny85__
#endif // end _ADAFRUIT_SPITFT_H_
