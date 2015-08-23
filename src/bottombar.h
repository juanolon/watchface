#pragma once

#include <pebble.h>

typedef struct BottomBar{
    GFont *font;
    int currentDay;
    bool startOnMonday;
    char date[2];
} BottomBar;

typedef struct BottomBar BottomBar;
typedef Layer BottomBarLayer;

BottomBarLayer* bar_layer_create(GRect, GFont *);

void bar_update_proc(BottomBarLayer *, GContext *);

void bar_layer_destroy(BottomBarLayer *);

void bar_layer_set_day(BottomBarLayer *, char *);

void bar_layer_set_date(BottomBarLayer *, char*);
