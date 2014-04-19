//
//  EAGLView.m
//  OpenGLBasics
//
//  Created by Charlie Key on 6/24/09.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#include <ewol/context/IOs/Context.h>
#include <ewol/Dimension.h>


#include <sys/types.h>
#include <sys/sysctl.h>

#import "OpenglView.h"
#include <ewol/debug.h>

// tuto de deploiment d'appo$ilcation :
// http://mobiforge.com/design-development/deploying-iphone-apps-real-devices
// http://www.techotopia.com/index.php/Testing_Apps_on_the_iPhone_–_Developer_Certificates_and_Provisioning_Profiles


#define USE_DEPTH_BUFFER 1
#define DEGREES_TO_RADIANS(__ANGLE) ((__ANGLE) / 180.0 * M_PI)

// A class extension to declare private methods
@interface OpenglView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;


@end


@implementation OpenglView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (NSString *) platform{
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char*)malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithUTF8String:machine];
    free(machine);
    return platform;
}

- (NSString *) platformString{
	NSString *platform = [self platform];
	if ([platform isEqualToString:@"iPhone1,1"])    return @"iPhone 1G";
	if ([platform isEqualToString:@"iPhone1,2"])    return @"iPhone 3G";
	if ([platform isEqualToString:@"iPhone2,1"])    return @"iPhone 3GS";
	if ([platform isEqualToString:@"iPhone3,1"])    return @"iPhone 4";
	if ([platform isEqualToString:@"iPhone3,3"])    return @"Verizon iPhone 4";
	if ([platform isEqualToString:@"iPhone4,1"])    return @"iPhone 4S";
	if ([platform isEqualToString:@"iPhone5,1"])    return @"iPhone 5 (GSM)";
	if ([platform isEqualToString:@"iPhone5,2"])    return @"iPhone 5 (GSM+CDMA)";
	if ([platform isEqualToString:@"iPhone5,3"])    return @"iPhone 5c (GSM)";
	if ([platform isEqualToString:@"iPhone5,4"])    return @"iPhone 5c (GSM+CDMA)";
	if ([platform isEqualToString:@"iPhone6,1"])    return @"iPhone 5s (GSM)";
	if ([platform isEqualToString:@"iPhone6,2"])    return @"iPhone 5s (GSM+CDMA)";
	if ([platform isEqualToString:@"iPod1,1"])      return @"iPod Touch 1G";
	if ([platform isEqualToString:@"iPod2,1"])      return @"iPod Touch 2G";
	if ([platform isEqualToString:@"iPod3,1"])      return @"iPod Touch 3G";
	if ([platform isEqualToString:@"iPod4,1"])      return @"iPod Touch 4G";
	if ([platform isEqualToString:@"iPod5,1"])      return @"iPod Touch 5G";
	if ([platform isEqualToString:@"iPad1,1"])      return @"iPad";
	if ([platform isEqualToString:@"iPad2,1"])      return @"iPad 2 (WiFi)";
	if ([platform isEqualToString:@"iPad2,2"])      return @"iPad 2 (GSM)";
	if ([platform isEqualToString:@"iPad2,3"])      return @"iPad 2 (CDMA)";
	if ([platform isEqualToString:@"iPad2,4"])      return @"iPad 2 (WiFi)";
	if ([platform isEqualToString:@"iPad2,5"])      return @"iPad Mini (WiFi)";
	if ([platform isEqualToString:@"iPad2,6"])      return @"iPad Mini (GSM)";
	if ([platform isEqualToString:@"iPad2,7"])      return @"iPad Mini (GSM+CDMA)";
	if ([platform isEqualToString:@"iPad3,1"])      return @"iPad 3 (WiFi)";
	if ([platform isEqualToString:@"iPad3,2"])      return @"iPad 3 (GSM+CDMA)";
	if ([platform isEqualToString:@"iPad3,3"])      return @"iPad 3 (GSM)";
	if ([platform isEqualToString:@"iPad3,4"])      return @"iPad 4 (WiFi)";
	if ([platform isEqualToString:@"iPad3,5"])      return @"iPad 4 (GSM)";
	if ([platform isEqualToString:@"iPad3,6"])      return @"iPad 4 (GSM+CDMA)";
	if ([platform isEqualToString:@"iPad4,1"])      return @"iPad Air (WiFi)";
	if ([platform isEqualToString:@"iPad4,2"])      return @"iPad Air (Cellular)";
	if ([platform isEqualToString:@"iPad4,4"])      return @"iPad mini 2G (WiFi)";
	if ([platform isEqualToString:@"iPad4,5"])      return @"iPad mini 2G (Cellular)";
	if ([platform isEqualToString:@"i386"])         return @"Simulator";
	if ([platform isEqualToString:@"x86_64"])       return @"Simulator";
	return platform;
}
- (float)getScreenPPP {
	NSString *platform = [self platform];
	if ([platform isEqualToString:@"iPhone1,1"])    return 326.0f; //@"iPhone 1G";
	if ([platform isEqualToString:@"iPhone1,2"])    return 326.0f; //@"iPhone 3G";
	if ([platform isEqualToString:@"iPhone2,1"])    return 326.0f; //@"iPhone 3GS";
	if ([platform isEqualToString:@"iPhone3,1"])    return 326.0f; //@"iPhone 4";
	if ([platform isEqualToString:@"iPhone3,3"])    return 326.0f; //@"Verizon iPhone 4";
	if ([platform isEqualToString:@"iPhone4,1"])    return 326.0f; //@"iPhone 4S";
	if ([platform isEqualToString:@"iPhone5,1"])    return 326.0f; //@"iPhone 5 (GSM)";
	if ([platform isEqualToString:@"iPhone5,2"])    return 326.0f; //@"iPhone 5 (GSM+CDMA)";
	if ([platform isEqualToString:@"iPhone5,3"])    return 326.0f; //@"iPhone 5c (GSM)";
	if ([platform isEqualToString:@"iPhone5,4"])    return 326.0f; //@"iPhone 5c (GSM+CDMA)";
	if ([platform isEqualToString:@"iPhone6,1"])    return 326.0f; //@"iPhone 5s (GSM)";
	if ([platform isEqualToString:@"iPhone6,2"])    return 326.0f; //@"iPhone 5s (GSM+CDMA)";
	if ([platform isEqualToString:@"iPod1,1"])      return 326.0f; //@"iPod Touch 1G";
	if ([platform isEqualToString:@"iPod2,1"])      return 326.0f; //@"iPod Touch 2G";
	if ([platform isEqualToString:@"iPod3,1"])      return 326.0f; //@"iPod Touch 3G";
	if ([platform isEqualToString:@"iPod4,1"])      return 326.0f; //@"iPod Touch 4G";
	if ([platform isEqualToString:@"iPod5,1"])      return 326.0f; //@"iPod Touch 5G";
	if ([platform isEqualToString:@"iPad1,1"])      return 264.0f; //@"iPad";
	if ([platform isEqualToString:@"iPad2,1"])      return 264.0f; //@"iPad 2 (WiFi)";
	if ([platform isEqualToString:@"iPad2,2"])      return 264.0f; //@"iPad 2 (GSM)";
	if ([platform isEqualToString:@"iPad2,3"])      return 264.0f; //@"iPad 2 (CDMA)";
	if ([platform isEqualToString:@"iPad2,4"])      return 264.0f; //@"iPad 2 (WiFi)";
	if ([platform isEqualToString:@"iPad2,5"])      return 163.0f; //@"iPad Mini (WiFi)";
	if ([platform isEqualToString:@"iPad2,6"])      return 163.0f; //@"iPad Mini (GSM)";
	if ([platform isEqualToString:@"iPad2,7"])      return 163.0f; //@"iPad Mini (GSM+CDMA)";
	if ([platform isEqualToString:@"iPad3,1"])      return 264.0f; //@"iPad 3 (WiFi)";
	if ([platform isEqualToString:@"iPad3,2"])      return 264.0f; //@"iPad 3 (GSM+CDMA)";
	if ([platform isEqualToString:@"iPad3,3"])      return 264.0f; //@"iPad 3 (GSM)";
	if ([platform isEqualToString:@"iPad3,4"])      return 264.0f; //@"iPad 4 (WiFi)";
	if ([platform isEqualToString:@"iPad3,5"])      return 264.0f; //@"iPad 4 (GSM)";
	if ([platform isEqualToString:@"iPad3,6"])      return 264.0f; //@"iPad 4 (GSM+CDMA)";
	if ([platform isEqualToString:@"iPad4,1"])      return 264.0f; //@"iPad Air (WiFi)";
	if ([platform isEqualToString:@"iPad4,2"])      return 264.0f; //@"iPad Air (Cellular)";
	if ([platform isEqualToString:@"iPad4,4"])      return 326.0f; //@"iPad mini 2G (WiFi)";
	if ([platform isEqualToString:@"iPad4,5"])      return 326.0f; //@"iPad mini 2G (Cellular)";
	if ([platform isEqualToString:@"i386"])         return 326.0f; //@"Simulator";
	if ([platform isEqualToString:@"x86_64"])       return 326.0f; //@"Simulator";
	return 326.0f;
}

- (void)configureAspectRatio {
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGFloat screenScale = [[UIScreen mainScreen] scale];
	m_currentSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
	
	NSLog(@"**** screen size : %fx%f\n", m_currentSize.width, m_currentSize.height);
	float ratio = [self getScreenPPP];
	NSLog(@"**** pixel ratio : %f ppp \n", ratio);
	ewol::Dimension::setPixelRatio(vec2(1.0f/ratio, 1.0f/ratio), ewol::Dimension::Inch);
	IOs::resize(m_currentSize.width, m_currentSize.height);

}
- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}

- (void)setupContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}


- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);
}

- (void)setupFrameBuffer {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
}

- (void)render:(CADisplayLink*)displayLink {
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	IOs::draw(true);
	//NSLog(@"draw...");
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}


- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (id)initWithFrame:(CGRect)frame
{
	animationInterval = 1.0 / 60.0;
    self = [super initWithFrame:frame];
    if (self) {
		[self configureAspectRatio];
        [self setupLayer];
        [self setupContext];
        [self setupDepthBuffer];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        [self setupDisplayLink];
    }
    return self;
}

- (void)drawView {
	//setting up the draw content
	[EAGLContext setCurrentContext:_context];
	// Open GL draw : ...
	IOs::draw(true);
	glFlush();
	
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	UITouch *touch = [touches anyObject];
	touchLocation = [touch locationInView:self];
	CGRect localBounds = [self bounds];
	EWOL_ERROR("touchesBegan: " << vec2(touchLocation.x, localBounds.size.height - touchLocation.y));
	IOs::setInputState(1, true, touchLocation.x, localBounds.size.height - touchLocation.y);
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	UITouch *touch = [touches anyObject];
	touchLocation = [touch locationInView:self];
	CGRect localBounds = [self bounds];
	EWOL_ERROR("touchesEnded: " << vec2(touchLocation.x, localBounds.size.height - touchLocation.y));
	IOs::setInputState(1, false, touchLocation.x, localBounds.size.height - touchLocation.y);
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	UITouch *touch = [touches anyObject];
	touchLocation = [touch locationInView:self];
	CGRect localBounds = [self bounds];
	EWOL_ERROR("touchesMoved: " << vec2(touchLocation.x, localBounds.size.height - touchLocation.y));
	IOs::setInputMotion(1, touchLocation.x, localBounds.size.height - touchLocation.y);
}

- (void)layoutSubviews {
    [EAGLContext setCurrentContext:context];
    [self drawView];
}


- (void)startAnimation {
	NSLog(@"start annimation\n");
	/*
    self.animationTimer = [NSTimer
						   scheduledTimerWithTimeInterval:animationInterval
						   target:self
						   selector:@selector(drawView)
						   userInfo:nil
						   repeats:YES];
	 */
}


- (void)stopAnimation {
	NSLog(@"Stop annimation\n");
    self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    NSLog(@"new timer\n");
	[animationTimer invalidate];
    animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}


- (void)dealloc {
    
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    //[context release];
    //[super dealloc];
}


@end
