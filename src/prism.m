#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "wm/manager.h"

int main(int argc, char **argv) {
    /* Setup process */
    ProcessSerialNumber psn = {
        .lowLongOfPSN = kCurrentProcess
    };
    TransformProcessType(&psn, kProcessTransformToBackgroundApplication);

    return prism();
}