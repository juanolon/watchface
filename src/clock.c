#include <pebble.h>
#include <clock.h>

TextLayer* clock_get_digit_text_layer(Clock *clock, GRect rect){
    TextLayer* s_digit_layer = text_layer_create(rect);

#ifdef PBL_COLOR
    text_layer_set_background_color(s_digit_layer, GColorOrange);
    text_layer_set_text_color(s_digit_layer, GColorDarkGreen);
#else
    text_layer_set_background_color(s_digit_layer, GColorBlack);
    text_layer_set_text_color(s_digit_layer, GColorClear);
#endif

    text_layer_set_font(s_digit_layer, clock->font);
    text_layer_set_text_alignment(s_digit_layer, GTextAlignmentCenter);
    return s_digit_layer;
}

Clock* create_clock(GFont *font){
    Clock *clock = (Clock*)malloc(sizeof(Clock));
    if (!clock) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "failed to allocate memory for the clock");
        return NULL;
    }

    clock->font = font;

    clock->upleft = clock_get_digit_text_layer(clock, GRect(10, 10, 57, 58));
    clock->upright = clock_get_digit_text_layer(clock, GRect(77, 10, 57, 58));
    clock->downleft = clock_get_digit_text_layer(clock, GRect(10, 72, 57, 58));
    clock->downright = clock_get_digit_text_layer(clock, GRect(77, 72, 57, 58));

    return clock;
}

void clock_set_time(Clock *clock, char *cTime){

    char *test = (char*)malloc(sizeof(9));

    for(int i=0; i<7;i++){
        test[i] = '\0';
    }

    // TODO refactor:
    // http://stackoverflow.com/a/2104846
    // strcpy(test, '\0..')
    for(int i=0; i<4;i++){
        test[i*2] = cTime[i];
    }

    APP_LOG(APP_LOG_LEVEL_INFO, "setting time:");
    APP_LOG(APP_LOG_LEVEL_INFO, &test[0]);
    APP_LOG(APP_LOG_LEVEL_INFO, &test[2]);
    APP_LOG(APP_LOG_LEVEL_INFO, &test[4]);
    APP_LOG(APP_LOG_LEVEL_INFO, &test[6]);

    text_layer_set_text(clock->upleft, &test[0]);
    text_layer_set_text(clock->upright, &test[2]);
    text_layer_set_text(clock->downleft, &test[4]);
    text_layer_set_text(clock->downright, &test[6]);
}

void clock_attach_to_window(Clock *clock, Layer *layer){
    layer_add_child(layer, text_layer_get_layer(clock->upleft));
    layer_add_child(layer, text_layer_get_layer(clock->upright));
    layer_add_child(layer, text_layer_get_layer(clock->downleft));
    layer_add_child(layer, text_layer_get_layer(clock->downright));
}

void clock_unload_clock(Clock *clock){
    text_layer_destroy(clock->upleft);
    text_layer_destroy(clock->upright);
    text_layer_destroy(clock->downleft);
    text_layer_destroy(clock->downright);
    fonts_unload_custom_font(clock->font);
    free(clock);
    clock = NULL;
}

void clock_update_time(Clock *clock){
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    char buffer[] = "0000";

    // TODO get from the settings
    if (clock_is_24h_style() == true) {
        strftime(buffer, sizeof(buffer), "%H%M", tick_time);
    } else {
        // use 12 hour format
        strftime(buffer, sizeof(buffer), "%I%M", tick_time);
    }

    clock_set_time(clock, buffer);
}

