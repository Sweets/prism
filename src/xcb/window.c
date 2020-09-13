#include <xcb/xcb_icccm.h>

#include "connection.h"
#include "ewmh.h"
#include "window.h"

void configure_window(xcb_window_t window_id, unsigned int value_mask,
    unsigned int *values) {
    xcb_configure_window(xcb_connection, window_id, value_mask, values);
}

void map_window(xcb_window_t window_id) {
    xcb_map_window(xcb_connection, window_id);
}

void unmap_window(xcb_window_t window_id) {
    xcb_unmap_window(xcb_connection, window_id);
}

void focus_window(xcb_window_t window_id) {
    unsigned int values[2] = {
        XCB_ICCCM_WM_STATE_NORMAL,
        XCB_NONE
    };

    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, window_id,
        ewmh_connection->_NET_WM_STATE, ewmh_connection->_NET_WM_STATE,
        32, 2, values);

    xcb_set_input_focus(xcb_connection, XCB_INPUT_FOCUS_POINTER_ROOT,
        window_id, XCB_CURRENT_TIME);
    xcb_ewmh_set_active_window(ewmh_connection, 0, window_id);
}

void raise_window(xcb_window_t window_id) {
    unsigned int value_mask = XCB_CONFIG_WINDOW_STACK_MODE;
    unsigned int value = XCB_STACK_MODE_ABOVE;

    configure_window(window_id, value_mask, &value);
}

void lower_window(xcb_window_t window_id) {
    unsigned int value_mask = XCB_CONFIG_WINDOW_STACK_MODE;
    unsigned int value = XCB_STACK_MODE_BELOW;

    configure_window(window_id, value_mask, &value);
}

void close_window(xcb_window_t window_id) {
    xcb_get_property_cookie_t protocols_cookie;
    xcb_intern_atom_cookie_t delete_cookie;
    xcb_icccm_get_wm_protocols_reply_t protocols;

    protocols_cookie = xcb_icccm_get_wm_protocols(xcb_connection, window_id,
        ewmh_connection->WM_PROTOCOLS);
    delete_cookie = xcb_intern_atom(xcb_connection, 0, 16, "WM_DELETE_WINDOW");

    xcb_icccm_get_wm_protocols_reply(xcb_connection, protocols_cookie,
        &protocols, NULL);

    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(xcb_connection,
        delete_cookie, NULL);

    if (reply) {
        xcb_atom_t atom;
        unsigned int index = 0;
        xcb_client_message_event_t event = {
            .response_type  = XCB_CLIENT_MESSAGE,
            .format         = 32,
            .sequence       = 0,
            .window         = window_id,
            .type           = ewmh_connection->WM_PROTOCOLS,
            .data.data32    = { 0, XCB_CURRENT_TIME }
        };

        while (index < protocols.atoms_len) {
            atom = protocols.atoms[index++];

            if (reply->atom == atom) {
                event.data.data32[0] = atom;

                xcb_send_event(xcb_connection, 0, window_id,
                    XCB_EVENT_MASK_NO_EVENT, (char*)&event);
                xcb_icccm_get_wm_protocols_reply_wipe(&protocols);
                return;
            }
        }
    }

    xcb_icccm_get_wm_protocols_reply_wipe(&protocols);
    xcb_kill_client(xcb_connection, window_id);
}

void change_window_coordinates(xcb_window_t window_id, unsigned int x,
    unsigned int y) {

    unsigned int value_mask =
        XCB_CONFIG_WINDOW_X |
        XCB_CONFIG_WINDOW_Y;
    unsigned int values[2] = { x, y };

    configure_window(window_id, value_mask, values);
}

void change_window_dimensions(xcb_window_t window_id, unsigned int height,
    unsigned int width) {

    unsigned int value_mask =
        XCB_CONFIG_WINDOW_WIDTH |
        XCB_CONFIG_WINDOW_HEIGHT;
    unsigned int values[2] = { width, height };

    configure_window(window_id, value_mask, values);
}

void change_window_geometry(xcb_window_t window_id, unsigned int x,
    unsigned int y, unsigned int height, unsigned int width) {

    unsigned int value_mask =
        XCB_CONFIG_WINDOW_X     |
        XCB_CONFIG_WINDOW_Y     |
        XCB_CONFIG_WINDOW_WIDTH |
        XCB_CONFIG_WINDOW_HEIGHT;
    unsigned int values[4] = { x, y, width, height };

    configure_window(window_id, value_mask, values);
}

void window_grab_buttons(xcb_window_t window_id, unsigned char buttons,
    unsigned int modifiers) {
    xcb_grab_button(xcb_connection, 0, /*Do not pass pointer events*/
        window_id,
        XCB_EVENT_MASK_BUTTON_PRESS,
        XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
        XCB_NONE, XCB_NONE, buttons, modifiers);
}

void window_release_buttons(xcb_window_t window_id, unsigned char buttons,
    unsigned int modifiers) {
    xcb_ungrab_button(xcb_connection, buttons, window_id, modifiers);
}

void *get_window_property(xcb_window_t window_id, xcb_atom_t property,
    xcb_atom_t atom_type) {
    xcb_get_property_cookie_t atom_cookie;
    xcb_get_property_reply_t *property_cookie;

    atom_cookie = xcb_get_property(xcb_connection, 0, window_id, property,
        atom_type, 0, 256);
    property_cookie = xcb_get_property_reply(xcb_connection, atom_cookie, NULL);

    void *window_property;
    window_property = xcb_get_property_value(property_cookie);

    return window_property;
}

xcb_get_window_attributes_reply_t *get_window_attributes(
    xcb_window_t window_id) {
    xcb_get_window_attributes_cookie_t attributes_cookie;
    xcb_get_window_attributes_reply_t *attributes;

    attributes_cookie = xcb_get_window_attributes(xcb_connection,
        window_id);
    attributes = xcb_get_window_attributes_reply(xcb_connection,
        attributes_cookie, NULL);

    return attributes;
}

char *name_of_window(xcb_window_t window_id) {
    char *name = (char*)get_window_property(window_id, XCB_ATOM_WM_NAME,
        XCB_GET_PROPERTY_TYPE_ANY);

    return name;
}

char *class_of_window(xcb_window_t window_id) {
    char *class = (char*)get_window_property(window_id, XCB_ATOM_WM_CLASS,
        XCB_GET_PROPERTY_TYPE_ANY);

    return class;
}
