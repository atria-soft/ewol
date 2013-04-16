/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#import <ewol/renderer/os/gui.MacOs.OpenglView.h>
#include <OpenGL/gl.h>
#include <ewol/renderer/os/eSystemMacOSInterface.h>
 
static int32_t width=0, height=0;
 
@implementation OpenGLView

-(void) drawRect: (NSRect) bounds
{
	if(    width!= bounds.size.width
	    || height!= bounds.size.height) {
		width= bounds.size.width;
		height= bounds.size.height;
		eSystem_Resize(width,height);
	}
    eSystem_Draw(true);
    glFlush();
}


@end
