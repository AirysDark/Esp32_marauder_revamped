#include "BatteryInterface.h"
#include "lang_var.h"
#include <Wire.h>
#include <math.h>   // for isfinite, lroundf
BatteryInterface::BatteryInterface() {
  
}

void BatteryInterface::main(uint32_t currentTime) {
  if (currentTime != 0) {
    if (currentTime - initTime >= 3000) {
      //Serial.println("Checking Battery Level");
      this->initTime = millis();
      int8_t new_level = this->getBatteryLevel();
      //this->battery_level = this->getBatteryLevel();
      if (this->battery_level != new_level) {
        Serial.println(text00 + (String)new_level);
        this->battery_level = new_level;
        Serial.println("Battery Level: " + (String)this->battery_level);
      }
    }
  }
}

void BatteryInterface::RunSetup() {
  byte error;
  byte addr;

  #ifdef HAS_BATTERY

    Wire.begin(I2C_SDA, I2C_SCL);

    Serial.println("Checking for battery monitors...");

    Wire.beginTransmission(IP5306_ADDR);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.println("Detected IP5306");
      this->has_ip5306 = true;
      this->i2c_supported = true;
    }

    Wire.beginTransmission(MAX17048_ADDR);
    error = Wire.endTransmission();

    if (error == 0) {
      if (maxlipo.begin()) {
        Serial.println("Detected MAX17048");
        this->has_max17048 = true;
        this->i2c_supported = true;
      }
    }

    /*for(addr = 1; addr < 127; addr++ ) {
      Wire.beginTransmission(addr);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at address 0x");
        
        if (addr<16)
          Serial.print("0");

        Serial.println(addr,HEX);
        
        if (addr == IP5306_ADDR) {
          this->has_ip5306 = true;
          this->i2c_supported = true;
        }

        if (addr == MAX17048_ADDR) {
          if (maxlipo.begin()) {
            Serial.println("Detected MAX17048");
            this->has_max17048 = true;
            this->i2c_supported = true;
          }
        }
      }
    }*/

    /*if (this->maxlipo.begin()) {
      Serial.println("Detected MAX17048");
      this->has_max17048 = true;
      this->i2c_supported = true;
    }*/
    
    this->initTime = millis();
  #endif
}


int8_t BatteryInterface::getBatteryLevel() {
  // ---- IP5306 fuel gauge over I2C ----
  if (this->has_ip5306) {
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(0x78);
    if (Wire.endTransmission(false) == 0 && Wire.requestFrom(IP5306_ADDR, 1)) {
      this->i2c_supported = true;
      switch (Wire.read() & 0xF0) {
        case 0xE0: return 25;
        case 0xC0: return 50;
        case 0x80: return 75;
        case 0x00: return 100;
        default:   return 0;    // unknown nibble -> treat as empty
      }
    }
    this->i2c_supported = false;
    return -1;                  // IP5306 present but read failed
  }

  // ---- MAX17048 fuel gauge ----
  if (this->has_max17048) {
    float percent = this->maxlipo.cellPercent();   // 0..100
    if (!isfinite(percent)) return -1;
    if (percent < 0)   percent = 0;
    if (percent > 100) percent = 100;
    return (int8_t) lroundf(percent);
  }

  // ---- Optional ADC fallback (if you have a battery sense pin) ----
  // #ifdef BAT_ADC_PIN
  //   int raw = analogRead(BAT_ADC_PIN);
  //   // map raw to 0..100 here...
  //   return (int8_t)clamped_percent;
  // #endif

  // No gauge available
  return -1;
}