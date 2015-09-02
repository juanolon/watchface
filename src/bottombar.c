#include <pebble.h>
#include <bottombar.h>

// TODO translations?
#define DAYS "S\0M\0T\0W\0T\0F\0S"
#define BACKGROUND GColorOrange
#define CURRENT_BACKGROUND GColorBlueMoon

#define DAY_BOX_RADIUS 2
#define DAY_BOX_WIDTH 15 // 144/8
#define DAY_BOX_HEIGHT 20

void bar_update_proc(BottomBarLayer* bar_layer, GContext* ctx) {
    BottomBar *data = (BottomBar *)layer_get_data(bar_layer);
    GRect bounds = layer_get_bounds(bar_layer);
    GRect box;

    // attribute: day background color
    graphics_context_set_fill_color(ctx, GColorGreen);
    graphics_context_set_text_color(ctx, GColorBlack);

    int orig_y = bounds.origin.y;
    int orig_x = bounds.origin.x;

    // Draw days
    for (int i = 0; i < 7; ++i) {
        box = GRect(orig_x+(i*DAY_BOX_WIDTH), orig_y, DAY_BOX_WIDTH, DAY_BOX_HEIGHT);

        if (i == data->currentDay) {
            graphics_context_set_fill_color(ctx, CURRENT_BACKGROUND);
        } else {
            graphics_context_set_fill_color(ctx, BACKGROUND);
        }
        graphics_fill_rect(ctx, box, DAY_BOX_RADIUS, GCornersTop);

        graphics_draw_text(
                ctx,
                &DAYS[i*2],
                data->font,
                box,
                GTextOverflowModeFill,
                GTextAlignmentCenter,
                NULL
                );
    }

    // Draw date
    graphics_context_set_fill_color(ctx, GColorGreen);
    box = GRect(orig_x+(8*DAY_BOX_WIDTH), orig_y, DAY_BOX_WIDTH*2, DAY_BOX_HEIGHT);

    graphics_draw_text(
            ctx,
            data->date,
            data->font,
            box,
            GTextOverflowModeFill,
            GTextAlignmentLeft,
            NULL
            );
}

// sorry the camelCase and underscore_case mix :P
BottomBarLayer* bar_layer_create(GRect frame, GFont* font){
    BottomBarLayer *bar_layer = layer_create_with_data(frame, sizeof(BottomBar));
    layer_set_update_proc(bar_layer, bar_update_proc);

    BottomBar *bar = (BottomBar *)layer_get_data(bar_layer);
    bar->font = font;
    bar->currentDay = 0;
    bar->startOnMonday = true;

    return bar_layer;
}

void bar_layer_destroy(BottomBarLayer* barLayer){
    if (barLayer) {
        layer_destroy(barLayer);
    }
}

void bar_layer_set_day(BottomBarLayer* barLayer, char* dayIndex){
    BottomBar *data = (BottomBar *)layer_get_data(barLayer);
    data->currentDay = atoi(dayIndex);
    layer_mark_dirty(barLayer);
}

void bar_layer_set_date(BottomBarLayer* barLayer, char* date){
    BottomBar *data = (BottomBar *)layer_get_data(barLayer);
    memcpy(data->date, date, 2);
    layer_mark_dirty(barLayer);
}
