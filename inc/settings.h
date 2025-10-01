#ifndef SETTINGS_H
# define SETTINGS_H

// basic settings
// SCALE: image size multiblied by the scale(scale 1 is 1920x1080)
// AA_MIN_SAMPLES: the minimum amount of rays per pixel (should be between 8 and 32)
// MAX_DEPTH: the number of child rays that are created when there is an intersection (should be between 8 and 64)
# define SCALE 0.5
# define AA_MIN_SAMPLES 32
# define MAX_DEPTH 64

// for lume and color intensity(dont need to be modified normally)
# define AA_VAR_EPS 0.0004
# define COLOR_INTENSITY 0.5

// will be set automaticly(dont change them)
# define WIDTH (1920 * SCALE)
# define HEIGHT (1080 * SCALE)
# define ASPECT_RATIO ((double)WIDTH / (double)HEIGHT)
# define AA_MAX_SAMPLES (AA_MIN_SAMPLES * 8)

# ifndef ANTI_ALIASING
#  define ANTI_ALIASING 0
# endif

#endif