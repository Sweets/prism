#define SIGUNUSED 32

struct sigaction signal_handler;
void (*signal_handlers[SIGUNUSED])(void);

void handle_termination_signal(void);
void handle_user_defined_signal(void);