#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "../axabl/AXUIElement.h"
#import "../axabl/NSWorkspace.h"

int main(int argc, char **argv) {
    ProcessSerialNumber psn = {
        .lowLongOfPSN = kCurrentProcess
    };
    TransformProcessType(&psn, kProcessTransformToBackgroundApplication);

    if (!initialize_axui())
        return EXIT_FAILURE;
    initialize_nsworkspace();

    CFRunLoopRun();

    finalize_nsworkspace();
    finalize_axui();

    return EXIT_SUCCESS;
}