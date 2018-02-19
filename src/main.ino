/*
 * It takes about 100 microseconds (0.0001 s) to read an analog input, so the maximum reading rate is about 10,000 times a second. == 10khz
 * 10000 / 2 / 128 == around 78hz per bin
*/

#include "fix_fft.h"
#include <PololuLedStrip.h>

#include "main.h"
#include "config.h"

// fft stuff
char im[FFT_SAMPLE_COUNT], data_raw[FFT_SAMPLE_COUNT];
int data[FFT_SAMPLE_COUNT/2];
uint16_t now;

// led strip stuff
PololuLedStrip<LED_PIN> ledStrip;
rgb_color colors[LED_COUNT];

AnimationControl animation_control;
ShiftingState shifting_state;
BeatingState beating_state;

void animation_set_state(AnimationState state) {
  animation_control.state |= state;
}

void animation_unset_state(AnimationState state) {
  animation_control.state &= ~state;
}

bool animation_is_state(AnimationState state) {
  return (animation_control.state & state);
}

void update() {
  /* Set animation states */
  now = millis();
  if (data[SHIFTING_BIN] >= SHIFTING_BIN_VALUE) {
    if (SHIFTING_ENABLED && !animation_is_state(SHIFTING) && ((now - shifting_state.last) > SHIFTING_INTERVAL) ) {
    // red = 340
    // purple = 140
      animation_set_state(SHIFTING);
    }
  }
  if (data[BEATING_BIN] >= BEATING_BIN_VALUE) {
    if (BEATING_ENABLED && !animation_is_state(BEATING)  && ((now - beating_state.last) > BEATING_INTERVAL) ) {
      animation_set_state(BEATING);
      beating_state.increasing = 1;
    }
  }

  /* Process animation states */
  if ( animation_is_state(SHIFTING)) {
    shifting_state.last = now;
    if (SHIFTING_IS_STATIC)
      animation_control.current_hue = SHIFTING_STATIC_VALUE;
    else
      animation_control.current_hue += SHIFTING_STEP;
    animation_unset_state(SHIFTING);
  }
  if ( animation_is_state(BEATING)) {
    if (!beating_state.increasing) {
      animation_control.current_brightness -= BEATING_STEP;
      if (animation_control.current_brightness <= BEATING_BASE_BRIGHTNESS) {
        animation_control.current_brightness = BEATING_BASE_BRIGHTNESS;
        animation_unset_state(BEATING);
        beating_state.last = now;
      }
    } else {
      animation_control.current_brightness += constrain(map(data[BEATING_BIN], 1, 20, 20, 40), 20, 40);
      if (animation_control.current_brightness >= 255) {
        animation_control.current_brightness = 255;
        beating_state.increasing = 0;
      }
    }
  }
  animation_control.current_hue++;
}

void sample_data() {
  for (int i = 0; i < FFT_SAMPLE_COUNT; i++) {
    data_raw[i] = analogRead(MIC_PIN) / 4 - FFT_SAMPLE_COUNT;
    im[i] = 0;
  }
}

void filter_fft_bin_data() {
  for (int i = 1; i < (FFT_SAMPLE_COUNT/2); i++) {  // ignore first bin (0-78Hz~)
    data[i] = sqrt(data_raw[i] * data_raw[i] + im[i] * im[i]);
    #ifdef DEBUG
    Serial.print(data[i]);
    Serial.print(" ");
    #endif
  }
  #ifdef DEBUG
  Serial.println("");
  #endif
}

void refresh_display() {
  for(int i = 0; i < LED_COUNT; i++)
  {
    byte x = animation_control.current_hue - (i << 1);
    colors[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, animation_control.current_brightness);
  }

  // Write the colors to the LED strip.
  ledStrip.write(colors, LED_COUNT);
}

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  analogReference(DEFAULT);
  animation_set_state(NONE);
}

void loop() {
  sample_data();
  fix_fft(data_raw, im, 7, 0);
  filter_fft_bin_data();
  update();
  refresh_display();
};
