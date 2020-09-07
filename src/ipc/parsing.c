#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"

ipc_command_t ipc_get_command(char *command) {
    if (!strcmp(command, "quit"))
        return IPC_CMD_QUIT;
    else if (!strcmp(command, "tile"))
        return IPC_CMD_SET_TILE_DIRECTION;
    else if (!strcmp(command, "reload"))
        return IPC_CMD_RELOAD;
    else if (!strcmp(command, "map"))
        return IPC_CMD_MAP;
    else if (!strcmp(command, "unmap"))
        return IPC_CMD_UNMAP;

    return IPC_CMD_NULL;
}

char **ipc_split_input(char *buffer) {
    char **argv = calloc(1, sizeof(char*));
    unsigned int argc  = 1;
    unsigned int index = 0;

    char *token;
    while ((token = strsep(&buffer, " "))) {
        if (index + 1 > argc) {
            argc++;
            argv = realloc(argv, sizeof(char*) * argc);
        }

        argv[index] = token;
        index++;
    }

    return argv;
}
