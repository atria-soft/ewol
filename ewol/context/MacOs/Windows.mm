/**
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
 */


#import <ewol/context/MacOs/Windows.h>
#import <ewol/context/MacOs/OpenglView.h>
#include <ewol/context/MacOS/Context.h>
#include <ewol/key/key.h>

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
	
	//[windowsID setDelegate:view];
	EWOL_DEBUG("ALLOCATE ...");
	return windowsID;
}


+ (void)dealloc {
	EWOL_DEBUG("FREE ...");
	//[_window release];
	[super dealloc];
}

static ewol::key::Special guiKeyBoardMode;


static int32_t getUniChar(NSEvent* theEvent) {
	NSString *str = [theEvent charactersIgnoringModifiers];
	// TODO : set if for every char in the string !!!
	unichar c = [str characterAtIndex:0];
	if (guiKeyBoardMode.getAlt() == false) {
		return int32_t(c);
	}
	switch(c) {
		case '(':
			return int32_t('{');
		case '5':
			return int32_t('[');
		case ')':
			return int32_t('}');
		case U'°':
			return int32_t(']');
			
	}
	return int32_t(c);
}

- (void)keyDown:(NSEvent *)theEvent {
	bool thisIsAReapeateKey = false;
	if ([theEvent isARepeat]) {
		thisIsAReapeateKey = true;
	}
	int32_t c = getUniChar(theEvent);
	EWOL_WARNING("KeyDown " << char(c));
	MacOs::setKeyboard(guiKeyBoardMode, c, true, thisIsAReapeateKey);
	if (true==thisIsAReapeateKey) {
		MacOs::setKeyboard(guiKeyBoardMode, c, false, thisIsAReapeateKey);
	}
}

- (void)keyUp:(NSEvent *)theEvent {
	bool thisIsAReapeateKey = false;
	if ([theEvent isARepeat]) {
		thisIsAReapeateKey = true;
	}
	int32_t c = getUniChar(theEvent);
	EWOL_WARNING("KeyUp " << char(c));
	MacOs::setKeyboard(guiKeyBoardMode, c, false, thisIsAReapeateKey);
	if (true==thisIsAReapeateKey) {
		MacOs::setKeyboard(guiKeyBoardMode, c, true, thisIsAReapeateKey);
	}
}

- (void)flagsChanged:(NSEvent *)theEvent {
	if (([theEvent modifierFlags] & NSAlphaShiftKeyMask) != 0) {
		EWOL_WARNING("NSAlphaShiftKeyMask");
		if (guiKeyBoardMode.getCapsLock() == false) {
			guiKeyBoardMode.setCapsLock(true);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardCapLock, true);
		}
	} else {
		if (guiKeyBoardMode.getCapsLock() == true) {
			guiKeyBoardMode.setCapsLock(false);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardCapLock, false);
		}
	}
	
	if (([theEvent modifierFlags] & NSShiftKeyMask) != 0) {
		EWOL_WARNING("NSShiftKeyMask");
		if (guiKeyBoardMode.getShift() == false) {
			guiKeyBoardMode.setShift(true);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardShiftLeft, true);
		}
	} else {
		if (guiKeyBoardMode.getShift() == true) {
			guiKeyBoardMode.setShift(false);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardShiftLeft, false);
		}
	}
	
	if (([theEvent modifierFlags] & NSControlKeyMask) != 0) {
		EWOL_WARNING("NSControlKeyMask");
		if (guiKeyBoardMode.getCtrl() == false) {
			guiKeyBoardMode.setCtrl(true);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardCtrlLeft, true);
		}
	} else {
		if (guiKeyBoardMode.getCtrl() == true) {
			guiKeyBoardMode.setCtrl(false);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardCtrlLeft, false);
		}
	}
	
	if (([theEvent modifierFlags] & NSAlternateKeyMask) != 0) {
		EWOL_WARNING("NSAlternateKeyMask");
		if (guiKeyBoardMode.getAlt() == false) {
			guiKeyBoardMode.setAlt(true);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardAlt, true);
		}
	} else {
		if (guiKeyBoardMode.getAlt() == true) {
			guiKeyBoardMode.setAlt(false);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardAlt, false);
		}
	}
	
	if (([theEvent modifierFlags] & NSCommandKeyMask) != 0) {
		EWOL_WARNING("NSCommandKeyMask");
		if (guiKeyBoardMode.getMeta() == false) {
			guiKeyBoardMode.setMeta(true);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardMetaLeft, true);
		}
	} else {
		if (guiKeyBoardMode.getMeta() == true) {
			guiKeyBoardMode.setMeta(false);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardMetaLeft, false);
		}
	}
	
	if (([theEvent modifierFlags] & NSNumericPadKeyMask) != 0) {
		EWOL_WARNING("NSNumericPadKeyMask");
		if (guiKeyBoardMode.getNumLock() == false) {
			guiKeyBoardMode.setNumLock(true);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardNumLock, true);
		}
	} else {
		if (guiKeyBoardMode.getNumLock() == true) {
			guiKeyBoardMode.setNumLock(false);
			MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardNumLock, false);
		}
	}
	if (([theEvent modifierFlags] & NSHelpKeyMask) != 0) {
		EWOL_WARNING("NSHelpKeyMask");
	}
	if (([theEvent modifierFlags] & NSFunctionKeyMask) != 0) {
		EWOL_WARNING("NSFunctionKeyMask");
		MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardContextMenu, true);
		MacOs::setKeyboardMove(guiKeyBoardMode, ewol::key::keyboardContextMenu, false);
	}
	EWOL_WARNING("EVENT : " << int32_t([theEvent modifierFlags]));
}

// this generate all the event entry availlable ==> like a big keep focus ...
- (BOOL)acceptsFirstResponder {
	return YES;
}
- (BOOL)becomeFirstResponder {
	return YES;
}

-(void)mouseMoved:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("mouseMoved : " << (float)point.x << " " << (float)point.y);
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


