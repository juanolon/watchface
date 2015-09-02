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
        if (i<2) { // upperpart
            box = GRect(orig_x+((i%2)*HOUR_BOX_WIDTH), orig_y, HOUR_BOX_WIDTH, HOUR_BOX_HEIGHT);
        } else { // lowerpart
            box = GRect(orig_x+((i%2)*HOUR_BOX_WIDTH), orig_y+HOUR_BOX_HEIGHT, HOUR_BOX_WIDTH, HOUR_BOX_HEIGHT);
        }

        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_fill_rect(ctx, box, HOUR_BOX_RADIUS, GCornersTop);
        snprintf(digit, 10, "%d", data->hour[i]); // integer to string

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
    if (clockLayer) {
        layer_destroy(clockLayer);
    }
}

void clock_layer_set_time(ClockLayer* clockLayer, char* time){
    Clock *data = (Clock *)layer_get_data(clockLayer);

    // small hack for spliting the string:
    // 1. buffer twice the size
    // 2. copy each character separate by null characters
    // 3. ???
    // 4. profit
    char test[8];
    for(int i=0; i<4;i++){
        test[i*2] = time[i];
    }
    for(int i=0; i<4;i++){
        data->hour[i] = atoi(&test[i*2]);
    }

    layer_mark_dirty(clockLayer);
}
