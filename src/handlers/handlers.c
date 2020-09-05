#include <stdlib.h>
#include <unistd.h>

#include <xcb/xcb.h>

#include "ipc.h"
#include "signal.h"
#include "xcb.h"

#include "../ipc/socket.h"
#include "../xcb/connection.h"

void handle_xcb_events() {
    xcb_generic_event_t *xcb_event;
    unsigned int xcb_event_type;

    while ((xcb_event = xcb_poll_for_event(xcb_connection))) {
        xcb_event_type = xcb_event->response_type & ~0x80;

        if (xcb_events[xcb_event_type])
            xcb_events[xcb_event_type](xcb_event);

        free(xcb_event);
    }
}

void handle_ipc_input() {
    int file_descriptor = accept_connection();
    if (file_descriptor < 0)
        return;

    char *input = read_from_socket(file_descriptor);
    char **args = ipc_split_input(input);
    ipc_command_t command = ipc_get_command(args[0]);

    char *reply;
    if (ipc_commands[command]) {
        reply = ipc_commands[command](args);
        if (reply)
            write_to_socket(reply, file_descriptor);
    }
    close(file_descriptor);

/*    unsigned int index = 0;
    while (args[index])
        free(args[index++]);*/

    free(input);
}

void handle_signals(int signal) {
    if (signals[signal])
        signals[signal]();
}
