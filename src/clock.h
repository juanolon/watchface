#pragma once

#include <pebble.h>

typedef struct Clock{
    TextLayer *upleft, *upright, *downleft, *downright;
    GFont *font;
} Clock;

typedef struct Clock Clock;

TextLayer* clock_get_digit_text_layer(Clock *, GRect);

Clock* create_clock(GFont *);

void clock_set_time(Clock *, char *);

void clock_attach_to_window(Clock *, Layer *);

void clock_unload_clock(Clock *);

void clock_update_time(Clock *);
