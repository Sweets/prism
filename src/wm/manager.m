#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "../../axabl/AXUIElement.h"
#import "../../axabl/NSWorkspace.h"

#import "../handlers/application.h"
#import "../handlers/handlers.h"

#import "manager.h"

int prism() {
    /* Initialize axabl backend */
    initialize_nsworkspace();
    if (!initialize_axui())
        return EXIT_FAILURE;
    
    /* Initialize event and signal handlers */
    initialize_handlers();

    /* Allocate and initialize managed object dicts */
    managed_ax_elements = [[NSMutableDictionary alloc] init];

    /* Collect applications running prior to prism */
    for (NSRunningApplication *application in nsworkspace_running_applications())
        application_launched(application, [NSNumber numberWithInteger:[application processIdentifier]]);

    /* Fire and forget */
    CFRunLoopRun();

    return EXIT_SUCCESS;
}