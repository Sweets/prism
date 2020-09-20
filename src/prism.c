#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "ipc/controller.h"
#include "ipc/socket.h"
#include "handlers/handlers.h"
#include "util/logging.h"
#include "wm/manager.h"
#include "xcb/connection.h"

int main(int argc, char **argv) {
    char argument;
    int unprotected_loglevel;

    while ((argument = getopt(argc, argv, "hrd:")) >= 0) {
        switch (argument) {
            case 'd':
                unprotected_loglevel = atoi(optarg);
                if (unprotected_loglevel > DEBUG_LOGGING ||
                    unprotected_loglevel < NO_LOGGING)
                    loglevel = FATAL_LOGGING;
                else
                    loglevel = unprotected_loglevel;
                break;
            case 'h':
                // show help dialog
                printf("%s\n",
                    "prism ...\n"
                    "-h\tHelp dialog\n"
                    "-d N\tSets log level\n"
                    "-r\tRequests an action or information from prism");
                return EXIT_SUCCESS;
                break;
            case 'r':
                return prism_controller(argc, argv);
                break;
            default:
                break;
        }
    }

    if (!initialize_prism()) {
        log_fatal("Unable to initialize prism.");
        return EXIT_FAILURE;
    }

    struct pollfd file_descriptors[] = {
        { xcb_file_descriptor,    .events = POLLIN },
        { socket_file_descriptor, .events = POLLIN }
    };

    while (window_manager_is_active && poll(file_descriptors, 2, -1)) {
        if (file_descriptors[0].revents & POLLIN)
            handle_xcb_events();

        if (file_descriptors[1].revents & POLLIN)
            handle_ipc_input();
    }

    finalize_prism();

    return EXIT_SUCCESS;
}
