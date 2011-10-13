/**
 *******************************************************************************
 * @file Main.cpp
 * @brief main fonction
 * @author Edouard DUPIN
 * @date 13/10/2011
 * @par Project
 * Edn 
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>

// need to run xcompmgr to have transparency

static Atom del_atom;
static Display *Xdisplay;
static GLXFBConfig fbconfig;
static Window WindowHandle, GLXWindowHandle;
static int width, height;


/**
 * @brief 
 */
static Bool WaitForMapNotify(Display *d, XEvent *e, char *arg)
{
	return (e->type == MapNotify) && (e->xmap.window == *(Window*)arg);
}

/**
 * @brief Create the X11 windows
 */
static void createX11Window()
{
	XEvent event;
	int x,y, attr_mask;
	XSizeHints hints;
	XWMHints *StartupState;
	XTextProperty textprop;
	XSetWindowAttributes attr;
	static char *title = (char*)"FTB's little OpenGL example";

	// Connect to the X server
	Xdisplay = XOpenDisplay(NULL);
	if (NULL == Xdisplay) {
		fprintf(stderr, "Couldn't connect to X server\n");
		exit(-1);
	}
	int Xscreen = DefaultScreen(Xdisplay);
	Window Xroot = RootWindow(Xdisplay, Xscreen);

	int numfbconfigs;
	int VisualData[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, True,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		GLX_ALPHA_SIZE, 1,
		GLX_DEPTH_SIZE, 1,
		None
	};
	XVisualInfo *visual;
	GLXFBConfig *fbconfigs = glXChooseFBConfig(Xdisplay, Xscreen, VisualData, &numfbconfigs);
	for(int i = 0; i<numfbconfigs; i++) {
		visual = glXGetVisualFromFBConfig(Xdisplay, fbconfigs[i]);
		if(!visual) {
			continue;
		}

		XRenderPictFormat * pictFormat = XRenderFindVisualFormat(Xdisplay, visual->visual);
		if(!pictFormat) {
			continue;
		}

		if(pictFormat->direct.alphaMask > 0) {
			fbconfig = fbconfigs[i];
			break;
		}
	}

	// Create a colormap - only needed on some X clients, eg. IRIX
	attr.colormap = XCreateColormap(Xdisplay, Xroot, visual->visual, AllocNone);;


	attr.border_pixel = 0;
	attr.event_mask =   StructureNotifyMask
	                  | EnterWindowMask
	                  | LeaveWindowMask
	                  | ExposureMask
	                  | ButtonPressMask
	                  | ButtonReleaseMask
	                  | OwnerGrabButtonMask
	                  | KeyPressMask
	                  | KeyReleaseMask;

	// set no background at the gui
	attr.background_pixmap = None;

	// select internal attribute
	attr_mask = CWBackPixmap | CWColormap | CWBorderPixel | CWEventMask;
	// Create the window
	width = DisplayWidth(Xdisplay, DefaultScreen(Xdisplay))/2;
	height = DisplayHeight(Xdisplay, DefaultScreen(Xdisplay))/2;
	x=width/2;
	y=height/4;

	// Real create of the window
	WindowHandle = XCreateWindow(Xdisplay,
	                             Xroot,
	                             x, y, width, height,
	                             1,
	                             visual->depth,
	                             InputOutput,
	                             visual->visual,
	                             attr_mask, &attr);

	if( !WindowHandle ) {
		fprintf(stderr, "Couldn't create the window\n");
		exit(-1);
	}

	/* Configure it...  (ok, ok, this next bit isn't "minimal") */
	textprop.value = (unsigned char*)title;
	textprop.encoding = XA_STRING;
	textprop.format = 8;
	textprop.nitems = strlen(title);

	hints.x = x;
	hints.y = y;
	hints.width = width;
	hints.height = height;
	hints.flags = USPosition|USSize;

	StartupState = XAllocWMHints();
	StartupState->initial_state = NormalState;
	StartupState->flags = StateHint;

	XSetWMProperties(Xdisplay, WindowHandle,&textprop, &textprop,/* Window title/icon title*/
		NULL, 0,/* Argv[], argc for program*/
		&hints, /* Start position/size*/
		StartupState,/* Iconised/not flag   */
		NULL);

	XFree(StartupState);

	/* Open it, wait for it to appear */
	XMapWindow(Xdisplay, WindowHandle);
	XIfEvent(Xdisplay, &event, WaitForMapNotify, (char*)&WindowHandle);

	// Set the kill atom so we get a message when the user tries to close the window
	if ((del_atom = XInternAtom(Xdisplay, "WM_DELETE_WINDOW", 0)) != None) {
		XSetWMProtocols(Xdisplay, WindowHandle, &del_atom, 1);
	}
}


/**
 * @brief Create a special context to manage transparency of the windows inside X11 system :
 */
static void createTheRenderContext()
{
	/* See if we can do OpenGL on this visual */
	int dummy;
	if (!glXQueryExtension(Xdisplay, &dummy, &dummy)) {
		fprintf(stderr, "OpenGL not supported by X server\n");
		exit(-1);
	}

	/* Create the OpenGL rendering context */
	GLXContext RenderContext = glXCreateNewContext(Xdisplay, fbconfig, GLX_RGBA_TYPE, 0, True);
	if (!RenderContext) {
		fprintf(stderr, "Failed to create a GL context\n");
		exit(-1);
	}

	GLXWindowHandle = glXCreateWindow(Xdisplay, fbconfig, WindowHandle, NULL);

	/* Make it current */
	if (!glXMakeContextCurrent(Xdisplay, GLXWindowHandle, GLXWindowHandle, RenderContext)) {
		fprintf(stderr, "glXMakeCurrent failed for window\n");
		exit(-1);
	}

}

/**
 * @brief draw the current diplay of the screen
 */
static void Draw(void)
{
	// set the size of the open GL system
	glViewport(0,0,width,height);
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., (float)width, 0., (float)height, 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);

	glBegin(GL_QUADS);
		glColor3f(1., 0., 0.); glVertex3f( .25*(float)width, .25*(float)height, 0.);
		glColor3f(0., 1., 0.); glVertex3f( .75*(float)width, .25*(float)height, 0.);
		glColor3f(0., 0., 1.); glVertex3f( .75*(float)width, .75*(float)height, 0.);
		glColor3f(1., 1., 0.); glVertex3f( .25*(float)width, .75*(float)height, 0.);
	glEnd();

	printf("redraw (%d,%d)\n", width, height);
	/* Swapbuffers */
	glXSwapBuffers(Xdisplay, GLXWindowHandle);
}


/**
 * @brief main input fonction
 */
int main(int argc, char *argv[])
{
	printf("==================================================\n");
	printf("== Configuration (Start)\n");
	printf("==================================================\n");
	createX11Window();
	
	createTheRenderContext();

	printf("==================================================\n");
	printf("== Configuration (End)\n");
	printf("==================================================\n");

	printf("==================================================\n");
	printf("== Main Process (Start)\n");
	printf("==================================================\n");
	// main cycle
	while(1) {
		XEvent event;
		XConfigureEvent *xc;
		// main X boucle : 
		while (XPending(Xdisplay)) {
			XNextEvent(Xdisplay, &event);
			switch (event.type)
			{
				case ClientMessage:
					// Request close of the current client :
					if (event.xclient.data.l[0] == del_atom) {
						// TODO : Clear all internal elements ...
						exit(0);
					}
					break;
				case ConfigureNotify:
					xc = &(event.xconfigure);
					width = xc->width;
					height = xc->height;
					break;
			}
		}
		Draw();
		usleep( 100000 );
	}
	
	return 0;
}
