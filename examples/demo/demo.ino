// Use JLed to control LEDs using a PCA9685 I2C PWM controller.
//
// see 
//  * https://github.com/jandelgado/jled-pca9685-hal
//  * https://github.com/jandelgado/jled
// 
// Copyright 2022 by Jan Delgado. All rights reserved.
//
#include <jled-pca9685.h>

constexpr auto I2C_ADDRESS = 0x40;
auto i2c = TwoWire();
auto pwm = Adafruit_PWMServoDriver(I2C_ADDRESS, i2c);

// A wrapper to create a JLed object with a PCA9685 HAL.
// Use 
//   auto led = JLedPCA9685(pin).Blink(1000,2000);  
// instead of 
//   auto led = jled::JLedPCA9685(jled::PCA9985Hal(pin, pwm)).Blink(1000,2000);  
jled::JLedPCA9685 JLedPCA9685(jled::PCA9685Hal::PinType pin) {
    return jled::JLedPCA9685(jled::PCA9685Hal(pin, &pwm));
}

// led is using the Arduino HAL and drives the builtin LED
auto led_builtin = JLed(LED_BUILTIN).Blink(500, 500).Forever().LowActive();

// these leds use the PCA985 HAL and drive leds through the I2C bus
auto led12 = JLedPCA9685(12).Breathe(2000).Forever();
auto led13 = JLedPCA9685(13).FadeOn(500).Forever();
auto led14 = JLedPCA9685(14).FadeOff(1000).Forever();
// long form of defining a JLed object with the custom HAL
auto led15 = jled::JLedPCA9685(jled::PCA9685Hal(15, &pwm)).Blink(250, 750).Forever();

void setup() {
    pwm.begin();
}

void loop() {
  led_builtin.Update();
  led12.Update();
  led13.Update();
  led14.Update();
  led15.Update();
}
