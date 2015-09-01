#pragma once

#include <pebble.h>

typedef struct Clock{
    GFont *font;
    int hour[8];
} Clock;

typedef struct Clock Clock;
typedef Layer ClockLayer;

TextLayer* clock_get_digit_text_layer(Clock *, GRect);

ClockLayer* clock_layer_create(GRect, GFont *);

void clock_layer_destroy(ClockLayer *);

void clock_set_time(Clock *, char *); // DEPRECATE

void clock_layer_set_time(ClockLayer *, char *);
