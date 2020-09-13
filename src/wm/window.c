#include <stdlib.h>

#include "manager.h"
#include "group.h"
#include "window.h"

#include "../util/vector.h"
#include "../xcb/connection.h"
#include "../xcb/window.h"

window_t *manage_window(xcb_window_t window_id) {
    window_t *window = calloc(1, sizeof(window_t));

    window->id = window_id;
    window->parent = xcb_generate_id(xcb_connection);

    window->x      = window->y     = 0;
    window->height = window->width = 1;

    /* Create the parent window */

    unsigned int masked_values =
        XCB_CW_BACK_PIXEL        |
        XCB_CW_BORDER_PIXEL      |
        XCB_CW_OVERRIDE_REDIRECT |
        XCB_CW_COLORMAP;

    unsigned int values[] = {
        0, 0, 1, screen_colormap
    };

    xcb_create_window(xcb_connection, 32, window->parent, xcb_screen->root,
        0, 0, 1, 1,
        0, /* Border size */
        XCB_WINDOW_CLASS_INPUT_OUTPUT, screen_visual->visual_id,
        masked_values, values);

    values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
    xcb_change_window_attributes(xcb_connection, window->parent,
        XCB_CW_EVENT_MASK, values);

    xcb_reparent_window(xcb_connection, window->id, window->parent, 0, 0);
    map_window(window->id);

    push_to_vector(managed_windows, window);

    return window;
}

void unmanage_window(xcb_window_t window_id) {
    unsigned int index = 0;
    window_t *window = NULL;

    for (; index < managed_windows->size; index++) {
        window = get_from_vector(managed_windows, index);

        if (window->id == window_id) {
            xcb_destroy_window(xcb_connection, window->parent);

            pull_from_vector(managed_windows, index);
            reset_vector_iterator(managed_windows);

            free(window);
            return;
        }
    }
}

window_t *window_from_id(xcb_window_t id) {
    window_t *window = NULL;
    while ((window = vector_iterator(managed_windows))) {
        if (window->id == id) {
            reset_vector_iterator(managed_windows);
            return window;
        }
    }

    return NULL;
}

void change_managed_window_coordinates(window_t *window, float x, float y) {
    change_window_coordinates(window->parent, (unsigned int)x, (unsigned int)y);
}

void change_managed_window_dimensions(window_t *window, float height,
    float width) {
    change_window_dimensions(window->parent, (unsigned int)height,
        (unsigned int)width);
    change_window_dimensions(window->id,     (unsigned int)height,
        (unsigned int)width);
}

void change_managed_window_geometry(window_t *window, float x, float y,
    float height, float width) {
    change_window_geometry(window->parent, (unsigned int)x, (unsigned int)y,
        (unsigned int)height, (unsigned int)width);
    change_window_dimensions(window->id, (unsigned int)height,
        (unsigned int)width);
}
