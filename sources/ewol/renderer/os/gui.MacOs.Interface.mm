


#import <Cocoa/Cocoa.h>
	#include "ewol/renderer/os/gui.MacOs.Interface.h"


int mm_main(int argc, const char *argv[])
//int main(int argc, const char *argv[])
{
    [NSAutoreleasePool new];
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    id menubar = [[NSMenu new] autorelease];
    id appMenuItem = [[NSMenuItem new] autorelease];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    id appMenu = [[NSMenu new] autorelease];
    id appName = [[NSProcessInfo processInfo] processName];
    id quitTitle = [@"Quit " stringByAppendingString:appName];
    id quitMenuItem = [ [ [NSMenuItem alloc] initWithTitle:quitTitle
                          action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];
    id window = [ [ [NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 340, 480)
                    styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO]
                   autorelease];
    [window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [window setTitle:appName];
    [window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp run];
    return 0;
} 
 
// gcc -framework Cocoa -x objective-c -o testApplDirectMinimal sources/ewol/renderer/os/gui.MacOs.Interface.mm
  