#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "../../axabl/AXUIElement.h"

#import "../wm/manager.h"

#import "application.h"
#import "handlers.h"

void application_launched(NSRunningApplication *application, id process_identifier) {
    AXUIElementRef ax_element = axui_create_application_element(process_identifier);
    id encoded_ax_element = axui_encode_element(ax_element);

    for (NSString *notification in window_event_handlers)
        axui_install_observer(ax_element, (CFStringRef)notification, generic_observer_callback);

    [managed_ax_elements setObject:encoded_ax_element forKey:process_identifier];
}

void application_terminated(NSRunningApplication *application, id process_identifier) {
    NSLog(@"app fucking murked.");
}

void application_activated(NSRunningApplication *application, id process_identifier) {
    NSLog(@"app activated");
}