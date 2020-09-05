
#include <stdlib.h>

#include "group.h"
#include "manager.h"
#include "tile.h"
#include "window.h"

#include "../xcb/connection.h"
#include "../xcb/pointer.h"
#include "../xcb/window.h"

group_t *initialize_group() {
    group_t *group = calloc(1, sizeof(group_t));

    group->x = group->y = 0;
    group->height   = xcb_screen->height_in_pixels;
    group->width    = xcb_screen->width_in_pixels;
    group->children = construct_vector();

    push_to_vector(groups, group);

    return group;
}

void push_window_to_group(group_t *group, window_t *window) {
    if (!group->children->size) {
        window->height = (float)group->height;
        window->width  = (float)group->width;

        change_managed_window_geometry(window,
            window->x + group->x, window->y + group->y,
            window->height, window->width);
    } else {
        split_current_view_port(group->children->size % 4, group, focused_window, window);
    }

    push_to_vector(group->children, window);
}

void move_group(group_t *group, unsigned int x, unsigned int y) {
    group->x = x;
    group->y = y;

    window_t *window = NULL;
    while ((window = vector_iterator(group->children)))
        change_managed_window_coordinates(window,
            window->x + group->x, window->y + group->y);
}

void resize_group(group_t *group, unsigned int height, unsigned int width) {
    if (height < 100)
        height = 100;

    if (width < 100)
        width = 100;

    float height_proportion, width_proportion;

    height_proportion = height / (float)group->height;
    width_proportion = width  / (float)group->width;

    window_t *window = NULL;
    while ((window = vector_iterator(group->children))) {
        window->x = (window->x * width_proportion);
        window->y = (window->y * height_proportion);
        window->height = window->height * height_proportion;
        window->width = window->width * width_proportion;

        change_managed_window_geometry(window,
            window->x  + group->x, window->y + group->y,
            window->height, window->width);
    }

    group->height = height;
    group->width  = width;
}
