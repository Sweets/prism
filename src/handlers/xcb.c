#include "xcb.h"

#include "../util/logging.h"
#include "../util/vector.h"
#include "../wm/manager.h"
#include "../wm/tile.h"
#include "../wm/window.h"
#include "../xcb/connection.h"
#include "../xcb/ewmh.h"
#include "../xcb/pointer.h"
#include "../xcb/window.h"

void (*xcb_events[XCB_NO_OPERATION])(xcb_generic_event_t*) = {
    [XCB_MAP_REQUEST]    = handle_window_map_request,
    [XCB_DESTROY_NOTIFY] = handle_window_destruction,

    [XCB_BUTTON_PRESS]   = handle_button_down,
    [XCB_BUTTON_RELEASE] = handle_button_up,
    [XCB_MOTION_NOTIFY]  = handle_pointer,
};

void *store;

void handle_window_map_request(xcb_generic_event_t *generic_event) {
    xcb_map_request_event_t *event;
    event = (xcb_map_request_event_t *)generic_event;

    xcb_window_t window_id = event->window;
    xcb_ewmh_get_atoms_reply_t *window_type = NULL;
    xcb_get_property_cookie_t type_cookie;

    type_cookie = xcb_ewmh_get_wm_window_type(ewmh_connection, window_id);
    xcb_get_window_attributes_reply_t *attributes = get_window_attributes(
        window_id);

    if (attributes && attributes->override_redirect)
        goto map;

    xcb_ewmh_get_wm_window_type_reply(ewmh_connection, type_cookie,
        window_type, NULL);

    if (window_type) {
        unsigned int index = 0;
        xcb_atom_t atom;

        while ((atom = window_type->atoms[index++])) {
            // special case: ewmh_connection->_NET_WM_WINDOW_TYPE_SPLASH
            // centered
            if (atom != ewmh_connection->_NET_WM_WINDOW_TYPE_NORMAL) {
                xcb_ewmh_get_atoms_reply_wipe(window_type);
                goto map;
            }
        }

        xcb_ewmh_get_atoms_reply_wipe(window_type);
    }

    window_t *window = manage_window(window_id);
    window_id = window->parent;

    push_window_to_group(focused_group, window);
    push_to_vector(managed_windows, window);

    if (focused_window)
        window_grab_buttons(focused_window->id,
            XCB_BUTTON_INDEX_ANY ^ XCB_BUTTON_INDEX_4 ^ XCB_BUTTON_INDEX_5,
            XCB_MOD_MASK_ANY);

    map_window(window->id);
    focus_window(window->id);
    raise_window(window->id);
    focused_window = window;
    
    flush();
    return;

map:
    map_window(window_id);
    flush();
}

void handle_window_destruction(xcb_generic_event_t *generic_event) {
    xcb_destroy_notify_event_t *event;
    event = (xcb_destroy_notify_event_t *)generic_event;

    xcb_window_t window_id = event->window;
    window_t *window = window_from_id(window_id);

    if (!window)
        return;

    if (focused_window->id == window->id)
        focused_window = NULL;

    pull_window_from_group(focused_group, window); // Determine which group it belongs to
    claim_freed_space(window, focused_group);
    unmanage_window(window_id);
    flush();
}

void handle_button_down(xcb_generic_event_t *generic_event) {
    xcb_button_press_event_t *event;
    event = (xcb_button_press_event_t*)generic_event;

    xcb_window_t window_id = event->event;
    window_t *window = NULL;

    while ((window = vector_iterator(managed_windows))) {
        if (window->parent == window_id) {
            window_id = window->id;
            reset_vector_iterator(managed_windows);
            break;
        }
    }

    window = window_from_id(window_id);
    if (!window)
        return; // unmanaged window

    if (focused_window)
        window_grab_buttons(focused_window->id,
            XCB_BUTTON_INDEX_ANY ^ XCB_BUTTON_INDEX_4 ^ XCB_BUTTON_INDEX_5,
            XCB_MOD_MASK_ANY);

    window_release_buttons(window->id,
        XCB_BUTTON_INDEX_ANY, XCB_MOD_MASK_ANY);
    raise_window(window->id);
    focus_window(window->id);
    focused_window = window;

    flush();

/*    log_debug("POINTER");
    xcb_grab_pointer(xcb_connection, XCB_NONE,
        xcb_screen->root,
        XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_BUTTON_MOTION |
        XCB_EVENT_MASK_POINTER_MOTION_HINT,
        XCB_GRAB_MODE_ASYNC,
        XCB_GRAB_MODE_ASYNC,
        xcb_screen->root,
        XCB_NONE,
        XCB_CURRENT_TIME);
    flush();

    store = get_pointer_coordinates();*/
}

void handle_button_up(xcb_generic_event_t *generic_event) {
    xcb_ungrab_pointer(xcb_connection, XCB_CURRENT_TIME);
}

void handle_pointer(xcb_generic_event_t *generic_event) {
    xcb_motion_notify_event_t *event;
    event = (xcb_motion_notify_event_t*)generic_event;

    xcb_query_pointer_reply_t *p = get_pointer_coordinates();
    if (event->state & XCB_BUTTON_MASK_1) {
        move_group(focused_group, p->root_x, p->root_y);
    } else if (event->state & XCB_BUTTON_MASK_3) {
        unsigned int height, width;

	    width = p->root_x - focused_group->x;
	    height = p->root_y - focused_group->y;

        resize_group(focused_group, height, width);
    }

    flush();
}
