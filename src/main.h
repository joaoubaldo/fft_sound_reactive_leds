#ifndef MAIN_H
#define MAIN_H
#include "config.h"

// animation control
enum AnimationState {
  NONE,
  SHIFTING,
  BEATING
};

struct AnimationControl {
   uint16_t current_hue;
   uint16_t current_brightness = 100;
   char state;
};

typedef struct ShiftingState {
  uint16_t last;
} ShiftingState;

typedef struct BeatingState {
  uint16_t last;
  bool increasing;
} BeatingState;


// Set animation state
void animation_set_state(AnimationState state);

// Remove animation state
void animation_unset_state(AnimationState state);

// Query animation state
bool animation_is_state(AnimationState state);

// Post process fft bin data
void filter_fft_bin_data();

// Process all animation based logic
void update();

// Write current state to leds
void refresh_display();

// Read analog data
void sample_data();

void setup();

void loop();
#endif
