#pragma once
#include <Wire.h>

// minimal SX1509 IO Expander driver
class SX1509 {
  public:
    static constexpr uint8_t kDeviceAddress = 0x3e;
    static constexpr uint16_t kTimeout = 1000;

    static constexpr uint8_t kRegReset = 0x7d;
    static constexpr uint8_t kRegDirectionA = 0xf;
    static constexpr uint8_t kRegDirectionB = 0xe;
    static constexpr uint8_t kRegInterruptMaskA = 0x13;
    static constexpr uint8_t kRegDataA = 0x11;
    static constexpr uint8_t kRegDataB = 0x10;
    static constexpr uint8_t kRegPullUpA = 0x7;
    static constexpr uint8_t kRegPullUpB = 0x6;

    // Initialize I2C and initiate a reset of the expander
    bool init() {
      Wire.begin();
      softwareReset();

      // Test that the device is responding correctly by
      // checking that we read the correct values for
      // a register.
      uint16_t regs ;
      if (!read(kRegInterruptMaskA, regs)) {
        return false;
      }
      return regs == 0xff00;
    }

    // Set pin direction for bank A
    bool directionA(uint8_t mask) {
      return write(kRegDirectionA, mask);
    }

    // Set pin direction for bank B
    bool directionB(uint8_t mask) {
      return write(kRegDirectionB, mask);
    }

    // Set logic level for bank A
    bool dataA(uint8_t mask) {
      return write(kRegDataA, mask);
    }

    // Set logic level for bank B
    bool dataB(uint8_t mask) {
      return write(kRegDataB, mask);
    }

    // Read the logic level for bank B
    bool readDataB(uint8_t &data) {
      return read(kRegDataB, data);
    }

    // Configure pullups for bank A
    bool pullupA(uint8_t mask) {
      return write(kRegPullUpA, mask);
    }

    // Configure pullups for bank B
    bool pullupB(uint8_t mask) {
      return write(kRegPullUpB, mask);
    }

    // Initiate a software reset
    void softwareReset() {
      write(kRegReset, 0x12);
      write(kRegReset, 0x34);
    }

    // Write a value to a register
    bool write(uint8_t addr, uint8_t val) {
      Wire.beginTransmission(kDeviceAddress);
      Wire.write(addr);
      Wire.write(val);
      Wire.endTransmission();
    }

    // Read bytes starting from the specified register address
    bool read(uint8_t addr, uint8_t* bytes, uint8_t bufsize) {
      unsigned int timeout = kTimeout * bufsize;

      Wire.beginTransmission(kDeviceAddress);
      Wire.write(addr);
      Wire.endTransmission();
      Wire.requestFrom(kDeviceAddress, bufsize);

      while ((Wire.available() < bufsize) && (timeout != 0)) {
        timeout--;
      }

      if (timeout == 0) {
        return false;
      }

      while (bufsize-- > 0) {
        *bytes = Wire.read();
      }
      return true;
    }

    // Read a single byte register
    bool read(uint8_t addr, uint8_t &u8) {
      return read(addr, &u8, 1);
    }

    // Read a 16-bit register
    bool read(uint8_t addr, uint16_t &u16) {
      uint8_t buf[2];
      if (read(addr, buf, sizeof(buf))) {
        u16 = (buf[0] << 8) | buf[1];
        return true;
      }
      return false;
    }
};
