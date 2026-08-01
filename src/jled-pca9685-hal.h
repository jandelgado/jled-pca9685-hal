// Copyright (c) 2022 Jan Delgado <jdelgado[at]gmx.net>
// https://github.com/jandelgado/jled
//
// a JLed HAL to control LEDs using a PCA9685 with the I2C bus.
// This implementations uses the Adafruit PWM Servo Driver Library to control
// the PCA9685.
//
// See
//   * https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//   * https://github.com/jandelgado/jled
//
#pragma once

#include <Adafruit_PWMServoDriver.h>
#include <jled.h>

namespace jled {

class PCA9685Hal {

public:
  using PinType = uint8_t;

  PCA9685Hal() = delete;

  PCA9685Hal(PinType pin, Adafruit_PWMServoDriver *pwm)
      : pin_(pin), pwm_(pwm) {
      }

  PCA9685Hal &operator=(const PCA9685Hal &rhs) {
    pin_ = rhs.pin_;
    pwm_ = rhs.pwm_;
    // reset the dedup cache: it referred to the pin/driver we just
    // overwrote, so it must not suppress the next analogWrite().
    last_ = 0;
    return *this;
  };

  template <typename Brightness>
  void analogWrite(Brightness val, bool invert) const {
    // scale JLed brightness to the 12 bit [0..4095] duty cycle used by PCA9685
    const uint16_t duty = scaleToNative<12>(val);
    if (duty == last_) {
      return;
    }
    last_ = duty;
    pwm_->setPin(pin_, duty, invert);
  }

private:
  PinType pin_;
  mutable uint16_t last_ = 0;
  Adafruit_PWMServoDriver *pwm_;
};

// PCA9685 has native invert support via Adafruit_PWMServoDriver::setPin(),
// so brightness inversion (LowActive()) is applied in hardware.
class JLedPCA9685
    : public TJLed<PCA9685Hal, JLedClockType, uint8_t, JLedPCA9685> {
  using Base = TJLed<PCA9685Hal, JLedClockType, uint8_t, JLedPCA9685>;

public:
  using Base::Base;

  // PCA9685Hal always needs both the pin and the driver instance, so, unlike
  // TJLed's single-pin constructor, this convenience constructor takes both.
  JLedPCA9685(PCA9685Hal::PinType pin, Adafruit_PWMServoDriver *pwm)
      : Base(PCA9685Hal(pin, pwm)) {}
};

class JLedPCA9685HD
    : public TJLed<PCA9685Hal, JLedClockType, uint16_t, JLedPCA9685HD> {
  using Base = TJLed<PCA9685Hal, JLedClockType, uint16_t, JLedPCA9685HD>;

public:
  using Base::Base;

  JLedPCA9685HD(PCA9685Hal::PinType pin, Adafruit_PWMServoDriver *pwm)
      : Base(PCA9685Hal(pin, pwm)) {}
};
}; // namespace jled
