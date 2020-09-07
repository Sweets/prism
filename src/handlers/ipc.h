#pragma once

#include "../ipc/parsing.h"

extern char *(*ipc_commands[IPC_CMD_NULL])(char**);

char *handle_quit_command(char**);
char *handle_reload_command(char**);
char *handle_tile_command(char**);
char *handle_map_command(char**);
char *handle_unmap_command(char**);
