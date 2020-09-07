#include "group.h"
#include "manager.h"
#include "tile.h"
#include "window.h"

#include "../xcb/window.h"

split_mode_t splitting_mode = SOUTHERN_SPLIT;

void split_current_view_port(split_mode_t splitting_mode, group_t *group,
    window_t *originating_window, window_t *introducing_window) {

    float height = originating_window->height / 2.0;
    float width = originating_window->width / 2.0;

    switch (splitting_mode) {
        case NORTHERN_SPLIT:
            introducing_window->x = originating_window->x;
            introducing_window->y = originating_window->y;
            originating_window->y += height;

            originating_window->height = introducing_window->height = height;
            introducing_window->width  = originating_window->width;
            break;
        case SOUTHERN_SPLIT:
            introducing_window->x = originating_window->x;
            introducing_window->y = originating_window->y + height;

            originating_window->height = introducing_window->height = height;
            introducing_window->width  = originating_window->width;
            break;
        case WESTERN_SPLIT:
            introducing_window->x = originating_window->x;
            originating_window->x += width;
            introducing_window->y = originating_window->y;

            introducing_window->height = originating_window->height;
            introducing_window->width  = originating_window->width = width;
            break;
        case EASTERN_SPLIT:
            introducing_window->x = originating_window->x + width;
            introducing_window->y = originating_window->y;

            introducing_window->height = originating_window->height;
            introducing_window->width  = originating_window->width = width;
            break;
    }

    originating_window->tile_child  = introducing_window->id;
    introducing_window->tile_parent = originating_window->id;

    change_managed_window_geometry(originating_window,
        originating_window->x + group->x, originating_window->y + group->y,
        originating_window->height,       originating_window->width);

    change_managed_window_geometry(introducing_window,
        introducing_window->x + group->x, introducing_window->y + group->y,
        introducing_window->height,       introducing_window->width);
}

/* Space reclaiming */

void claim_freed_space(window_t *window, group_t *group) {
    window_t *parent = window_from_id(window->tile_parent);
    window_t *child  = window_from_id(window->tile_child);

    tile_children_recursive_real_estate_claim(window, group);

    if (parent) {
        parent->tile_child = window->tile_child;
    }

    if (child)
        child->tile_parent = window->tile_parent;
}

void tile_children_recursive_real_estate_claim(window_t *window,
    group_t *group) {
    /* Good luck trying to follow this cognitively.
     * Your brain might melt. Mine certainly did while programming this.
     */
    window_t *parent_window = window_from_id(window->tile_parent);
    window_t *child_window = window_from_id(window->tile_child);

    if (child_window) {
        if (!child_window->tile_child) {
            if (!parent_window) {
                child_window->x = child_window->y = 0;
                child_window->height = group->height;
                child_window->width  = group->width;
            } else {
                if (window->x < child_window->x)
                    child_window->x = window->x;

                if (window->y < child_window->y)
                    child_window->y = window->y;

                if (child_window->height + window->height <=
                    parent_window->height)
                    child_window->height += window->height;

                if (child_window->width  + window->width  <=
                    parent_window->width)
                    child_window->width += window->width;
            }
        } else {
            // Recurse to lowest depth
            tile_children_recursive_real_estate_claim(child_window, group);

            child_window->x = window->x;
            child_window->y = window->y;
            child_window->height = window->height;
            child_window->width  = window->width;
        }

        change_managed_window_geometry(child_window,
            child_window->x + group->x,      child_window->y + group->y,
            child_window->height, child_window->width);
    } else {
        if (!parent_window)
            return;

        child_window = window;
        window = parent_window;

        if (!window->tile_parent) {
            window->x = window->y = 0;
            window->height = group->height;
            window->width = group->width;

            change_managed_window_geometry(window,
                window->x + group->x, window->y + group->y,
                window->height, window->width);
            return;
        }

        parent_window = window_from_id(window->tile_parent);

        if (child_window->x < window->x)
            window->x = child_window->x;

        if (child_window->y < window->y)
            window->y = child_window->y;

        if (child_window->height + window->height <=
            parent_window->height)
            window->height += child_window->height;

        if (child_window->width  + window->width  <=
            parent_window->width)
            window->width += child_window->width;

        change_managed_window_geometry(window,
            window->x + group->x, window->y + group->y,
            window->height, window->width);
    }
}
