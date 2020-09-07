#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "socket.h"

#include "../util/logging.h"

int prism_controller(int argc, char **argv) {
    if (argc <= 1)
        return EXIT_FAILURE;

    if (!initialize_socket(CONTROLLER))
        return EXIT_FAILURE;

    // 	private use: U+F0000 - U+FFFFF

    char *buffer = calloc(sizeof(char), 256);
    for (unsigned int index = 1; index < argc;) {
        if (strlen(buffer) + strlen(argv[index]) > 256)
            break;

        strcat(buffer, argv[index++]);
        buffer[strlen(buffer)] = ' ';
    }
    buffer[strlen(buffer)] = '\0';

    write_to_socket(buffer, socket_file_descriptor);

    char *reply = read_from_socket(socket_file_descriptor);
    if (reply) {
        printf("prism: %s\n", reply);
    }

    finalize_socket();

    return EXIT_SUCCESS;
}
