/**
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */


#import <ewol/context/MacOs/Windows.h>
#import <ewol/context/MacOs/OpenglView.h>

#include <ewol/debug.h>

@implementation EwolMainWindows

+ (id)alloc {
	id windowsID = [super alloc];
	EWOL_DEBUG("ALLOCATE ...");
	return windowsID;
}

- (id)init {
	id windowsID = [super init];
	EWOL_DEBUG("INIT ...");
	// set the windows at a specific position :
	[windowsID cascadeTopLeftFromPoint:NSMakePoint(50,50)];
	EWOL_DEBUG("ALLOCATE ...");
	// set the windows resizable
	[windowsID setStyleMask:[windowsID styleMask] | NSResizableWindowMask];
	EWOL_DEBUG("ALLOCATE ...");
	// oposite : [window setStyleMask:[window styleMask] & ~NSResizableWindowMask];
	// set the title
	id appName = [[NSProcessInfo processInfo] processName];
	EWOL_DEBUG("ALLOCATE ...");
	[windowsID setTitle:appName];
	EWOL_DEBUG("ALLOCATE ...");
	
	[windowsID setAcceptsMouseMovedEvents:YES];
	EWOL_DEBUG("ALLOCATE ...");
	// ???
	[windowsID makeKeyAndOrderFront:nil];
	EWOL_DEBUG("ALLOCATE ...");
	[NSApp activateIgnoringOtherApps:YES];
	EWOL_DEBUG("ALLOCATE ...");
	
	NSRect window_frame = [windowsID frame];
	EWOL_DEBUG("ALLOCATE ...");
	
	OpenGLView* view=[[OpenGLView alloc]initWithFrame:window_frame]; //NSMakeRect(0, 0, 800, 600)];
	EWOL_DEBUG("ALLOCATE ...");
	[windowsID setContentView:view];
	EWOL_DEBUG("ALLOCATE ...");
	[view setAutoresizesSubviews:YES];
	EWOL_DEBUG("ALLOCATE ...");
	
	// Override point for customization after application launch.
	//[window addSubview:view];
	//[window addChildWindow:view];
	//[window makeKeyAndVisible];
	
	[windowsID setDelegate:view];
	EWOL_DEBUG("ALLOCATE ...");
	return windowsID;
}


+ (void)dealloc {
	EWOL_DEBUG("FREE ...");
	//[_window release];
	[super dealloc];
}
/*

- (void)sendEvent:(NSEvent *)event {
	EWOL_WARNING(" EVENT ... ");
}
*/

@end




@implementation MyApplication

- (void) applicationDidFinishLaunching: (NSNotification *) note
{
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(100, 100, 100, 100)
												   styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:YES];
	
    self.window = window;
	
    [window close];
	
    [super stop: self];
}

@end


