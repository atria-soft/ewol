/**
 *******************************************************************************
 * @file guiX11.cpp
 * @brief Gui abstraction layer (Sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
 * @par Project
 * ewol
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


#include <ewolDebug.h>
#include <etkString.h>
#include <guiX11.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <X11/Xatom.h>
#ifdef EWOL_NO_VISUAL_CONFIG
#	include <X11/extensions/xf86vmode.h> 
#else
#	include <X11/extensions/Xrender.h>
#endif
//#define TEST_MODE_1

typedef struct {
	Display *dpy;
	int screen;
	Window win;
	GLXContext ctx;
	XSetWindowAttributes attr;
	Bool fs;
	Bool doubleBuffered;
	XF86VidModeModeInfo deskMode;
	int x, y;
	unsigned int width, height;
	unsigned int depth;
} GLWindow;

GLWindow GLWin;
//original desktop mode which we save so we can restore it later
XF86VidModeModeInfo desktopMode;

// attributes for a single buffered visual in RGBA format with at least 4 bits per color and a 16 bit depth buffer
static int attrListSgl[] = {
	GLX_RGBA, GLX_RED_SIZE, 4,
	GLX_GREEN_SIZE, 4,
	GLX_BLUE_SIZE, 4,
	GLX_DEPTH_SIZE, 16,
	None
};

// attributes for a double buffered visual in RGBA format with at least 4 bits per color and a 16 bit depth buffer
static int attrListDbl[] = {
	GLX_RGBA, GLX_DOUBLEBUFFER,
	GLX_RED_SIZE, 4,
	GLX_GREEN_SIZE, 4,
	GLX_BLUE_SIZE, 4,
	GLX_DEPTH_SIZE, 16,
	None
};

namespace guiAbstraction {
	extern "C" {
		typedef struct Hints
		{
			unsigned long   flags;
			unsigned long   functions;
			unsigned long   decorations;
			long            inputMode;
			unsigned long   status;
		} Hints;
	}
	class X11systemInterface
	{
		private:
			Atom del_atom;
			Display *m_display;
			GLXFBConfig fbconfig;
			Window WindowHandle, GLXWindowHandle;
			int width, height;
					XVisualInfo *visual;
			bool doubleBuffered;
			bool           m_run;
			ewol::Windows* m_uniqueWindows;
			
			bool CreateX11Context(void)
			{
				int x,y, attr_mask;
				XSizeHints hints;
				XWMHints *StartupState;
				XTextProperty textprop;
				XSetWindowAttributes attr;
				static char *title = (char*)"APPLICATION Title ... (todo)";
				
				// Connect to the X server
				m_display = XOpenDisplay(NULL);
				if(NULL == m_display) {
					EWOL_CRITICAL("Could not open display X.");
					exit(-1);
				} else {
					EWOL_INFO("Display opened.");
				}
				int Xscreen = DefaultScreen(m_display);
				Window Xroot = RootWindow(m_display, Xscreen);
				#ifdef EWOL_NO_VISUAL_CONFIG
					int glxMajor, glxMinor, vmMajor, vmMinor;
					XF86VidModeQueryVersion(m_display, &vmMajor, &vmMinor);
					printf("XF86 VideoMode extension version %d.%d\n", vmMajor, vmMinor);
					XF86VidModeModeInfo **modes;
					int modeNum, bestMode;
					XF86VidModeGetAllModeLines(m_display, Xscreen, &modeNum, &modes);
					/* save desktop-resolution before switching modes */
					GLWin.deskMode = *modes[0];
					desktopMode = *modes[0];
					/* look for mode with requested resolution */
					for (int32_t i = 0; i < modeNum; i++)
					{
						if(    (modes[i]->hdisplay == width)
						    && (modes[i]->vdisplay == height))
						{
							bestMode = i;
						}
					}
					/* get an appropriate visual */
					visual = glXChooseVisual(m_display, Xscreen, attrListDbl);
					
					if (visual == NULL)
					{
						visual = glXChooseVisual(m_display, Xscreen, attrListSgl);
						doubleBuffered = false;
						printf("singlebuffered rendering will be used, no doublebuffering available\n");
					}
					else
					{
						doubleBuffered = true;
						printf("doublebuffered rendering available\n");
					}
					glXQueryVersion(m_display, &glxMajor, &glxMinor);
					printf("GLX-Version %d.%d\n", glxMajor, glxMinor);
					
					attr.colormap = XCreateColormap(m_display, Xroot, visual->visual, AllocNone);
				#else
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
					GLXFBConfig *fbconfigs = glXChooseFBConfig(m_display, Xscreen, VisualData, &numfbconfigs);
					EWOL_DEBUG("get glx format config =" << numfbconfigs);
					for(int i = 0; i<numfbconfigs; i++) {
						visual = glXGetVisualFromFBConfig(m_display, fbconfigs[i]);
						if(!visual) {
							continue;
						}
						XRenderPictFormat * pictFormat = XRenderFindVisualFormat(m_display, visual->visual);
						if(!pictFormat) {
							continue;
						}
						
						if(pictFormat->direct.alphaMask > 0) {
							fbconfig = fbconfigs[i];
							EWOL_DEBUG("SELECT fbconfig id=" << i);
							break;
						}
					}
					// Create a colormap - only needed on some X clients, eg. IRIX
					attr.colormap = XCreateColormap(m_display, Xroot, visual->visual, AllocNone);
				#endif
				
				
				attr.border_pixel = 0;
				attr.event_mask =   StructureNotifyMask
				                  | SubstructureNotifyMask
				                  | EnterWindowMask
				                  | LeaveWindowMask
				                  | ExposureMask
				                  | ButtonPressMask
				                  | ButtonReleaseMask
				                  | OwnerGrabButtonMask
				                  | KeyPressMask
				                  | KeyReleaseMask
				                  | PointerMotionMask
				                  | FocusChangeMask
				                  | SubstructureRedirectMask;
				
				// set no background at the gui
				attr.background_pixmap = None;
				
				// select internal attribute
				attr_mask = CWBackPixmap | CWColormap | CWBorderPixel | CWEventMask;
				// Create the window
				width = DisplayWidth(m_display, DefaultScreen(m_display))/2;
				height = DisplayHeight(m_display, DefaultScreen(m_display))/2;
				x=width/2;
				y=height/4;
				
				// Real create of the window
				WindowHandle = XCreateWindow(m_display,
				                             Xroot,
				                             x, y, width, height,
				                             1,
				                             visual->depth,
				                             InputOutput,
				                             visual->visual,
				                             attr_mask, &attr);
				
				if( !WindowHandle ) {
					EWOL_CRITICAL("Couldn't create the window");
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
				
				XSetWMProperties(m_display, WindowHandle,&textprop, &textprop,/* Window title/icon title*/
					NULL, 0,/* Argv[], argc for program*/
					&hints, /* Start position/size*/
					StartupState,/* Iconised/not flag   */
					NULL);
				
				XFree(StartupState);
				
				/* Open it, wait for it to appear */
				XMapWindow(m_display, WindowHandle);
				//XIfEvent(m_display, &event, WaitForMapNotify, (char*)&WindowHandle);
				
				// Set the kill atom so we get a message when the user tries to close the window
				if ((del_atom = XInternAtom(m_display, "WM_DELETE_WINDOW", 0)) != None) {
					XSetWMProtocols(m_display, WindowHandle, &del_atom, 1);
				}
				
				//code to remove decoration
				/*
				{
					
					Hints hints;
					Atom property;
					hints.flags = 2;// Specify that we're changing the window decorations.
					hints.decorations = 0;// 0 (false) means that window decorations should go bye-bye
					property = XInternAtom(m_display, "_MOTIF_WM_HINTS", true);
					if (0 != property) {
						XChangeProperty(m_display,WindowHandle,property,property,32,PropModeReplace,(unsigned char *)&hints,5);
						XMapWindow(m_display, WindowHandle);
					} else {
						EWOL_ERROR("Can not get the property for the rmoving decoration of the X11 system ....");
					}
				}
				*/
				return true;
			}
			
			bool CreateOGlContext(void)
			{
				#ifndef EWOL_NO_VISUAL_CONFIG
					/* See if we can do OpenGL on this visual */
					int dummy;
					if (!glXQueryExtension(m_display, &dummy, &dummy)) {
						EWOL_CRITICAL("OpenGL not supported by X server");
						exit(-1);
					}
					
					/* Create the OpenGL rendering context */
					GLXContext RenderContext = glXCreateNewContext(m_display, fbconfig, GLX_RGBA_TYPE, 0, True);
					if (!RenderContext) {
						EWOL_CRITICAL("Failed to create a GL context");
						exit(-1);
					}
					
					GLXWindowHandle = glXCreateWindow(m_display, fbconfig, WindowHandle, NULL);
					
					/* Make it current */
					if (!glXMakeContextCurrent(m_display, GLXWindowHandle, GLXWindowHandle, RenderContext)) {
						EWOL_CRITICAL("glXMakeCurrent failed for window");
						exit(-1);
					}
				#else
					/* create a GLX context */
					GLXContext RenderContext = glXCreateContext(m_display, visual, 0, GL_TRUE);
						/* connect the glx-context to the window */
						glXMakeCurrent(m_display, WindowHandle, RenderContext);
						if (glXIsDirect(m_display, RenderContext)) {
							printf("DRI enabled\n");
						} else {
							printf("no DRI available\n");
						}
						glShadeModel(GL_SMOOTH);
						glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
						glClearDepth(1.0f);
						glEnable(GL_DEPTH_TEST);
						glDepthFunc(GL_LEQUAL);
						glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
						// we use resizeGL once to set up our initial perspective
						//resizeGL(width, height);
						//Reset the rotation angle of our object
						//rotQuad = 0;
						glFlush();
				#endif
				return true;
			}
			
			void Draw(void)
			{
				//EWOL_DEBUG("redraw (" << width << "," << height << ")");
				if(NULL == m_uniqueWindows) {
					//EWOL_DEBUG("Has No Windows set...");
					
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
				} else {
					m_uniqueWindows->SysDraw();
				}
				/* Swapbuffers */
				#ifndef EWOL_NO_VISUAL_CONFIG
					glXSwapBuffers(m_display, GLXWindowHandle);
				#else
					glFlush();
					// swap the buffers if we have doublebuffered
					if (doubleBuffered) {
						glXSwapBuffers(m_display, WindowHandle);
					}
				#endif
			}
		
		public:
			X11systemInterface(void)
			{
				visual = NULL;
				CreateX11Context();
				CreateOGlContext();
				m_run = true;
			}
			
			~X11systemInterface(void)
			{
				Stop();
			}
			
			void Setwindow(ewol::Windows* newWindows)
			{
				m_uniqueWindows = newWindows;
				if (NULL != m_uniqueWindows) {
					m_uniqueWindows->CalculateSize((double)width, (double)height);
				}
			}
			
			void Run(void)
			{
				// main cycle
				while(true == m_run) {
					XEvent event;
					// main X boucle :
					while (XPending(m_display)) {
						XNextEvent(m_display, &event);
						
						switch (event.type)
						{
							case ClientMessage:
								{
									Atom atom = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
									if((int64_t)atom == (int64_t)event.xclient.data.l[0]) {
										if (NULL != m_uniqueWindows) {
											m_uniqueWindows->SysOnKill();
										}
										Stop();
									}
								}
								break;
							case ConfigureNotify:
								width  = event.xconfigure.width;
								height = event.xconfigure.height;
								break;
						}
						// parse event
						if(NULL == m_uniqueWindows) {
							EWOL_DEBUG("Has No Windows set...");
						} else {
							switch (event.type)
							{
								case ConfigureNotify:
									EWOL_DEBUG("X11 event : " << event.type << " = \"ConfigureNotify\" Origin(" << event.xconfigure.x << "," << event.xconfigure.y << ") Size(" << event.xconfigure.width << "," << event.xconfigure.height << ")");
									m_uniqueWindows->CalculateSize((double)event.xconfigure.width, (double)event.xconfigure.height);
									break;
								case Expose:
									EWOL_DEBUG("X11 event : " << event.type << " = \"Expose\"");
									m_uniqueWindows->SysOnExpose();
									break;
								case ButtonPress:
									EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonPress\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
									if ( event.xbutton.button & Button2 ) {
										m_uniqueWindows->GenEventInput(2, ewol::EVENT_INPUT_TYPE_DOWN, (double)event.xbutton.x, (double)event.xbutton.y);
									} else if (event.xbutton.button & Button1) {
										m_uniqueWindows->GenEventInput(1, ewol::EVENT_INPUT_TYPE_DOWN, (double)event.xbutton.x, (double)event.xbutton.y);
									}
									break;
								case ButtonRelease:
									EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonRelease\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
									if(event.xbutton.button & Button2) {
										m_uniqueWindows->GenEventInput(2, ewol::EVENT_INPUT_TYPE_UP, (double)event.xbutton.x, (double)event.xbutton.y);
									} else if (event.xbutton.button & Button1) {
										m_uniqueWindows->GenEventInput(1, ewol::EVENT_INPUT_TYPE_UP, (double)event.xbutton.x, (double)event.xbutton.y);
									}
									break;
								case EnterNotify:
									EWOL_DEBUG("X11 event : " << event.type << " = \"EnterNotify\" (" << (double)event.xcrossing.x << "," << (double)event.xcrossing.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (double)event.xcrossing.x, (double)event.xcrossing.y);
									break;
								case MotionNotify:
									EWOL_DEBUG("X11 event : " << event.type << " = \"MotionNotify\" (" << (double)event.xmotion.x << "," << (double)event.xmotion.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_MOVE, (double)event.xmotion.x, (double)event.xmotion.y);
									break;
								case LeaveNotify:
									EWOL_DEBUG("X11 event : " << event.type << " = \"LeaveNotify\" (" << (double)event.xcrossing.x << "," << (double)event.xcrossing.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_LEAVE, (double)event.xcrossing.x, (double)event.xcrossing.y);
									break;
								case FocusIn:
									EWOL_DEBUG("X11 event : " << event.type << " = \"FocusIn\"");
									m_uniqueWindows->SetFocus();
									break;
								case FocusOut:
									EWOL_DEBUG("X11 event : " << event.type << " = \"FocusOut\"");
									m_uniqueWindows->RmFocus();
									break;
								case KeyPress:
								case KeyRelease:
									EWOL_DEBUG("X11 event : " << event.type << " = \"KeyPress/KeyRelease\" ");
									{
										char buf[11];
										KeySym keysym;
										XComposeStatus status;
										int count = XLookupString(&event.xkey, buf, 10, &keysym, &status);
										buf[count] = '\0';
										if(event.type == KeyPress) {
											// TODO : set the char here...
										} else {
											// TODO : set the char here...
										}
										break;
									}
								//case DestroyNotify:
								//	break;
								case MapNotify:
									EWOL_DEBUG("X11 event : " << event.type << " = \"MapNotify\"");
									m_uniqueWindows->SysOnShow();
									break;
								case UnmapNotify:
									EWOL_DEBUG("X11 event : " << event.type << " = \"UnmapNotify\"");
									m_uniqueWindows->SysOnHide();
									break;
								default:
									EWOL_DEBUG("X11 event : " << event.type << " = \"???\"");
							}
						}
					}
					Draw();
					usleep( 100000 );
				}
			}
			
			void Stop(void)
			{
				m_run = false;
			}
	};
};



#undef __class__
#define __class__ "guiAbstraction"

static bool guiAbstractionIsInit = false;
#ifdef TEST_MODE_1
static guiAbstraction::X11display * myDisplay = NULL;
static guiAbstraction::X11eventMng * myEventManager = NULL;
#else
static guiAbstraction::X11systemInterface * myX11Access = NULL;
#endif
void guiAbstraction::Init(int32_t argc, char *argv[])
{
	if (false == guiAbstractionIsInit) {
		// set the gui is init :
		guiAbstractionIsInit = true;
		EWOL_INFO("INIT for X11 environement");
		#ifdef TEST_MODE_1
			myDisplay = new guiAbstraction::X11display("");
			myEventManager = new guiAbstraction::X11eventMng(*myDisplay);
		#else
			myX11Access = new guiAbstraction::X11systemInterface();
		#endif
	} else {
		EWOL_CRITICAL("Can not INIT X11 ==> already init before");
	}
}


void guiAbstraction::Run(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("Start Running");
		#ifdef TEST_MODE_1
			myEventManager->Run();
		#else
			myX11Access->Run();
		#endif
		EWOL_INFO("Stop Running");
	} else {
		EWOL_CRITICAL("Can not Run X11 ==> not init ... ");
	}
}

void guiAbstraction::Stop(void)
{
	if (true == guiAbstractionIsInit) {
		#ifdef TEST_MODE_1
			myEventManager->Stop();
		#else
			myX11Access->Stop();
		#endif
	} else {
		EWOL_CRITICAL("Can not Stop X11 ==> not init ... ");
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows * newOne)
{
	if (true == guiAbstractionIsInit) {
		#ifdef TEST_MODE_1
			myEventManager->Setwindow(newOne);
		#else
			myX11Access->Setwindow(newOne);
		#endif
	} else {
		EWOL_CRITICAL("Can not set Windows X11 ==> not init ... ");
	}
}

void guiAbstraction::UnInit(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("UN-INIT for X11 environement");
		#ifdef TEST_MODE_1
			if (NULL != myEventManager) {
				delete(myEventManager);
			}
			if (NULL != myDisplay) {
				delete(myDisplay);
			}
		#else
			if (NULL != myX11Access) {
				delete(myX11Access);
			}
		#endif
		guiAbstractionIsInit = false;
	} else {
		EWOL_CRITICAL("Can not Un-Init X11 ==> not init ... ");
	}
}


