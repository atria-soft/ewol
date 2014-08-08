/**
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
 */

#import <Cocoa/Cocoa.h>

@interface EwolMainWindows : NSWindow {
	
}
+ (id)alloc;
- (id)init;
+ ()dealloc;
@end


@interface MyApplication : NSApplication

@property (strong, nonatomic) NSWindow *window;

@end