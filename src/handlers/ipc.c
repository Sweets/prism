#include "ipc.h"

#include "../wm/manager.h"
#include "../wm/tile.h"

char *(*ipc_commands[IPC_CMD_NULL])(char**) = {
    [IPC_CMD_QUIT]               = handle_quit_command,
    [IPC_CMD_RELOAD]             = handle_reload_command,
    [IPC_CMD_SET_TILE_DIRECTION] = handle_tile_command,
};

char *handle_quit_command(char **args) {
    (void)args;

    window_manager_is_active = 0;
    return NULL;
}

char *handle_reload_command(char **args) {
    (void)args;

    //

    return NULL;
}

char *handle_tile_command(char **args) {
    /* prism tile_direction [up|down|left|right] */

    if (!args[1])
        return "tile_direction: No direction provided.";

    if (!strcmp(args[1], "up")) {
        splitting_mode = NORTHERN_SPLIT;
    } else if (!strcmp(args[1], "down")) {
        splitting_mode = SOUTHERN_SPLIT;
    } else if (!strcmp(args[1], "left")) {
        splitting_mode = WESTERN_SPLIT;
    } else if (!strcmp(args[1], "right")) {
        splitting_mode = EASTERN_SPLIT;
    } else {
        return "tile_direction: Invalid direction specified.";
    }

    return NULL;
}
