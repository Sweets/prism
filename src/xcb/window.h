#pragma once

#include <xcb/xcb.h>

void configure_window(xcb_window_t, unsigned int, unsigned int*);

void map_window(xcb_window_t);
void unmap_window(xcb_window_t);

void focus_window(xcb_window_t);

void raise_window(xcb_window_t);
void lower_window(xcb_window_t);

void close_window(xcb_window_t);

void change_window_coordinates(xcb_window_t, unsigned int, unsigned int);
void change_window_dimensions(xcb_window_t, unsigned int, unsigned int);
void change_window_geometry(xcb_window_t, unsigned int, unsigned int,
    unsigned int, unsigned int);

void window_grab_buttons(xcb_window_t, unsigned char, unsigned int);
void window_release_buttons(xcb_window_t, unsigned char, unsigned int);

void *get_window_property(xcb_window_t, xcb_atom_t, xcb_atom_t);
xcb_get_window_attributes_reply_t *get_window_attributes(xcb_window_t);
char *name_of_window(xcb_window_t);
char *class_of_window(xcb_window_t);
