#include <pebble.h>
#include <clock.h>
#include <bottombar.h>

static Window *s_main_window;
static GFont *s_day_font;
static GFont *s_clock_font;
static ClockLayer *s_clock_layer;
static BottomBarLayer *s_bar_layer;

static void update_ui() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    char hour[] = "0000";
    if (clock_is_24h_style() == true) {
        strftime(hour, sizeof(hour), "%H%M", tick_time);
    } else {
        strftime(hour, sizeof(hour), "%I%M", tick_time);
    }
    clock_layer_set_time(s_clock_layer, hour);
    APP_LOG(APP_LOG_LEVEL_INFO, hour);

    char date[] = "00";
    strftime(date, sizeof(date), "%d", tick_time);
    bar_layer_set_date(s_bar_layer, date);

    char day[] = "0";
    // TODO handle setting "start on monday"
    strftime(day, sizeof(day), "%w", tick_time);
    bar_layer_set_day(s_bar_layer, day);
}

static void main_window_load(Window *window){
    Layer *window_layer = window_get_root_layer(window);

    s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RUBYK_ONE_15));
    s_clock_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RUBYK_ONE_55));

    // 144x168
    // TODO pass a child layer instead
    s_clock_layer = clock_layer_create(GRect(0, 0, 144, 144), s_clock_font);
    layer_add_child(window_layer, s_clock_layer);

    // 144x16
    s_bar_layer = bar_layer_create(GRect(0, 144, 144, 30), s_day_font);
    layer_add_child(window_layer, s_bar_layer);
    update_ui();
}

static void main_window_unload(Window *window){
    clock_layer_destroy(s_clock_layer);
    bar_layer_destroy(s_bar_layer);
}

static void tick_handler(struct tm *stick_time, TimeUnits units_changed){
    update_ui();
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
    app_event_loop();
    deinit();
}
