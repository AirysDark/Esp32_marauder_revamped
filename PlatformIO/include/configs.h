#pragma once
#ifndef CONFIGS_H
#define CONFIGS_H

// ========================= Common feature toggles =========================
#ifndef WIFI_DUAL_MODE
  #define WIFI_DUAL_MODE 1      // use AP+STA
#endif

#ifndef GPS_ENABLED
  #define GPS_ENABLED    1
#endif

#ifndef NO_NEOPIXEL
  #define NO_NEOPIXEL    1      // explicitly disable NeoPixel/WS2812 usage
#endif

// ========================= Board selection ================================
// Exactly one of these blocks should match. You can define the board macro
// in platformio.ini via build_flags (e.g. -DGENERIC_ESP32 or -DXIAO_ESP32_S3).

#if defined(MARAUDER_M5STICKC)

/* -------------------- M5StickC -------------------------- */
  #define HAS_SCREEN       1
  #define HAS_CYD_TOUCH    0
  #define HAS_C5_SD        0
  #define HAS_LED          (!NO_NEOPIXEL)

  #ifndef GPS_RX_PIN
    #define GPS_RX_PIN    33
  #endif
  #ifndef GPS_TX_PIN
    #define GPS_TX_PIN    32
  #endif
  #ifndef GPS_BAUD
    #define GPS_BAUD      9600
  #endif

#elif defined(GENERIC_ESP32)

/* -------------------- Generic ESP32 (esp32dev) -------------------------- */
  #define HAS_SCREEN       1
  #define HAS_CYD_TOUCH    0
  #define HAS_C5_SD        0
  #define HAS_LED          0
  #define HAS_SD           0

  #ifndef GPS_RX_PIN
    #define GPS_RX_PIN    16
  #endif
  #ifndef GPS_TX_PIN
    #define GPS_TX_PIN    17
  #endif
  #ifndef GPS_BAUD
    #define GPS_BAUD      9600
  #endif

  #ifndef TFT_MOSI
    #define TFT_MOSI      23
  #endif
  #ifndef TFT_SCLK
    #define TFT_SCLK      18
  #endif
  #ifndef TFT_CS
    #define TFT_CS         5
  #endif
  #ifndef TFT_DC
    #define TFT_DC         2
  #endif
  #ifndef TFT_RST
    #define TFT_RST        4
  #endif

#elif defined(XIAO_ESP32_S3)

/* -------------------- Seeed Studio XIAO ESP32-S3 ------------------------- */
  #define HAS_SCREEN       0       // no onboard display
  #define HAS_CYD_TOUCH    0
  #define HAS_C5_SD        0
  #define HAS_LED          1       // onboard RGB LED on GPIO 21
  #define HAS_SD           0

  // GPS on UART (defaults to IO43 = RX, IO44 = TX)
  #ifndef GPS_RX_PIN
    #define GPS_RX_PIN    43
  #endif
  #ifndef GPS_TX_PIN
    #define GPS_TX_PIN    44
  #endif
  #ifndef GPS_BAUD
    #define GPS_BAUD      9600
  #endif

  // I2C (SDA=8, SCL=9)
  #define I2C_SDA         8
  #define I2C_SCL         9

  // SPI (defaults; adjust if using an external TFT/SD breakout)
  #define SPI_MOSI        10
  #define SPI_MISO        11
  #define SPI_SCLK        12
  #define SPI_CS          13

#else

/* -------------------- Fallback (unknown board) --------------------------- */
  #define HAS_SCREEN       0
  #define HAS_CYD_TOUCH    0
  #define HAS_C5_SD        0
  #define HAS_LED          0

  #ifndef GPS_RX_PIN
    #define GPS_RX_PIN    16
  #endif
  #ifndef GPS_TX_PIN
    #define GPS_TX_PIN    17
  #endif
  #ifndef GPS_BAUD
    #define GPS_BAUD      9600
  #endif

#endif // board selection

// ========================= Project-wide helpers ===========================
#ifdef HAS_SCREEN
  #if HAS_SCREEN
    #define SCREEN_BUFFER 1
  #endif
#endif

#define USE_SPIFFS 1

#endif // CONFIGS_H