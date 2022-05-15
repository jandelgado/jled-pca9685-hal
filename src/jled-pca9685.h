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

    PCA9685Hal(PinType pin, Adafruit_PWMServoDriver* pwm)
        : pin_(pin)
        , pwm_(pwm)
    {
    }

    PCA9685Hal& operator=(const PCA9685Hal& rhs)
    {
        pin_ = rhs.pin_;
        pwm_ = rhs.pwm_;
        return *this;
    };

    void analogWrite(uint8_t duty) const
    {
        if (duty == last_) {
            return;
        }
        last_ = duty;

        // scale 8bit JLed duty to [0..4095]
        const uint32_t _duty = duty == 0 ? 0 : (duty << 4) | 15;
        pwm_->setPin(pin_, _duty, false);
    }

    uint32_t millis() const { return ::millis(); };

private:
    PinType pin_;
    mutable uint8_t last_ = 0;
    Adafruit_PWMServoDriver* pwm_;
};

class JLedPCA9685 : public TJLed<PCA9685Hal, JLedPCA9685> {
    using TJLed<PCA9685Hal, JLedPCA9685>::TJLed;
};

};
