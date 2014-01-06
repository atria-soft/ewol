/**
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */

#import <Cocoa/Cocoa.h>

@interface EwolMainWindows : NSWindow {
	
}
+ (id)alloc;
- (id)init;
+ (void)dealloc;
@end


@interface MyApplication : NSApplication

@property (strong, nonatomic) NSWindow *window;

@end