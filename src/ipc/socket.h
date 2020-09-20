#pragma once

extern int socket_file_descriptor;

typedef enum {
    WINDOW_MANAGER_IPC,
    CONTROLLER
} ipc_mode_t;

char *socket_path;
ipc_mode_t ipc_mode;

unsigned short initialize_socket(ipc_mode_t);
int accept_connection(void);
void write_to_socket(char*, int);
char *read_from_socket(int);
void finalize_socket(void);
