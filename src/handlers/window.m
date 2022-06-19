#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "window.m"

void window_created(CGWindowID window_identifier, AXUIElementRef ax_element) {
    NSLog(@"new window created");
}

void window_destroyed(CGWindowID window_identifier, AXUIElementRef ax_element) {
    //
}