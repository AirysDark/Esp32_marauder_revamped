#pragma once

#include <Arduino.h>
#include <MicroNMEA.h>

// Pins and serial selection come from configs.h, but provide sane defaults if not defined
#ifndef GPS_SERIAL
#  define GPS_SERIAL Serial2
#endif
#ifndef GPS_RX_PIN
#  define GPS_RX_PIN 16
#endif
#ifndef GPS_TX_PIN
#  define GPS_TX_PIN 17
#endif
#ifndef GPS_BAUD
#  define GPS_BAUD 9600
#endif

class GpsInterface {
public:
  explicit GpsInterface(HardwareSerial& serial = GPS_SERIAL);

  // Initialize UART for the GPS
  void begin(uint32_t baud = GPS_BAUD);

  // Pump bytes from UART into MicroNMEA parser. Call this often.
  void handleIncoming();

  // Accessors
  bool hasFix() const { return m_hasFix; }
  int32_t latitudeE7()  const { return m_latE7; }  // degrees * 1e7
  int32_t longitudeE7() const { return m_lonE7; }  // degrees * 1e7
  int16_t altitudeCm()  const { return m_altCm; }  // centimeters (if available, else 0)
  uint8_t sats()        const { return m_sats; }
  uint16_t hdop()       const { return m_hdop; }   // HDOP * 10 (e.g., 9 -> 0.9)

private:
  HardwareSerial& m_serial;

  // MicroNMEA needs an internal sentence buffer
  static const size_t NMEA_BUF_LEN = 120;
  char m_nmeaBuffer[NMEA_BUF_LEN];
  MicroNMEA m_nmea;

  // Cached values
  bool    m_hasFix   = false;
  int32_t m_latE7    = 0;
  int32_t m_lonE7    = 0;
  int16_t m_altCm    = 0;    // altitude in cm
  uint8_t m_sats     = 0;
  uint16_t m_hdop    = 0;    // hdop*10
};
