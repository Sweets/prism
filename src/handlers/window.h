#import <Foundation/Foundation.h>

typedef void (window_event_handler_t)(CGWindowID, AXUIElementRef);

window_event_handler_t window_created;
window_event_handler_t window_destroyed;
window_event_handler_t window_moved;
window_event_handler_t window_resized;
window_event_handler_t window_focused;
window_event_handler_t window_minimized;
window_event_handler_t window_unminimized;