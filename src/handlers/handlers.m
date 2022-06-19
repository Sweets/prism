#import <Foundation/Foundation.h>

#import "../../axabl/AXUIElement.h"
#import "../../axabl/NSWorkspace.h"

#import "application.h"
#import "handlers.h"
#import "signal.h"
#import "window.h"

void initialize_handlers() {
    /* Signal handler */
    sigemptyset(&signal_handler.sa_mask);

    for (int index = 0; index < SIGUNUSED; index++)
        if (signal_handlers[index])
            sigaction(index, &signal_handler, NULL);
    
    /* Application event handlers */

    application_event_handlers = @{
        NSWorkspaceDidLaunchApplicationNotification:
            [NSValue valueWithPointer: &application_launched],
        NSWorkspaceDidTerminateApplicationNotification: 
            [NSValue valueWithPointer: &application_terminated],
        NSWorkspaceDidActivateApplicationNotification:
            [NSValue valueWithPointer: &application_activated],
/*        NSWorkspaceDidDeactivateApplicationNotification:
            [NSValue valueWithPointer: &application_deactivated]*/
    };

    for (NSNotificationName notification in [application_event_handlers allKeys])
        nsworkspace_install_observer(notification, generic_notification_handler);

    /* Window event handlers */

    window_event_handlers = @{
        (NSString *)kAXWindowCreatedNotification:
            [NSValue valueWithPointer: &window_created],
        (NSString *)kAXUIElementDestroyedNotification:
            [NSValue valueWithPointer: &window_destroyed]
    };
}

/* Generic handlers */

void generic_signal_handler(int signal) {
    if (signal_handlers[signal])
        signal_handlers[signal]();
}

void (^generic_notification_handler)(NSNotification*) = ^(NSNotification *notification) {
    NSNotificationName notification_name = [notification name];

    NSValue *event_handler = application_event_handlers[notification_name];
    if (!event_handler)
        return;
        
    application_event_handler_t *handler_ptr = [event_handler pointerValue];

    NSRunningApplication *application =
        [[notification userInfo] valueForKey:@"NSWorkspaceApplicationKey"];
    pid_t process_identifier = [application processIdentifier];
    id pid = [NSNumber numberWithInteger:process_identifier];

    handler_ptr(application, pid);
};

void generic_observer_callback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void *data) {
    NSValue *event_handler = window_event_handlers[(NSString*)notification];
    if (!event_handler)
        return;
    
    window_event_handler_t *handler_ptr = [event_handler pointerValue];

    if (!handler_ptr)
        return;

    CGWindowID window_identifier = axui_get_window_id(element);
    handler_ptr(window_identifier, element);
}