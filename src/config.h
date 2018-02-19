#ifndef CONFIG_H
#define CONFIG_H
/* Configuration */

// #define DEBUG

const int LED_PIN = 12;  // Pin connected to Pololu Led Strip data pin
const int MIC_PIN = A0;  // Analog pin connected to audio signal.
const int LED_COUNT = 22;  // Number of leds in control.
const int FFT_SAMPLE_COUNT = 128;  // Number of samples to read. FFT bins are half of this value.

/* Shifting animation
This animation shifts color gradient by STEP every INTERVAL when BIN value exceeds BIN_VALUE.
If IS_STATIC is true, the shifting doesnt use STEP, instead, the current hue value is set to STATIC_VALUE when BIN exceeds BIN_VALUE.
*/
const bool SHIFTING_ENABLED = false;
const uint16_t SHIFTING_INTERVAL = 333;
const uint16_t SHIFTING_STEP = 30;
const int SHIFTING_BIN = 1;
const int SHIFTING_BIN_VALUE = 5;
const bool SHIFTING_IS_STATIC = false;
const uint16_t SHIFTING_STATIC_VALUE = 350;

/* Beating animation
This animation increases brightness by STEP every INTERVAL when BIN value exceeds BIN_VALUE.
When the threshold (BIN_VALUE) is not met, led brightness is set at BASE_BRIGHTNESS (should be < 200).
When the threshold is met, the led brightness goes up to 255.
*/
const bool BEATING_ENABLED = true;
const uint16_t BEATING_INTERVAL = 200;
const uint16_t BEATING_STEP = 30;
const uint16_t BEATING_BASE_BRIGHTNESS = 50;
const int BEATING_BIN = 1;
const int BEATING_BIN_VALUE = 5;
#endif
