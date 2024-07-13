#include <ApplicationServices/ApplicationServices.h>
#include <stdio.h>

// Callback function to handle mouse events
CGEventRef mouseEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type == kCGEventLeftMouseDown) {
        CGPoint location = CGEventGetLocation(event);
        printf("Left click at: (%f, %f)\n", location.x, location.y);
        // You can return the location as needed
    }
    return event;
}

int main(void) {
    // Create an event tap to capture mouse events
    CGEventMask eventMask = CGEventMaskBit(kCGEventLeftMouseDown);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        0,
        eventMask,
        mouseEventCallback,
        NULL
    );

    if (!eventTap) {
        fprintf(stderr, "Failed to create event tap\n");
        return 1;
    }

    // Create a run loop source and add it to the current run loop
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    // Enable the event tap
    CGEventTapEnable(eventTap, true);

    // Run the current run loop
    CFRunLoopRun();

    // Clean up
    CFRelease(runLoopSource);
    CFRelease(eventTap);

    return 0;
}

