#pragma once
#include <Arduino.h>
// Waveshare RP2040 Zero
// https://www.waveshare.com/wiki/RP2040-Zero
// https://www.waveshare.com/w/upload/4/4c/RP2040_Zero.pdf
// https://www.waveshare.com/img/devkit/RP2040-Zero/RP2040-Zero-details-7.jpg
//
// https://www.mischianti.org/2022/09/19/waveshare-rp2040-zero-high-resolution-pinout-and-specs/
// https://www.mischianti.org/wp-content/uploads/2022/09/Waveshare-rp2040-zero-Raspberry-Pi-Pico-alternative-pinout.jpg

/*
                Pin#              Pin#
                    ___(_____)___
              5v 1 |   *USB C*   | 23 GPIO0
             GND 2 |             | 22 GPIO1
            3.3v 3 |             | 21 GPIO2
          GPIO29 4 |             | 20 GPIO3
          GPIO28 5 |             | 19 GPIO4
          GPIO27 6 |             | 18 GPIO5
          GPIO26 7 |             | 17 GPIO6
          GPIO15 8 |             | 16 GPIO7
          GPIO14 9 |__|_|_|_|_|__| 15 GPIO8
                      1 1 1 1 1
                      0 1 2 3 4

                    Pin10 = GPIO13
                    Pin11 = GPIO12
                    Pin12 = GPIO11
                    Pin13 = GPIO10
                    Pin14 = GPIO9
*/

// NeoPixel
#define PIN_NEOPIXEL   (16u)
//#define LED_BUILTIN PIN_NEOPIXEL

// Serial1
#define PIN_SERIAL1_TX (0u)
#define PIN_SERIAL1_RX (1u)

#define PIN_SERIAL2_TX (8u)
#define PIN_SERIAL2_RX (9u)

// SPI
// #define PIN_SPI0_MISO  (4u)
// #define PIN_SPI0_MOSI  (3u)
// #define PIN_SPI0_SCK   (2u)
// #define PIN_SPI0_SS    (5u)
#define PIN_SPI0_MISO  (0u)
#define PIN_SPI0_MOSI  (3u)
#define PIN_SPI0_SCK   (2u)
#define PIN_SPI0_SS    (1u)

#define PIN_SPI1_MISO  (12u)
#define PIN_SPI1_MOSI  (15u)
#define PIN_SPI1_SCK   (14u)
#define PIN_SPI1_SS    (13u)

// Wire
#define PIN_WIRE0_SDA  (4u)
#define PIN_WIRE0_SCL  (5u)

#define PIN_WIRE1_SDA  (26u)
#define PIN_WIRE1_SCL  (27u)
  
#define SERIAL_HOWMANY (2u)
#define SPI_HOWMANY    (2u)
#define WIRE_HOWMANY   (2u)

#define PINS_COUNT          (30u)
#define NUM_DIGITAL_PINS    (30u)
#define NUM_ANALOG_INPUTS   (4u)
#define NUM_ANALOG_OUTPUTS  (0u)
#define ADC_RESOLUTION      (12u)

#define LED_BUILTIN PIN_LED

static const uint8_t D0 = (0u);
static const uint8_t D1 = (1u);
static const uint8_t D2 = (2u);
static const uint8_t D3 = (3u);
static const uint8_t D4 = (4u);
static const uint8_t D5 = (5u);
static const uint8_t D6 = (6u);
static const uint8_t D7 = (7u);
static const uint8_t D8 = (8u);
static const uint8_t D9 = (9u);
static const uint8_t D10 = (10u);
static const uint8_t D11 = (11u);
static const uint8_t D12 = (12u);
static const uint8_t D13 = (13u);
static const uint8_t D14 = (14u);
static const uint8_t D15 = (15u);
static const uint8_t D16 = (16u);
static const uint8_t D17 = (17u);
static const uint8_t D18 = (18u);
static const uint8_t D19 = (19u);
static const uint8_t D20 = (20u);
static const uint8_t D21 = (21u);
static const uint8_t D22 = (22u);
static const uint8_t D23 = (23u);
static const uint8_t D24 = (24u);
static const uint8_t D25 = (25u);
static const uint8_t D26 = (26u);
static const uint8_t D27 = (27u);
static const uint8_t D28 = (28u);
static const uint8_t D29 = (29u);

// #ifdef __PIN_A0
// 	static const uint8_t A0 = __PIN_A0;
// #else
// 	static const uint8_t A0 = (26u);
// #endif

// #ifdef __PIN_A1
// 	static const uint8_t A1 = __PIN_A1;
// #else
// 	static const uint8_t A1 = (27u);
// #endif

// #ifdef __PIN_A2
// 	static const uint8_t A2 = __PIN_A2;
// #else
// 	static const uint8_t A2 = (28u);
// #endif

// #ifdef __PIN_A3
// 	static const uint8_t A3 = __PIN_A3;
// #else
// 	static const uint8_t A3 = (29u);
// #endif

// static const uint8_t SS = PIN_SPI0_SS;
// static const uint8_t MOSI = PIN_SPI0_MOSI;
// static const uint8_t MISO = PIN_SPI0_MISO;
// static const uint8_t SCK = PIN_SPI0_SCK;

// static const uint8_t SDA = PIN_WIRE0_SDA;
// static const uint8_t SCL = PIN_WIRE0_SCL;

