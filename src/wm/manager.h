#pragma once

#include <xcb/xcb.h> // REMOVE

#include "group.h"

#include "../util/vector.h"

extern unsigned char window_manager_is_active;

extern window_t *focused_window; // REMOVE
extern group_t *focused_group;

extern vector_t *managed_windows;
extern vector_t *groups;

unsigned int initialize_prism(void);
void finalize_prism(void);
