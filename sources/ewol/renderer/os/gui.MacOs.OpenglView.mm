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
    EWOL_INFO("mouseDown" << (float)point.x << " " << (float)point.y);
    EWOL_INFO("mouseDown");
}
-(void)mouseDragged:(NSEvent *)event {
    EWOL_INFO("mouseDragged");
}
-(void)mouseUp:(NSEvent *)event {
    EWOL_INFO("mouseUp");
}
-(void)mouseMoved:(NSEvent *)event {
    EWOL_INFO("mouseMoved");
}
-(void)mouseEntered:(NSEvent *)event {
    EWOL_INFO("mouseEntered");
}
-(void)mouseExited:(NSEvent *)event {
    EWOL_INFO("mouseExited");
}
-(void)rightMouseDown:(NSEvent *)event {
    EWOL_INFO("rightMouseDown");
}
-(void)rightMouseDragged:(NSEvent *)event {
    EWOL_INFO("rightMouseDragged");
}
-(void)rightMouseUp:(NSEvent *)event {
    EWOL_INFO("rightMouseUp");
}
-(void)otherMouseDown:(NSEvent *)event {
    EWOL_INFO("otherMouseDown");
}
-(void)otherMouseDragged:(NSEvent *)event {
    EWOL_INFO("otherMouseDragged");
}
-(void)otherMouseUp:(NSEvent *)event {
    EWOL_INFO("otherMouseUp");
}


@end
