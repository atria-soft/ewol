/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#import <ewol/renderer/MacOs/OpenglView.h>
#include <OpenGL/gl.h>
#include <ewol/renderer/MacOS/Context.h>
#include <ewol/debug.h>
#include <ewol/Dimension.h>

@implementation OpenGLView

static ewol::SpecialKey guiKeyBoardMode;

- (void) prepareOpenGL
{
	EWOL_ERROR("prepare");
	GLint swapInt = 1;
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; // set to vbl sync
	
	// set system dpi size : 
	NSScreen *screen = [NSScreen mainScreen];
	NSDictionary *description = [screen deviceDescription];
	NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
	CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);
	
	ewol::dimension::setPixelRatio(vec2((float)displayPixelSize.width/(float)displayPhysicalSize.width,
	                                    (float)displayPixelSize.height/(float)displayPhysicalSize.height),
	                                    ewol::Dimension::Millimeter);
}

-(void) drawRect: (NSRect) bounds
{
	MacOs::draw(true);
	glFlush();
}

-(void)reshape {
	EWOL_INFO("view reshape (" << [self frame].size.width << "," << [self frame].size.height << ")" );
	
	// window resize; width and height are in pixel coordinates
	// but they are floats
	float width = [self frame].size.width;
	float height = [self frame].size.height;
	MacOs::resize(width,height);
}

-(void)mouseDown:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//float x = [event locationInWindow].x; //point.x;
	//EWOL_INFO("mouseDown : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseState(1, true, point.x, point.y);
}
-(void)mouseDragged:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("mouseDragged : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseMotion(1, point.x, point.y);
}
-(void)mouseUp:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("mouseUp : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseState(1, false, point.x, point.y);
}
-(void)mouseMoved:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("mouseMoved : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseMotion(0, point.x, point.y);
}
-(void)mouseEntered:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("mouseEntered : " << (float)point.x << " " << (float)point.y);
}
-(void)mouseExited:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("mouseExited : " << (float)point.x << " " << (float)point.y);
}
-(void)rightMouseDown:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("rightMouseDown : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseState(3, true, point.x, point.y);
}
-(void)rightMouseDragged:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("rightMouseDragged : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseMotion(3, point.x, point.y);
}
-(void)rightMouseUp:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("rightMouseUp : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseState(3, false, point.x, point.y);
}
-(void)otherMouseDown:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	int32_t btNumber = [event buttonNumber];
	switch (btNumber) {
		case 2: // 2 : Middle button
			btNumber = 2;
			break;
		case 3: // 3 : border button DOWN
			btNumber = 8;
			break;
		case 4: // 4 : border button UP
			btNumber = 9;
			break;
		case 5: // 5 : horizontal scroll Right to left
			btNumber = 11;
			break;
		case 6: // 6 : horizontal scroll left to Right
			btNumber = 10;
			break;
		case 7: // 7 : Red button
			btNumber = 12;
			break;
		default:
			btNumber = 15;
			break;
	}
	//EWOL_INFO("otherMouseDown : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseState(btNumber, true, point.x, point.y);
}
-(void)otherMouseDragged:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	int32_t btNumber = [event buttonNumber];
	switch (btNumber) {
		case 2: // 2 : Middle button
			btNumber = 2;
			break;
		case 3: // 3 : border button DOWN
			btNumber = 8;
			break;
		case 4: // 4 : border button UP
			btNumber = 9;
			break;
		case 5: // 5 : horizontal scroll Right to left
			btNumber = 11;
			break;
		case 6: // 6 : horizontal scroll left to Right
			btNumber = 10;
			break;
		case 7: // 7 : Red button
			btNumber = 12;
			break;
		default:
			btNumber = 15;
			break;
	}
	//EWOL_INFO("otherMouseDragged : " << (float)point.x << " " << (float)point.y);
	MacOs::setMouseMotion(btNumber, point.x, point.y);
}
-(void)otherMouseUp:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	int32_t btNumber = [event buttonNumber];
	switch (btNumber) {
		case 2: // 2 : Middle button
			btNumber = 2;
			break;
		case 3: // 3 : border button DOWN
			btNumber = 8;
			break;
		case 4: // 4 : border button UP
			btNumber = 9;
			break;
		case 5: // 5 : horizontal scroll Right to left
			btNumber = 11;
			break;
		case 6: // 6 : horizontal scroll left to Right
			btNumber = 10;
			break;
		case 7: // 7 : Red button
			btNumber = 12;
			break;
		default:
			btNumber = 15;
			break;
	}
	//EWOL_INFO("otherMouseUp : " << (float)point.x << " " << (float)point.y << " bt id=" << btNumber );
	MacOs::setMouseState(btNumber, false, point.x, point.y);
}
- (void)scrollWheel:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//EWOL_INFO("scrollWheel : " << (float)point.x << " " << (float)point.y << "button : " << (int32_t)([event buttonNumber]));
	EWOL_INFO("scrollWheel : " << (float)point.x << " " << (float)point.y << " delta(" << (float)([event deltaX]) << "," << (float)([event deltaY]) << ")");
	float deltaY = [event deltaY];
	int32_t idEvent = 4;
	if (deltaY < 0) {
		idEvent = 5;
	}
	if (abs(deltaY) < 0.1f) {
		return;
	}
	for (float iii=abs(deltaY) ; iii>=0.0f ; iii-=1.0f) {
		MacOs::setMouseState(idEvent, true , point.x, point.y);
		MacOs::setMouseState(idEvent, false, point.x, point.y);
	}
}
/*
// http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/EventOverview/EventArchitecture/EventArchitecture.html
-(void)sendEvent:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("sendEvent : " << (float)point.x << " " << (float)point.y);
}
*/

// this generate all the event entry availlable ==> like a big keep focus ...
- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)keyDown:(NSEvent *)theEvent {
	bool thisIsAReapeateKey = false;
	if ([theEvent isARepeat]) {
		thisIsAReapeateKey = true;
	}
	NSString *str = [theEvent charactersIgnoringModifiers];
	// TODO : set if for every char in the string !!!
	unichar c = [str characterAtIndex:0];
	//EWOL_DEBUG("KeyDown " << (char)c);
	MacOs::setKeyboard(guiKeyBoardMode, (char)c, true, thisIsAReapeateKey);
	if (true==thisIsAReapeateKey) {
		MacOs::setKeyboard(guiKeyBoardMode, (char)c, false, thisIsAReapeateKey);
	}
}

- (void)keyUp:(NSEvent *)theEvent {
	bool thisIsAReapeateKey = false;
	if ([theEvent isARepeat]) {
		thisIsAReapeateKey = true;
	}
	//EWOL_DEBUG("KeyUp ");
	NSString *str = [theEvent charactersIgnoringModifiers];
	unichar c = [str characterAtIndex:0];
	MacOs::setKeyboard(guiKeyBoardMode, (char)c, false, thisIsAReapeateKey);
	if (true==thisIsAReapeateKey) {
		MacOs::setKeyboard(guiKeyBoardMode, (char)c, true, thisIsAReapeateKey);
	}
}

#define FRAME_INTERVAL (0.01)

static NSTimer *timer = nil;

- (void)windowDidResignMain:(NSNotification *)notification {
//	NSLog(@"window did resign main");
	[timer invalidate];
	
	//game_deactivate(); // freeze, pause
	[self setNeedsDisplay:YES];
}

- (void)windowDidBecomeMain:(NSNotification *)notification {
//	NSLog(@"window did become main");
	
	//game_activate();
	[self setNeedsDisplay:YES];
	
	timer = [NSTimer timerWithTimeInterval:FRAME_INTERVAL 
	                 target:self 
	                 selector:@selector(timerEvent:) 
	                 userInfo:nil 
	                 repeats:YES];
	
	[[NSRunLoop mainRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
}

- (void)timerEvent:(NSTimer *)t {
	//run_game();
	[self setNeedsDisplay:YES];
}
@end
