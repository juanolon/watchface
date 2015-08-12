#include <pebble.h>
#include <tutorial1.h>

typedef struct Clock{
    TextLayer *upleft, *upright, *downleft, *downright;
    GFont font;
} Clock;

static Window *s_main_window;
static Clock *s_clock;

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

Clock* get_clock(){
    Clock *clock = (Clock*)malloc(sizeof(Clock));
    if (!clock) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "failed to allocate memory for the clock");
        return NULL;
    }

    clock->font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RUBYK_ONE_55));

    clock->upleft = clock_get_digit_text_layer(clock, GRect(10, 10, 57, 58));
    clock->upright = clock_get_digit_text_layer(clock, GRect(77, 10, 57, 58));
    clock->downleft = clock_get_digit_text_layer(clock, GRect(10, 72, 57, 58));
    clock->downright = clock_get_digit_text_layer(clock, GRect(77, 72, 57, 58));

    return clock;
}

void clock_set_time(Clock *clock, char *first_minute, char *second_minute, char *first_second, char *second_second){
/* void clock_set_time(Clock *clock, char hour){ */
    /* first_minute[1] = '\0'; */
    APP_LOG(APP_LOG_LEVEL_INFO, "setting time:");
    APP_LOG(APP_LOG_LEVEL_INFO, first_minute);
    APP_LOG(APP_LOG_LEVEL_INFO, second_minute);
    APP_LOG(APP_LOG_LEVEL_INFO, first_second);
    APP_LOG(APP_LOG_LEVEL_INFO, second_second);

    /* APP_LOG(APP_LOG_LEVEL_INFO, "setting time:"); */
    /* char * i; */
    /* for (i = hour; *hour; ++i) { */
    /*     APP_LOG(APP_LOG_LEVEL_INFO, i); */
    /* } */
    /* APP_LOG(APP_LOG_LEVEL_INFO, (*hour)[1]); */
    /* APP_LOG(APP_LOG_LEVEL_INFO, (*hour)[2]); */
    /* APP_LOG(APP_LOG_LEVEL_INFO, (*hour)[3]); */
    text_layer_set_text(clock->upleft, first_minute);
    text_layer_set_text(clock->upright, second_minute);
    text_layer_set_text(clock->downleft, first_second);
    text_layer_set_text(clock->downright, second_second); }

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

static void update_time(){
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    char buffer[] = "0000";
    /* static char hours[] = "00"; */
    /* static char minutes[] = "00"; */

    if (clock_is_24h_style() == true) {
        strftime(buffer, sizeof(buffer), "%H%M", tick_time);
        /* strftime(hours, sizeof(hours), "%H", tick_time); */
    } else {
        // use 12 hour format
        strftime(buffer, sizeof(buffer), "%I%M", tick_time);
        /* strftime(hours, sizeof(hours), "%I", tick_time); */
    }

    /* strftime(minutes, sizeof(minutes), "%M", tick_time); */

    /* char fHour, sHour, fMinute, sMinute; */
    /*  */
    /* fHour   = *(hours[0]); */
    /* sHour   = *(hours[1]); */
    /* fMinute = *(minutes[0]); */
    /* sMinute = *(minutes[1]); */
    /*  */
    /* APP_LOG(APP_LOG_LEVEL_INFO, "update_time"); */
    /* APP_LOG(APP_LOG_LEVEL_INFO, *&buffer[0]); */
    /* APP_LOG(APP_LOG_LEVEL_INFO, *buffer[1]); */

    /* clock_set_time(s_clock, &((*buffer)[0]), &buffer[1], &buffer[3], &buffer[4]); */
    /* clock_set_time(s_clock, fHour, sHour, fMinute, sMinute); */
    clock_set_time(s_clock, &buffer[0], &buffer[1], &buffer[2], &buffer[3]);
    /* clock_set_time(s_clock, &buffer[0], "3", "3", "0"); */
    /* clock_set_time(s_clock, "2", "3", "3", "0"); */
    /* clock_set_time(s_clock, buffer); */
}

static void main_window_load(Window *window){
    s_clock = get_clock();
    clock_attach_to_window(s_clock, window_get_root_layer(window));
    update_time();
}

static void main_window_unload(Window *window){
    clock_unload_clock(s_clock);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
    update_time();
}

static void init(){
    s_main_window = window_create();

    window_set_window_handlers(s_main_window, (WindowHandlers) {
            .load = main_window_load,
            .unload = main_window_unload
        });

    window_stack_push(s_main_window, true);

    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(){
    window_destroy(s_main_window);
}

int main(void){
    init();

    //waits for system events until exits
    app_event_loop();
    deinit();
}
