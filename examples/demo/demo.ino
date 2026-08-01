// Use JLed to control LEDs using a PCA9685 I2C PWM controller.
//
// see
//  * https://github.com/jandelgado/jled-pca9685-hal
//  * https://github.com/jandelgado/jled
//
// Copyright 2022-2026 by Jan Delgado. All rights reserved.
//
#include <jled-pca9685-hal.h>

// initialize the I2C and pwm driver. I2C_ADDRESS may vary among PCA9685 boards.
constexpr auto I2C_ADDRESS = 0x40;
auto i2c = TwoWire();
auto pwm = Adafruit_PWMServoDriver(I2C_ADDRESS, i2c);

// led_builtin is using the platforms HAL and drives the builtin LED
auto led_builtin = JLed(LED_BUILTIN).Blink(500, 500).Forever().LowActive();

// these LEDs use the PCA9685 HAL and drive leds through the I2C bus
// the HD version leverages full 12-bit resolution of the PCA9685 PWM, the others operate
// with 8-bit effect and PWM resolution. Watch carefully led12 and led13 side-by-side,
// the difference is visible.
auto led12 = jled::JLedPCA9685HD(12, &pwm).Breathe(15000).DelayAfter(500).Forever();
auto led13 = jled::JLedPCA9685(13, &pwm).Breathe(15000).DelayAfter(500).Forever();
auto led14 = jled::JLedPCA9685(14, &pwm).FadeOff(1000).Forever();
// equivalent, explicit HAL construction, e.g. for a custom PCA9685Hal
auto led15 = jled::JLedPCA9685(jled::PCA9685Hal(15, &pwm)).Blink(250, 750).Forever();

// construct a JLedRefGroup to control all LEDs in parallel.
JLedRef leds[] = {
    &led_builtin,
    &led12,
    &led13,
    &led14,
    &led15
};

auto group = JLedRefGroup::Parallel(leds);

void setup() {
    pwm.begin();
}

void loop() {
    group.Update();
}
