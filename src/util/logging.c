#include <stdio.h>
#include <stdarg.h>

#include "logging.h"

logging_level_t loglevel = NO_LOGGING;

void _log(logging_level_t level, const char *file, const char *function,
    const int line, char *formatting, ...) {

    /*
     * Loglevels:
     * 0 - Absolutely fucking nothing
     * 1 - Fatal
     * 2 - Message
     * 3 - Debug
     */

     if (loglevel < level)
        return;

    fprintf(stderr, "%s:%s L%d: ", file, function, line);

    va_list ap;
    va_start(ap, formatting);
    vfprintf(stderr, formatting, ap);
    va_end(ap);

    fputs("\n", stderr);
}
