#include "GpsInterface.h"

// Helper: convert MicroNMEA's integer lat/lon (1e-7 degrees) already provided by library
GpsInterface::GpsInterface(HardwareSerial& serial)
: m_serial(serial),
  m_nmea(m_nmeaBuffer, sizeof(m_nmeaBuffer))
{
  m_nmea.setAltitudeUnits(MicroNMEA::ALTITUDE_UNITS_METERS);
}

void GpsInterface::begin(uint32_t baud) {
  // Configure the UART with RX/TX pins if supported (ESP32 does)
  #if defined(ARDUINO_ARCH_ESP32)
    m_serial.begin(baud, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  #else
    m_serial.begin(baud);
  #endif
}

void GpsInterface::handleIncoming() {
  while (m_serial.available() > 0) {
    char c = static_cast<char>(m_serial.read());
    if (m_nmea.process(c)) {
      // A full, valid sentence has been processed – update cached values

      // Fix & position
      m_hasFix = m_nmea.isValid() && m_nmea.getFixStatus();

      long lat = 0, lon = 0;
      m_nmea.getLatitude(lat);
      m_nmea.getLongitude(lon);
      // MicroNMEA returns lat/lon in 1e-7 degrees
      m_latE7 = static_cast<int32_t>(lat);
      m_lonE7 = static_cast<int32_t>(lon);

      // Satellites and HDOP
      m_sats = m_nmea.getNumSatellites();
      m_hdop = m_nmea.getHDOP(); // hdop*10 per MicroNMEA docs

      // Altitude (if present) – MicroNMEA exposes getAltitude(long&)
      long altCm = 0;
      if (m_nmea.getAltitude(altCm)) {   // returns true if value present
        m_altCm = static_cast<int16_t>(altCm);
      }
    }
  }
}
