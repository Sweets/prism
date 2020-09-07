#pragma once

#include "group.h"
#include "window.h"

typedef enum {
    NORTHERN_SPLIT,
    SOUTHERN_SPLIT,
    EASTERN_SPLIT,
    WESTERN_SPLIT
} split_mode_t;

extern split_mode_t splitting_mode;

void split_current_view_port(split_mode_t, group_t*, window_t*, window_t*);
void claim_freed_space(window_t*, group_t*);
void tile_children_recursive_real_estate_claim(window_t*, group_t*);
