#pragma once

typedef enum {
    IPC_CMD_QUIT,
    IPC_CMD_RELOAD,
    IPC_CMD_SET_TILE_DIRECTION,
    IPC_CMD_MAP,
    IPC_CMD_UNMAP,

    IPC_CMD_NULL = 256
} ipc_command_t;

ipc_command_t ipc_get_command(char*);
char **ipc_split_input(char*);
