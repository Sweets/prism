#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

NSDictionary *application_event_handlers;
NSDictionary *window_event_handlers;

void initialize_handlers(void);

void generic_signal_handler(int);
void (^generic_notification_handler)(NSNotification*);
void generic_observer_callback(AXObserverRef, AXUIElementRef, CFStringRef, void*);