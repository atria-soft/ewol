/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#import <Cocoa/Cocoa.h>

@interface MacOsAppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
- ()sendEvent:(NSEvent *)event;
- ()applicationWillResignActive:(MacOsAppDelegate *)application;
@end
