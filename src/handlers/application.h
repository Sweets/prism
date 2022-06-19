#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

typedef void (application_event_handler_t)(NSRunningApplication*, id);

application_event_handler_t application_launched;
application_event_handler_t application_terminated;
application_event_handler_t application_activated;
application_event_handler_t application_deactivated;