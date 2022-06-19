#import <Foundation/Foundation.h>

#import "application.h"

void application_launched(NSRunningApplication *application, id process_identifier) {
    NSLog(@"app launched");
}

void application_terminated(NSRunningApplication *application, id process_identifier) {
    NSLog(@"app fucking murked.");
}

void application_activated(NSRunningApplication *application, id process_identifier) {
    NSLog(@"app activated");
}