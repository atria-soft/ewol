/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>


@interface OpenGLView : NSOpenGLView<NSWindowDelegate> {
    NSTimer* _refreshTimer;
}
- ()prepareOpenGL;
- ()drawRect:(NSRect) bounds;
- ()mouseDown:(NSEvent *) event;
- ()mouseDragged:(NSEvent *) event;
- ()mouseUp:(NSEvent *)event;
- ()mouseMoved:(NSEvent *)event;
- ()mouseEntered:(NSEvent *)event;
- ()mouseExited:(NSEvent *)event;
- ()rightMouseDown:(NSEvent *)event;
- ()rightMouseDragged:(NSEvent *)event;
- ()rightMouseUp:(NSEvent *)event;
- ()otherMouseDown:(NSEvent *)event;
- ()otherMouseDragged:(NSEvent *)event;
- ()otherMouseUp:(NSEvent *)event;
//- ()sendEvent:(NSEvent *)event
- ()keyDown:(NSEvent *)theEvent;
- ()flagsChanged:(NSEvent *)theEvent;
@end
