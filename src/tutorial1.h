#include <pebble.h>

#ifndef CLOCK_H
#define CLOCK_H

//typedef struct Clock Clock;
struct Clock;


TextLayer* clock_get_digit_text_layer(Clock *clock, GRect rect);

Clock* get_clock(void);

void clock_set_time(Clock *clock, char *first_minute, char *second_minute, char *first_second, char *second_second);
// void clock_set_time(Clock *clock, char hour);

void clock_attach_to_window(Clock *clock, Layer *layer);


void clock_unload_clock(Clock *clock);

#endif
