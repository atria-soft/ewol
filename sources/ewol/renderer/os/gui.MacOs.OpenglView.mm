

#import <ewol/renderer/os/gui.MacOs.OpenglView.h>

#if 0

@implementation OpenglView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
        eaglLayer.opaque = YES;
        
        mimContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        BOOL setContext=[EAGLContext setCurrentContext:mimContext];
        if (!mimContext || !setContext) 
        {
            [self release];
            return nil;
        }
     
        //Lets init and bind render buffer with current context
        glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer); 
        [mimContext renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];
        
        //Frame buffer- which is a collection of render buffers and other kind of buffers like depth etc.
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);        

        //Setting the dimensions of your view area.
        glViewport(0, 0, CGRectGetWidth(frame), CGRectGetHeight(frame));
        
       [self drawView];

        
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawView
{
    // Drawing code
    glClearColor(1.0f, 0.0f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    [mimContext presentRenderbuffer:GL_RENDERBUFFER];
}


- (void)dealloc
{
    [super dealloc];
}

@end
#endif

