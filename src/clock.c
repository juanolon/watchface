#include <pebble.h>
#include <clock.h>

#define DIGIT_BOX_RADIUS 2
/* #define DIGIT_BOX_WIDTH 72 // 144/2 */
/* #define DIGIT_BOX_HEIGHT 77 */
#define DIGIT_BOX_BORDER 7

#define CLOCK_BACKGROUND GColorWhite
#define CLOCK_DIGIT_COLOR GColorBlack

void clock_update_proc(ClockLayer* clock_layer, GContext* ctx) {
    Clock *data = (Clock *)layer_get_data(clock_layer);
    GRect bounds = layer_get_bounds(clock_layer);

    graphics_context_set_text_color(ctx, CLOCK_DIGIT_COLOR);

    int orig_y = bounds.origin.y;
    int orig_x = bounds.origin.x;
    int box_height = bounds.size.h/2;
    int box_width = bounds.size.w/2;
    char digit[1];
    GRect box;

    // Draw days
    for (int i = 0; i < 4; ++i) {
        if (i<2) { // upperpart
            box = GRect(orig_x+((i%2)*box_width), orig_y, box_width, box_height);
        } else { // lowerpart
            box = GRect(orig_x+((i%2)*box_width), orig_y+box_height, box_width, box_height);
        }

        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_fill_rect(ctx, box, DIGIT_BOX_RADIUS, GCornerNone);

        // draw a smaller box
        box.origin.x = box.origin.x + DIGIT_BOX_BORDER;
        box.origin.y = box.origin.y + DIGIT_BOX_BORDER;
        box.size.w = box.size.w - DIGIT_BOX_BORDER*2;
        box.size.h = box.size.h - DIGIT_BOX_BORDER*2;

        graphics_context_set_fill_color(ctx, CLOCK_BACKGROUND);
        graphics_fill_rect(ctx, box, DIGIT_BOX_RADIUS, GCornerNone);

        box.size.h = box.size.h - 30;
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
