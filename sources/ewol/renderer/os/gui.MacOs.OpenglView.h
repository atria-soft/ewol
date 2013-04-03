

#import <UIKit/UIKit.h>
#import <OpenGLES/ES2/gl.h> 
#import <QuartzCore/QuartzCore.h>

@interface OpenglView : UIView {
    
    EAGLContext *mimContext;
    
@private
    GLuint framebuffer;
    GLuint renderbuffer;
}
- (void)drawView;
@end
