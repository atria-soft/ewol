/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#import <ewol/renderer/os/gui.MacOs.OpenglView.h>
#include <OpenGL/gl.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/debug.h>
 
static int32_t width=0, height=0;
 
@implementation OpenGLView

-(void) drawRect: (NSRect) bounds
{
	if(    width!= bounds.size.width
	    || height!= bounds.size.height) {
		width= bounds.size.width;
		height= bounds.size.height;
		eSystem::Resize(width,height);
	}
    eSystem::Draw(true);
    glFlush();
}


-(void)mouseDown:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	//float x = [event locationInWindow].x; //point.x;
	EWOL_INFO("mouseDown : " << (float)point.x << " " << (float)point.y);
	eSystem::SetMouseState(1, true, point.x, point.y);
}
-(void)mouseDragged:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("mouseDragged : " << (float)point.x << " " << (float)point.y);
	eSystem::SetMouseMotion(1, point.x, point.y);
}
-(void)mouseUp:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("mouseUp : " << (float)point.x << " " << (float)point.y);
	eSystem::SetMouseState(1, false, point.x, point.y);
}
-(void)mouseMoved:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("mouseMoved : " << (float)point.x << " " << (float)point.y);
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
	EWOL_INFO("rightMouseDown : " << (float)point.x << " " << (float)point.y);
	eSystem::SetMouseState(3, true, point.x, point.y);
}
-(void)rightMouseDragged:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("rightMouseDragged : " << (float)point.x << " " << (float)point.y);
	eSystem::SetMouseMotion(3, point.x, point.y);
}
-(void)rightMouseUp:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("rightMouseUp : " << (float)point.x << " " << (float)point.y);
	eSystem::SetMouseState(3, false, point.x, point.y);
}
-(void)otherMouseDown:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("otherMouseDown : " << (float)point.x << " " << (float)point.y);
}
-(void)otherMouseDragged:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("otherMouseDragged : " << (float)point.x << " " << (float)point.y);
}
-(void)otherMouseUp:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("otherMouseUp : " << (float)point.x << " " << (float)point.y);
}
/*
// http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/EventOverview/EventArchitecture/EventArchitecture.html
-(void)sendEvent:(NSEvent *)event {
	NSPoint point = [event locationInWindow];
	EWOL_INFO("sendEvent : " << (float)point.x << " " << (float)point.y);
}
*/
@end
