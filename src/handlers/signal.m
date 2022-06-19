#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "../../axabl/AXUIElement.h"
#import "../../axabl/NSWorkspace.h"

#import "handlers.h"
#import "signal.h"

struct sigaction signal_handler = {
    .sa_handler = generic_signal_handler
};

void (*signal_handlers[SIGUNUSED])(void) = {
    [SIGINT]  = handle_termination_signal,
    [SIGTERM] = handle_termination_signal,
    [SIGQUIT] = handle_termination_signal,
    [SIGUSR1] = handle_user_defined_signal,
    [SIGUSR2] = handle_user_defined_signal,
};


void handle_termination_signal(void) {
    CFRunLoopStop(CFRunLoopGetMain());

    finalize_axui();
    finalize_nsworkspace();
}

void handle_user_defined_signal(void) {
    NSLog(@"received USR sig");
}