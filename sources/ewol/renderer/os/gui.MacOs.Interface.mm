

/*

#import <Cocoa/Cocoa.h>
#import <ewol/renderer/os/gui.MacOs.Interface.h>
#include <ewol/renderer/os/gui.MacOs.AppDelegate.h>

int mm_main(int argc, const char *argv[])
{
	return NSApplicationMain(argc, (const char **)argv);
}
*/



#import <Cocoa/Cocoa.h>
#include "ewol/renderer/os/gui.MacOs.Interface.h"

#import <ewol/renderer/os/gui.MacOs.OpenglView.h>

int mm_main(int argc, const char *argv[])
{
    [NSAutoreleasePool new];
    
    [NSApplication sharedApplication];
    // set the quit policy and all stadard for Mac
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    // ---------------------------------------------------------------
    // -- basic menu bar creation :
    // ---------------------------------------------------------------
    // set the basic menu bar (stadard on Mac OSX)
    id menubar = [[NSMenu new] autorelease];
    	//add an item
    	id appMenuItem = [[NSMenuItem new] autorelease];
    	// add the item to the menu bar: 
    	[menubar addItem:appMenuItem];
    // set the main menu in the menu bar ...
    [NSApp setMainMenu:menubar];
	
    id appMenu = [[NSMenu new] autorelease];
    id appName = [[NSProcessInfo processInfo] processName];
    // create the label to qui the application :
    id quitTitle = [@"Quit " stringByAppendingString:appName];
    // create the item to quit the appllication with META+q at shortCut
    id quitMenuItem = [ [ [NSMenuItem alloc] initWithTitle:quitTitle
                          action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
    // add the item to the menu:
    [appMenu addItem:quitMenuItem];
    // set the application menu to the main app menu ...
    [appMenuItem setSubmenu:appMenu];
    
    // ---------------------------------------------------------------
    // -- basic windows creation :
    // ---------------------------------------------------------------
    // create a windows of size 800/600
    id window = [ [ [NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 800, 600)
                    styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO]
                   autorelease];
	// set the windows at a specific position :
    [window cascadeTopLeftFromPoint:NSMakePoint(50,50)];
    // set the title
    [window setTitle:appName];
    // ???
    [window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
    
    NSRect window_frame = [window frame];
    
    OpenGLView* view=[[OpenGLView alloc]initWithFrame:window_frame]; //NSMakeRect(0, 0, 800, 600)];
    [window setContentView:view];
	[view setAutoresizesSubviews:YES];
    
    // Override point for customization after application launch.
    //[window addSubview:view];
    //[window addChildWindow:view];
    //[window makeKeyAndVisible];
    
    
    // start application :
    [NSApp run];
    // return no error
    return 0;
} 
 
 
 
 /*
 
NSView* view0 = ...; // a view made with IB 
NSView* view1 = ... ;// another view made with IB
NSWindow* window = [self window];

NSRect window_frame = [window frame];

NSView* cv = [[[NSView alloc] initWithFrame:window_frame] autorelease];
[window setContentView:cv];
[cv setAutoresizesSubviews:YES];

// add subview so it fits within the contentview frame
{
    NSView* v = view0;
    NSRect vframe = [v frame];
    [v setHidden:YES];
    [v setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

    NSView* tmp_superview = [[[NSView alloc] initWithFrame:vframe] autorelease];
    [tmp_superview addSubview:v];
    [tmp_superview setAutoresizesSubviews:YES];
    [tmp_superview setFrame:window_frame];

    [v removeFromSuperview];
       [cv addSubview:v];
}

// add subview so it fits within the contentview frame
{
    NSView* v = view1;
    NSRect vframe = [v frame];
    [v setHidden:YES];
    [v setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

    NSView* tmp_superview = [[[NSView alloc] initWithFrame:vframe] autorelease];
    [tmp_superview addSubview:v];
    [tmp_superview setAutoresizesSubviews:YES];
    [tmp_superview setFrame:window_frame];

    [v removeFromSuperview];
    [cv addSubview:v];
}

[view0 setHidden:NO];

*/
