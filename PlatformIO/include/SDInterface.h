#pragma once
#ifndef SDInterface_h
#define SDInterface_h

#include <Arduino.h>

#include "configs.h"
#include "settings.h"

#include <FS.h>
#include <SPI.h>      // SPI before SD
#include <SD.h>
#include <SPIFFS.h>   // used elsewhere (e.g., TFT_eSPI Smooth_font)

// Linked list used for file listings
#include "LinkedListCompat.h"   // or <LinkedList.h> if that's what your project uses

#include "Buffer.h"
#ifdef HAS_SCREEN
  #include "Display.h"
#endif
#include <Update.h>

#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_err.h"

extern Buffer buffer_obj;
extern Settings settings_obj;
#ifdef HAS_SCREEN
  extern Display display_obj;
#endif

#ifdef KIT
  #define SD_DET 4
#endif

class SDInterface {
  private:
  #if defined(MARAUDER_M5STICKC) || defined(HAS_CYD_TOUCH) || defined(MARAUDER_CARDPUTER)
    SPIClass* spiExt = nullptr;
  #elif defined(HAS_C5_SD)
    SPIClass* _spi = nullptr;
    int _cs = -1;
  #endif

    bool checkDetectPin();

  public:
  #ifdef HAS_C5_SD
    SDInterface(SPIClass* spi, int cs) : _spi(spi), _cs(cs) {}
  #else
    SDInterface() = default;
  #endif

    uint8_t  cardType = 0;        // CARD_NONE in SD lib is usually 0; keep safe default
    uint64_t cardSizeMB = 0;
    bool     supported = false;

    String card_sz;

    bool initSD();

    LinkedList<String>* sd_files = nullptr;

    void listDir(String str_dir);
    void listDirToLinkedList(LinkedList<String>* file_names, String str_dir = "/", String ext = "");
    File getFile(String path);
    void runUpdate();
    void performUpdate(Stream& updateSource, size_t updateSize);
    void main();
    bool removeFile(String file_path);
};

#endif // SDInterface_h