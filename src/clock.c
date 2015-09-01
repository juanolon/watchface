#include <pebble.h>
#include <clock.h>

#define HOUR_BOX_RADIUS 2
#define HOUR_BOX_WIDTH 70 // 144/2
#define HOUR_BOX_HEIGHT 60

void clock_update_proc(ClockLayer* clock_layer, GContext* ctx) {
    Clock *data = (Clock *)layer_get_data(clock_layer);
    GRect bounds = layer_get_bounds(clock_layer);
    GRect box;

    graphics_context_set_fill_color(ctx, GColorOrange);
    graphics_context_set_text_color(ctx, GColorDarkGreen);

    int orig_y = bounds.origin.y;
    int orig_x = bounds.origin.x;
    char digit[1];

    // Draw days
    for (int i = 0; i < 4; ++i) {
        if (i<2) { // up
            box = GRect(orig_x+((i%2)*HOUR_BOX_WIDTH), orig_y, HOUR_BOX_WIDTH, HOUR_BOX_HEIGHT);
        } else { // down
            box = GRect(orig_x+((i%2)*HOUR_BOX_WIDTH), orig_y+HOUR_BOX_HEIGHT, HOUR_BOX_WIDTH, HOUR_BOX_HEIGHT);
        }

        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_fill_rect(ctx, box, HOUR_BOX_RADIUS, GCornersTop);
        snprintf(digit, 10, "%d", data->hour[i]);

        APP_LOG(APP_LOG_LEVEL_INFO, digit);
        graphics_draw_text(
                ctx,
                digit,
                data->font,
                box,
                GTextOverflowModeFill,
                GTextAlignmentCenter,
                NULL
                );
    }
}

ClockLayer* clock_layer_create(GRect frame, GFont* font){
    ClockLayer *clock_layer = layer_create_with_data(frame, sizeof(Clock));
    layer_set_update_proc(clock_layer, clock_update_proc);

    Clock *bar = (Clock *)layer_get_data(clock_layer);
    bar->font = font;

    return clock_layer;
}

void clock_layer_destroy(ClockLayer* clockLayer){
    // TODO need to destroy child layers?
    if (clockLayer) {
        layer_destroy(clockLayer);
    }
}

void clock_layer_set_time(ClockLayer* clockLayer, char* time){
    Clock *data = (Clock *)layer_get_data(clockLayer);

    char *test = (char*)malloc(sizeof(9));

    for(int i=0; i<7;i++){
        test[i] = '\0';
    }
    for(int i=0; i<4;i++){
        test[i*2] = time[i];
    }

    for(int i=0; i<4;i++){
        data->hour[i] = atoi(&test[i*2]);
    }
    layer_mark_dirty(clockLayer);
}
