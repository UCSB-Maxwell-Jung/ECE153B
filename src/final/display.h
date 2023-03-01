/*!
 * @file Adafruit_ILI9341.h
 *
 * This is the documentation for Adafruit's ILI9341 driver for the
 * Arduino platform.
 *
 * This library works with the Adafruit 2.8" Touch Shield V2 (SPI)
 *    http://www.adafruit.com/products/1651
 * Adafruit 2.4" TFT LCD with Touchscreen Breakout w/MicroSD Socket - ILI9341
 *    https://www.adafruit.com/product/2478
 * 2.8" TFT LCD with Touchscreen Breakout Board w/MicroSD Socket - ILI9341
 *    https://www.adafruit.com/product/1770
 * 2.2" 18-bit color TFT LCD display with microSD card breakout - ILI9340
 *    https://www.adafruit.com/product/1770
 * TFT FeatherWing - 2.4" 320x240 Touchscreen For All Feathers
 *    https://www.adafruit.com/product/3315
 *
 * These displays use SPI to communicate, 4 or 5 pins are required
 * to interface (RST is optional).
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 *
 * This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
 * Adafruit_GFX</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef _ADAFRUIT_ILI9341H_
#define _ADAFRUIT_ILI9341H_

#include "Adafruit_GFX.h"
// #include "Arduino.h"
// #include "Print.h"
#include "Adafruit_SPITFT.h"
#include <SPI.h>

#define ILI9341_TFTWIDTH 240  ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT 320 ///< ILI9341 max TFT height

#define ILI9341_NOP 0x00     ///< No-op register
#define ILI9341_SWRESET 0x01 ///< Software reset register
#define ILI9341_RDDID 0x04   ///< Read display identification information
#define ILI9341_RDDST 0x09   ///< Read Display Status

#define ILI9341_SLPIN 0x10  ///< Enter Sleep Mode
#define ILI9341_SLPOUT 0x11 ///< Sleep Out
#define ILI9341_PTLON 0x12  ///< Partial Mode ON
#define ILI9341_NORON 0x13  ///< Normal Display Mode ON

#define ILI9341_RDMODE 0x0A     ///< Read Display Power Mode
#define ILI9341_RDMADCTL 0x0B   ///< Read Display MADCTL
#define ILI9341_RDPIXFMT 0x0C   ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT 0x0D   ///< Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF 0x20   ///< Display Inversion OFF
#define ILI9341_INVON 0x21    ///< Display Inversion ON
#define ILI9341_GAMMASET 0x26 ///< Gamma Set
#define ILI9341_DISPOFF 0x28  ///< Display OFF
#define ILI9341_DISPON 0x29   ///< Display ON

#define ILI9341_CASET 0x2A ///< Column Address Set
#define ILI9341_PASET 0x2B ///< Page Address Set
#define ILI9341_RAMWR 0x2C ///< Memory Write
#define ILI9341_RAMRD 0x2E ///< Memory Read

#define ILI9341_PTLAR 0x30    ///< Partial Area
#define ILI9341_VSCRDEF 0x33  ///< Vertical Scrolling Definition
#define ILI9341_MADCTL 0x36   ///< Memory Access Control
#define ILI9341_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1                                                        \
  0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2 0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3                                                        \
  0xB3 ///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR 0xB4  ///< Display Inversion Control
#define ILI9341_DFUNCTR 0xB6 ///< Display Function Control

#define ILI9341_PWCTR1 0xC0 ///< Power Control 1
#define ILI9341_PWCTR2 0xC1 ///< Power Control 2
#define ILI9341_PWCTR3 0xC2 ///< Power Control 3
#define ILI9341_PWCTR4 0xC3 ///< Power Control 4
#define ILI9341_PWCTR5 0xC4 ///< Power Control 5
#define ILI9341_VMCTR1 0xC5 ///< VCOM Control 1
#define ILI9341_VMCTR2 0xC7 ///< VCOM Control 2

#define ILI9341_RDID1 0xDA ///< Read ID 1
#define ILI9341_RDID2 0xDB ///< Read ID 2
#define ILI9341_RDID3 0xDC ///< Read ID 3
#define ILI9341_RDID4 0xDD ///< Read ID 4

#define ILI9341_GMCTRP1 0xE0 ///< Positive Gamma Correction
#define ILI9341_GMCTRN1 0xE1 ///< Negative Gamma Correction
//#define ILI9341_PWCTR6     0xFC

// Color definitions
#define ILI9341_BLACK 0x0000       ///<   0,   0,   0
#define ILI9341_NAVY 0x000F        ///<   0,   0, 123
#define ILI9341_DARKGREEN 0x03E0   ///<   0, 125,   0
#define ILI9341_DARKCYAN 0x03EF    ///<   0, 125, 123
#define ILI9341_MAROON 0x7800      ///< 123,   0,   0
#define ILI9341_PURPLE 0x780F      ///< 123,   0, 123
#define ILI9341_OLIVE 0x7BE0       ///< 123, 125,   0
#define ILI9341_LIGHTGREY 0xC618   ///< 198, 195, 198
#define ILI9341_DARKGREY 0x7BEF    ///< 123, 125, 123
#define ILI9341_BLUE 0x001F        ///<   0,   0, 255
#define ILI9341_GREEN 0x07E0       ///<   0, 255,   0
#define ILI9341_CYAN 0x07FF        ///<   0, 255, 255
#define ILI9341_RED 0xF800         ///< 255,   0,   0
#define ILI9341_MAGENTA 0xF81F     ///< 255,   0, 255
#define ILI9341_YELLOW 0xFFE0      ///< 255, 255,   0
#define ILI9341_WHITE 0xFFFF       ///< 255, 255, 255
#define ILI9341_ORANGE 0xFD20      ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define ILI9341_PINK 0xFC18        ///< 255, 130, 198

/**************************************************************************/
/*!
@brief Class to manage hardware interface with ILI9341 chipset (also seems to
work with ILI9340)
*/
/**************************************************************************/

// class Adafruit_ILI9341 : public Adafruit_SPITFT {
// public:
//   Adafruit_ILI9341(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK,
//                    int8_t _RST = -1, int8_t _MISO = -1);
//   Adafruit_ILI9341(int8_t _CS, int8_t _DC, int8_t _RST = -1);
// #if !defined(ESP8266)
//   Adafruit_ILI9341(SPIClass *spiClass, int8_t dc, int8_t cs = -1,
//                    int8_t rst = -1);
// #endif // end !ESP8266
//   Adafruit_ILI9341(tftBusWidth busWidth, int8_t d0, int8_t wr, int8_t dc,
//                    int8_t cs = -1, int8_t rst = -1, int8_t rd = -1);
// };

void tft_init(void);
void tft_begin(uint32_t freq = 0);
void tft_setRotation(uint8_t r);
void tft_invertDisplay(bool i);
void tft_scrollTo(uint16_t y);
void tft_setScrollMargins(uint16_t top, uint16_t bottom);

// Transaction API not used by GFX
void tft_setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

uint8_t tft_readcommand8(uint8_t reg, uint8_t index = 0);

// Remaining functions do not need to be declared in subclasses
// unless they wish to provide hardware-specific optimizations.
// Brief comments here...documented more thoroughly in .cpp file.

// Subclass' begin() function invokes this to initialize hardware.
// freq=0 to use default SPI speed. spiMode must be one of the SPI_MODEn
// values defined in SPI.h, which are NOT the same as 0 for SPI_MODE0,
// 1 for SPI_MODE1, etc...use ONLY the SPI_MODEn defines! Only!
// Name is outdated (interface may be parallel) but for compatibility:
void initSPI(uint32_t freq = 0, uint8_t spiMode = SPI_MODE0);
void setSPISpeed(uint32_t freq);
// Chip select and/or hardware SPI transaction start as needed:
void startWrite(void);
// Chip deselect and/or hardware SPI transaction end as needed:
void endWrite(void);
void sendCommand(uint8_t commandByte, uint8_t *dataBytes,
                  uint8_t numDataBytes);
void sendCommand(uint8_t commandByte, const uint8_t *dataBytes = NULL,
                  uint8_t numDataBytes = 0);
void sendCommand16(uint16_t commandWord, const uint8_t *dataBytes = NULL,
                    uint8_t numDataBytes = 0);
uint8_t readcommand8(uint8_t commandByte, uint8_t index = 0);
uint16_t readcommand16(uint16_t addr);

// These functions require a chip-select and/or SPI transaction
// around them. Higher-level graphics primitives might start a
// single transaction and then make multiple calls to these functions
// (e.g. circle or text rendering might make repeated lines or rects)
// before ending the transaction. It's more efficient than starting a
// transaction every time.
void writePixel(int16_t x, int16_t y, uint16_t color);
void writePixels(uint16_t *colors, uint32_t len, bool block = true,
                  bool bigEndian = false);
void writeColor(uint16_t color, uint32_t len);
void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                    uint16_t color);
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
// This is a new function, similar to writeFillRect() except that
// all arguments MUST be onscreen, sorted and clipped. If higher-level
// primitives can handle their own sorting/clipping, it avoids repeating
// such operations in the low-level code, making it potentially faster.
// CALLING THIS WITH UNCLIPPED OR NEGATIVE VALUES COULD BE DISASTROUS.
inline void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w,
                                    int16_t h, uint16_t color);
// Another new function, companion to the new non-blocking
// writePixels() variant.
void dmaWait(void);
// Used by writePixels() in some situations, but might have rare need in
// user code, so it's public...
bool dmaBusy(void) const; // true if DMA is used and busy, false otherwise
void swapBytes(uint16_t *src, uint32_t len, uint16_t *dest = NULL);

// These functions are similar to the 'write' functions above, but with
// a chip-select and/or SPI transaction built-in. They're typically used
// solo -- that is, as graphics primitives in themselves, not invoked by
// higher-level primitives (which should use the functions above).
void drawPixel(int16_t x, int16_t y, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
// A single-pixel push encapsulated in a transaction. I don't think
// this is used anymore (BMP demos might've used it?) but is provided
// for backward compatibility, consider it deprecated:
void pushColor(uint16_t color);

using Adafruit_GFX::drawRGBBitmap; // Check base class first
void drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w,
                    int16_t h);

void tft_invertDisplay(bool i);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

// Despite parallel additions, function names kept for compatibility:
void spiWrite(uint8_t b);          // Write single byte as DATA
void writeCommand(uint8_t cmd);    // Write single byte as COMMAND
uint8_t spiRead(void);             // Read single byte of data
void write16(uint16_t w);          // Write 16-bit value as DATA
void writeCommand16(uint16_t cmd); // Write 16-bit value as COMMAND
uint16_t read16(void);             // Read single 16-bit value

// Most of these low-level functions were formerly macros in
// Adafruit_SPITFT_Macros.h. Some have been made into inline functions
// to avoid macro mishaps. Despite the addition of code for a parallel
// display interface, the names have been kept for backward
// compatibility (some subclasses may be invoking these):
void SPI_WRITE16(uint16_t w); // Not inline
void SPI_WRITE32(uint32_t l); // Not inline
// Old code had both a spiWrite16() function and SPI_WRITE16 macro
// in addition to the SPI_WRITE32 macro. The latter two have been
// made into functions here, and spiWrite16() removed (use SPI_WRITE16()
// instead). It looks like most subclasses had gotten comfortable with
// SPI_WRITE16 and SPI_WRITE32 anyway so those names were kept rather
// than the less-obnoxious camelcase variants, oh well.

// Placing these functions entirely in the class definition inlines
// them implicitly them while allowing their use in other code:

/*!
    @brief  Set the chip-select line HIGH. Does NOT check whether CS pin
            is set (>=0), that should be handled in calling function.
            Despite function name, this is used even if the display
            connection is parallel.
*/
void SPI_CS_HIGH(void) {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *csPortSet = 1;
#else  // !KINETISK
  *csPortSet = csPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
  *csPort |= csPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_cs, HIGH);
#endif // end !USE_FAST_PINIO
}

/*!
    @brief  Set the chip-select line LOW. Does NOT check whether CS pin
            is set (>=0), that should be handled in calling function.
            Despite function name, this is used even if the display
            connection is parallel.
*/
void SPI_CS_LOW(void) {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *csPortClr = 1;
#else  // !KINETISK
  *csPortClr = csPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
  *csPort &= csPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_cs, LOW);
#endif // end !USE_FAST_PINIO
}

/*!
    @brief  Set the data/command line HIGH (data mode).
*/
void SPI_DC_HIGH(void) {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *dcPortSet = 1;
#else  // !KINETISK
  *dcPortSet = dcPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
  *dcPort |= dcPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_dc, HIGH);
#endif // end !USE_FAST_PINIO
}

/*!
    @brief  Set the data/command line LOW (command mode).
*/
void SPI_DC_LOW(void) {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *dcPortClr = 1;
#else  // !KINETISK
  *dcPortClr = dcPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
  *dcPort &= dcPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_dc, LOW);
#endif // end !USE_FAST_PINIO
}

protected:
// A few more low-level member functions -- some may have previously
// been macros. Shouldn't have a need to access these externally, so
// they've been moved to the protected section. Additionally, they're
// declared inline here and the code is in the .cpp file, since outside
// code doesn't need to see these.
inline void SPI_MOSI_HIGH(void);
inline void SPI_MOSI_LOW(void);
inline void SPI_SCK_HIGH(void);
inline void SPI_SCK_LOW(void);
inline bool SPI_MISO_READ(void);
inline void SPI_BEGIN_TRANSACTION(void);
inline void SPI_END_TRANSACTION(void);
inline void TFT_WR_STROBE(void); // Parallel interface write strobe
inline void TFT_RD_HIGH(void);   // Parallel interface read high
inline void TFT_RD_LOW(void);    // Parallel interface read low

// CLASS INSTANCE VARIABLES --------------------------------------------

// Here be dragons! There's a big union of three structures here --
// one each for hardware SPI, software (bitbang) SPI, and parallel
// interfaces. This is to save some memory, since a display's connection
// will be only one of these. The order of some things is a little weird
// in an attempt to get values to align and pack better in RAM.

#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
PORTreg_t csPortSet; ///< PORT register for chip select SET
PORTreg_t csPortClr; ///< PORT register for chip select CLEAR
PORTreg_t dcPortSet; ///< PORT register for data/command SET
PORTreg_t dcPortClr; ///< PORT register for data/command CLEAR
#else                  // !HAS_PORT_SET_CLR
PORTreg_t csPort;                 ///< PORT register for chip select
PORTreg_t dcPort;                 ///< PORT register for data/command
#endif                 // end HAS_PORT_SET_CLR
#endif                 // end USE_FAST_PINIO
#if defined(__cplusplus) && (__cplusplus >= 201100)
union {
#endif
  struct {          //   Values specific to HARDWARE SPI:
    SPIClass *_spi; ///< SPI class pointer
#if defined(SPI_HAS_TRANSACTION)
    SPISettings settings; ///< SPI transaction settings
#else
  uint32_t _freq; ///< SPI bitrate (if no SPI transactions)
#endif
    uint32_t _mode; ///< SPI data mode (transactions or no)
  } hwspi;          ///< Hardware SPI values
  struct {          //   Values specific to SOFTWARE SPI:
#if defined(USE_FAST_PINIO)
    PORTreg_t misoPort; ///< PORT (PIN) register for MISO
#if defined(HAS_PORT_SET_CLR)
    PORTreg_t mosiPortSet; ///< PORT register for MOSI SET
    PORTreg_t mosiPortClr; ///< PORT register for MOSI CLEAR
    PORTreg_t sckPortSet;  ///< PORT register for SCK SET
    PORTreg_t sckPortClr;  ///< PORT register for SCK CLEAR
#if !defined(KINETISK)
    ADAGFX_PORT_t mosiPinMask; ///< Bitmask for MOSI
    ADAGFX_PORT_t sckPinMask;  ///< Bitmask for SCK
#endif                           // end !KINETISK
#else                            // !HAS_PORT_SET_CLR
    PORTreg_t mosiPort;           ///< PORT register for MOSI
    PORTreg_t sckPort;            ///< PORT register for SCK
    ADAGFX_PORT_t mosiPinMaskSet; ///< Bitmask for MOSI SET (OR)
    ADAGFX_PORT_t mosiPinMaskClr; ///< Bitmask for MOSI CLEAR (AND)
    ADAGFX_PORT_t sckPinMaskSet;  ///< Bitmask for SCK SET (OR bitmask)
    ADAGFX_PORT_t sckPinMaskClr;  ///< Bitmask for SCK CLEAR (AND)
#endif                           // end HAS_PORT_SET_CLR
#if !defined(KINETISK)
    ADAGFX_PORT_t misoPinMask; ///< Bitmask for MISO
#endif                           // end !KINETISK
#endif                           // end USE_FAST_PINIO
    int8_t _mosi;              ///< MOSI pin #
    int8_t _miso;              ///< MISO pin #
    int8_t _sck;               ///< SCK pin #
  } swspi;                     ///< Software SPI values
  struct {                     //   Values specific to 8-bit parallel:
#if defined(USE_FAST_PINIO)

#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
    volatile uint32_t *writePort; ///< PORT register for DATA WRITE
    volatile uint32_t *readPort;  ///< PORT (PIN) register for DATA READ
#else
    volatile uint8_t *writePort;  ///< PORT register for DATA WRITE
    volatile uint8_t *readPort;   ///< PORT (PIN) register for DATA READ
#endif
#if defined(HAS_PORT_SET_CLR)
    // Port direction register pointers are always 8-bit regardless of
    // PORTreg_t -- even if 32-bit port, we modify a byte-aligned 8 bits.
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
    volatile uint32_t *dirSet; ///< PORT byte data direction SET
    volatile uint32_t *dirClr; ///< PORT byte data direction CLEAR
#else
    volatile uint8_t *dirSet; ///< PORT byte data direction SET
    volatile uint8_t *dirClr; ///< PORT byte data direction CLEAR
#endif
    PORTreg_t wrPortSet; ///< PORT register for write strobe SET
    PORTreg_t wrPortClr; ///< PORT register for write strobe CLEAR
    PORTreg_t rdPortSet; ///< PORT register for read strobe SET
    PORTreg_t rdPortClr; ///< PORT register for read strobe CLEAR
#if !defined(KINETISK)
    ADAGFX_PORT_t wrPinMask; ///< Bitmask for write strobe
#endif                         // end !KINETISK
    ADAGFX_PORT_t rdPinMask; ///< Bitmask for read strobe
#else                          // !HAS_PORT_SET_CLR
    // Port direction register pointer is always 8-bit regardless of
    // PORTreg_t -- even if 32-bit port, we modify a byte-aligned 8 bits.
    volatile uint8_t *portDir;  ///< PORT direction register
    PORTreg_t wrPort;           ///< PORT register for write strobe
    PORTreg_t rdPort;           ///< PORT register for read strobe
    ADAGFX_PORT_t wrPinMaskSet; ///< Bitmask for write strobe SET (OR)
    ADAGFX_PORT_t wrPinMaskClr; ///< Bitmask for write strobe CLEAR (AND)
    ADAGFX_PORT_t rdPinMaskSet; ///< Bitmask for read strobe SET (OR)
    ADAGFX_PORT_t rdPinMaskClr; ///< Bitmask for read strobe CLEAR (AND)
#endif                         // end HAS_PORT_SET_CLR
#endif                         // end USE_FAST_PINIO
    int8_t _d0;              ///< Data pin 0 #
    int8_t _wr;              ///< Write strobe pin #
    int8_t _rd;              ///< Read strobe pin # (or -1)
    bool wide = 0;           ///< If true, is 16-bit interface
  } tft8;                    ///< Parallel interface settings
#if defined(__cplusplus) && (__cplusplus >= 201100)
}; ///< Only one interface is active
#endif
#if defined(USE_SPI_DMA) &&                                                    \
  (defined(__SAMD51__) ||                                                    \
    defined(ARDUINO_SAMD_ZERO))     // Used by hardware SPI and tft8
Adafruit_ZeroDMA dma;              ///< DMA instance
DmacDescriptor *dptr = NULL;       ///< 1st descriptor
DmacDescriptor *descriptor = NULL; ///< Allocated descriptor list
uint16_t *pixelBuf[2];             ///< Working buffers
uint16_t maxFillLen;               ///< Max pixels per DMA xfer
uint16_t lastFillColor = 0;        ///< Last color used w/fill
uint32_t lastFillLen = 0;          ///< # of pixels w/last fill
uint8_t onePixelBuf;               ///< For hi==lo fill
#endif
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if !defined(KINETISK)
ADAGFX_PORT_t csPinMask; ///< Bitmask for chip select
ADAGFX_PORT_t dcPinMask; ///< Bitmask for data/command
#endif                     // end !KINETISK
#else                      // !HAS_PORT_SET_CLR
ADAGFX_PORT_t csPinMaskSet;     ///< Bitmask for chip select SET (OR)
ADAGFX_PORT_t csPinMaskClr;     ///< Bitmask for chip select CLEAR (AND)
ADAGFX_PORT_t dcPinMaskSet;     ///< Bitmask for data/command SET (OR)
ADAGFX_PORT_t dcPinMaskClr;     ///< Bitmask for data/command CLEAR (AND)
#endif                     // end HAS_PORT_SET_CLR
#endif                     // end USE_FAST_PINIO
uint8_t connection;      ///< TFT_HARD_SPI, TFT_SOFT_SPI, etc.
int8_t _rst;             ///< Reset pin # (or -1)
int8_t _cs;              ///< Chip select pin # (or -1)
int8_t _dc;              ///< Data/command pin #

int16_t _xstart = 0;          ///< Internal framebuffer X offset
int16_t _ystart = 0;          ///< Internal framebuffer Y offset
uint8_t invertOnCommand = 0;  ///< Command to enable invert mode
uint8_t invertOffCommand = 0; ///< Command to disable invert mode

uint32_t _freq = 0; ///< Dummy var to keep subclasses happy

#endif // _ADAFRUIT_ILI9341H_
