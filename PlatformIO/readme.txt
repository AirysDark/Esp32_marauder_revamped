Build & Upload (PlatformIO)

Build firmware:
pio run

Upload firmware:
pio run -t upload

Build filesystem image (SPIFFS/LittleFS):
pio run -t buildfs

Upload filesystem image:
pio run -t uploadfs