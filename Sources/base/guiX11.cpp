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
#if defined(EWOL_X11_MODE__XF86V)
#	include <X11/extensions/xf86vmode.h>
#elif defined(EWOL_X11_MODE__XRENDER)
#	include <X11/extensions/Xrender.h>
#else
#	error you might define an EWOL_X11_MODE in EWOL_X11_XF86V / EWOL_X11_XRENDER
#endif

#include <sys/times.h>


#undef __class__
#define __class__	"guiAbstraction"

#if defined(EWOL_X11_MODE__XF86V)
// attributes for a single buffered visual in RGBA format with at least 4 bits per color and a 16 bit depth buffer
static int attrListSgl[] = {
	GLX_RGBA,
	GLX_RED_SIZE, 4,
	GLX_GREEN_SIZE, 4,
	GLX_BLUE_SIZE, 4,
	GLX_DEPTH_SIZE, 16,
	None
};

// attributes for a double buffered visual in RGBA format with at least 4 bits per color and a 16 bit depth buffer
static int attrListDbl[] = {
	GLX_RGBA,
	GLX_DOUBLEBUFFER,
	GLX_RED_SIZE, 4,
	GLX_GREEN_SIZE, 4,
	GLX_BLUE_SIZE, 4,
	GLX_DEPTH_SIZE, 16,
	None
};
#elif defined(EWOL_X11_MODE__XRENDER)
static int VisualData[] = {
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
#endif


#define SEPARATED_CLICK_TIME          (30)

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
			// for double and triple click selection, we need to save the previous click up and down position , and the previous time ...
			int32_t m_previousBouttonId;
			int32_t m_previousDown_x;
			int32_t m_previousDown_y;
			int32_t m_previous_x;
			int32_t m_previous_y;
			int64_t m_previousTime;
			bool    m_previousDouble;
		private:
			Atom           m_delAtom;
			Display *      m_display;
			Window         WindowHandle;
			#if defined(EWOL_X11_MODE__XRENDER)
			GLXFBConfig    m_fbConfig;
			Window         m_GLXWindowHandle;
			#endif
			int32_t        m_width;
			int32_t        m_height;
			XVisualInfo *  m_visual;
			bool           m_doubleBuffered;
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
				
				#if defined(EWOL_X11_MODE__XF86V)
					EWOL_INFO("X11 Mode XF86 Video");
				#elif defined(EWOL_X11_MODE__XRENDER)
					EWOL_INFO("X11 Mode XRendrer Video");
				#endif
				
				// Connect to the X server
				m_display = XOpenDisplay(NULL);
				if(NULL == m_display) {
					EWOL_CRITICAL("Could not open display X.");
					exit(-1);
				} else {
					EWOL_INFO("Display opened.");
				}
				int Xscreen = DefaultScreen(m_display);
				#if defined(EWOL_X11_MODE__XF86V)
					{
						int32_t vmMajor, vmMinor;
						XF86VidModeQueryVersion(m_display, &vmMajor, &vmMinor);
						EWOL_INFO("XF86 VideoMode extension version " << vmMajor << "." << vmMinor);
					}
					// get an appropriate visual
					m_visual = glXChooseVisual(m_display, Xscreen, attrListDbl);
					if (NULL == m_visual) {
						m_visual = glXChooseVisual(m_display, Xscreen, attrListSgl);
						m_doubleBuffered = false;
						EWOL_INFO("XF86 singlebuffered rendering will be used, no doublebuffering available");
					} else {
						m_doubleBuffered = true;
						EWOL_INFO("XF86 doublebuffered rendering available");
					}
				#elif defined(EWOL_X11_MODE__XRENDER)
					int numfbconfigs;
					m_doubleBuffered = true;
					GLXFBConfig *fbconfigs = glXChooseFBConfig(m_display, Xscreen, VisualData, &numfbconfigs);
					EWOL_DEBUG("get glx format config =" << numfbconfigs);
					for(int i = 0; i<numfbconfigs; i++) {
						m_visual = glXGetVisualFromFBConfig(m_display, fbconfigs[i]);
						if(!m_visual) {
							continue;
						}
						XRenderPictFormat * pictFormat = XRenderFindVisualFormat(m_display, m_visual->visual);
						if(!pictFormat) {
							continue;
						}
						if(pictFormat->direct.alphaMask > 0) {
							m_fbConfig = fbconfigs[i];
							EWOL_DEBUG("SELECT fbconfig id=" << i);
							break;
						}
					}
				#endif
				{
					int32_t glxMajor, glxMinor;
					glXQueryVersion(m_display, &glxMajor, &glxMinor);
					EWOL_INFO("GLX-Version " << glxMajor << "." << glxMinor);
				}
				// Create a colormap - only needed on some X clients, eg. IRIX
				Window Xroot = RootWindow(m_display, Xscreen);
				attr.colormap = XCreateColormap(m_display, Xroot, m_visual->visual, AllocNone);
				
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
				m_width = DisplayWidth(m_display, DefaultScreen(m_display))/2;
				m_height = DisplayHeight(m_display, DefaultScreen(m_display))/2;
				x=m_width/2;
				y=m_height/4;
				
				// Real create of the window
				WindowHandle = XCreateWindow(m_display,
				                             Xroot,
				                             x, y, m_width, m_height,
				                             1,
				                             m_visual->depth,
				                             InputOutput,
				                             m_visual->visual,
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
				hints.width = m_width;
				hints.height = m_height;
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
				if ((m_delAtom = XInternAtom(m_display, "WM_DELETE_WINDOW", 0)) != None) {
					XSetWMProtocols(m_display, WindowHandle, &m_delAtom, 1);
				}
				
				//code to remove decoration
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
				return true;
			}
			
			bool CreateOGlContext(void)
			{
				#if defined(EWOL_X11_MODE__XRENDER)
					/* See if we can do OpenGL on this visual */
					int dummy;
					if (!glXQueryExtension(m_display, &dummy, &dummy)) {
						EWOL_CRITICAL("OpenGL not supported by X server");
						exit(-1);
					}
					
					/* Create the OpenGL rendering context */
					GLXContext RenderContext = glXCreateNewContext(m_display, m_fbConfig, GLX_RGBA_TYPE, 0, GL_TRUE);
					if (!RenderContext) {
						EWOL_CRITICAL("Failed to create a GL context");
						exit(-1);
					}
					m_GLXWindowHandle = glXCreateWindow(m_display, m_fbConfig, WindowHandle, NULL);
					/* Make it current */
					if (!glXMakeContextCurrent(m_display, m_GLXWindowHandle, m_GLXWindowHandle, RenderContext)) {
						EWOL_CRITICAL("glXMakeCurrent failed for window");
						exit(-1);
					}
				#elif defined(EWOL_X11_MODE__XF86V)
					/* create a GLX context */
					GLXContext RenderContext = glXCreateContext(m_display, m_visual, 0, GL_TRUE);
					/* connect the glx-context to the window */
					glXMakeCurrent(m_display, WindowHandle, RenderContext);
					if (glXIsDirect(m_display, RenderContext)) {
						EWOL_INFO("XF86 DRI enabled\n");
					} else {
						EWOL_INFO("XF86 DRI NOT available\n");
					}
				#endif
				return true;
			}
			
			void Draw(void)
			{
				//EWOL_DEBUG("redraw (" << m_width << "," << m_height << ")");
				if(NULL == m_uniqueWindows) {
					//EWOL_DEBUG("Has No Windows set...");
					
					// set the size of the open GL system
					glViewport(0,0,m_width,m_height);
					
					// Clear the screen with transparency ...
					glClearColor(0.750, 0.750, 0.750, 0.5);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0., (float)m_width, 0., (float)m_height, 1., 20.);
					
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(0, 0, -5);
					
					glBegin(GL_QUADS);
						glColor3f(1., 0., 0.); glVertex3f( .25*(float)m_width, .25*(float)m_height, 0.);
						glColor3f(0., 1., 0.); glVertex3f( .75*(float)m_width, .25*(float)m_height, 0.);
						glColor3f(0., 0., 1.); glVertex3f( .75*(float)m_width, .75*(float)m_height, 0.);
						glColor3f(1., 1., 0.); glVertex3f( .25*(float)m_width, .75*(float)m_height, 0.);
					glEnd();
				} else {
					m_uniqueWindows->SysDraw();
				}
				// swap the buffers if we have doublebuffered
				#if defined(EWOL_X11_MODE__XRENDER)
					glXSwapBuffers(m_display, m_GLXWindowHandle);
				#elif defined(EWOL_X11_MODE__XF86V)
					glFlush();
					if (m_doubleBuffered) {
						glXSwapBuffers(m_display, WindowHandle);
					}
				#endif
			}
		
		public:
			X11systemInterface(void)
			{
				m_visual = NULL;
				m_previousBouttonId = 0;
				m_previousDown_x = -1;
				m_previousDown_y = -1;
				m_previous_x = -1;
				m_previous_y = -1;
				m_previousTime = 0;
				m_previousDouble = false;
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
					m_uniqueWindows->CalculateSize((double)m_width, (double)m_height);
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
								m_width  = event.xconfigure.width;
								m_height = event.xconfigure.height;
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
									m_uniqueWindows->SetOrigin(event.xconfigure.x, event.xconfigure.y);
									break;
								case Expose:
									EWOL_DEBUG("X11 event : " << event.type << " = \"Expose\"");
									m_uniqueWindows->SysOnExpose();
									break;
								case ButtonPress:
									{
										int32_t btId = event.xbutton.button;
										//EWOL_DEBUG("X11 bt=" << btId << " event : " << event.type << "=\"ButtonPress\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
										// Send Down message
										m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_DOWN, (double)event.xbutton.x, (double)event.xbutton.y);
										// Check double or triple click event ...
										m_previousDown_x = event.xbutton.x;
										m_previousDown_y = event.xbutton.y;
										if (m_previousBouttonId != btId) {
											m_previousBouttonId = btId;
											m_previous_x = -1;
											m_previous_y = -1;
											m_previousTime = 0;
											m_previousDouble = false;
										} else {
											if(    abs(m_previous_x - event.xbutton.x) < 5
											    && abs(m_previous_y - event.xbutton.y) < 5 )
											{
												// nothink to do ... wait up ...
											} else {
												m_previous_x = -1;
												m_previous_y = -1;
												m_previousTime = 0;
												m_previousDouble = false;
											}
										}
									}
									break;
								case ButtonRelease:
									{
										int32_t btId = event.xbutton.button;
										
										//EWOL_DEBUG("X11 bt=" << btId << " event : " << event.type << "=\"ButtonRelease\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
										// send Up event ...
										m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_UP, (double)event.xbutton.x, (double)event.xbutton.y);
										
										if (m_previousBouttonId != btId) {
											m_previousDown_x = -1;
											m_previousDown_y = -1;
											m_previousBouttonId = 0;
											m_previous_x = -1;
											m_previous_y = -1;
											m_previousTime = 0;
											m_previousDouble = false;
										} else {
											int64_t currentTime = times(NULL); // return the tic in 10ms
											//EWOL_DEBUG("time is : " << currentTime << "    "<< currentTime/100 <<"s " << (currentTime%100)*10 << "ms");
											if (currentTime - m_previousTime >= SEPARATED_CLICK_TIME) {
												//check if the same area click : 
												if(    abs(m_previousDown_x - event.xbutton.x) < 5
												    && abs(m_previousDown_y - event.xbutton.y) < 5 )
												{
													// might generate an sigle event :
													//EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonClockedSingle\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
													m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_SINGLE, (double)event.xbutton.x, (double)event.xbutton.y);
													m_previous_x = m_previousDown_x;
													m_previous_y = m_previousDown_y;
													m_previousTime = currentTime;
												} else {
													// reset values ...
													m_previousDown_x = -1;
													m_previousDown_y = -1;
													m_previousBouttonId = 0;
													m_previous_x = -1;
													m_previous_y = -1;
													m_previousTime = 0;
												}
												m_previousDouble = false;
											} else {
												//check if the same area click : 
												if(    abs(m_previous_x - event.xbutton.x) < 5
												    && abs(m_previous_y - event.xbutton.y) < 5 )
												{
													// might generate an sigle event :
													if (false == m_previousDouble) {
														//EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonClockedDouble\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
														m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_DOUBLE, (double)event.xbutton.x, (double)event.xbutton.y);
														m_previousTime = currentTime;
														m_previousDouble = true;
													} else {
														//EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonClockedTriple\" (" << (double)event.xbutton.x << "," << (double)event.xbutton.y << ")");
														m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_TRIPLE, (double)event.xbutton.x, (double)event.xbutton.y);
														// reset values ...
														m_previousDown_x = -1;
														m_previousDown_y = -1;
														m_previousBouttonId = 0;
														m_previous_x = -1;
														m_previous_y = -1;
														m_previousTime = 0;
														m_previousDouble = false;
													}
												} else {
													// reset values ...
													m_previousDown_x = -1;
													m_previousDown_y = -1;
													m_previousBouttonId = 0;
													m_previous_x = -1;
													m_previous_y = -1;
													m_previousTime = 0;
													m_previousDouble = false;
												}
											}
											
											//int64_t currentTime = 
										}
									}
									break;
								case EnterNotify:
									//EWOL_DEBUG("X11 event : " << event.type << " = \"EnterNotify\" (" << (double)event.xcrossing.x << "," << (double)event.xcrossing.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (double)event.xcrossing.x, (double)event.xcrossing.y);
									break;
								case MotionNotify:
									//EWOL_DEBUG("X11 event : " << event.type << " = \"MotionNotify\" (" << (double)event.xmotion.x << "," << (double)event.xmotion.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_MOVE, (double)event.xmotion.x, (double)event.xmotion.y);
									break;
								case LeaveNotify:
									//EWOL_DEBUG("X11 event : " << event.type << " = \"LeaveNotify\" (" << (double)event.xcrossing.x << "," << (double)event.xcrossing.y << ")");
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
			
			void ChangeSize(int32_t w, int32_t h)
			{
				XResizeWindow(m_display, WindowHandle, w, h);
			}
			
			void ChangePos(int32_t x, int32_t y)
			{
				XMoveWindow(m_display, WindowHandle, x, y);
			}
	};
};



#undef __class__
#define __class__ "guiAbstraction"

static bool guiAbstractionIsInit = false;
static guiAbstraction::X11systemInterface * myX11Access = NULL;


void guiAbstraction::Init(int32_t argc, char *argv[])
{
	if (false == guiAbstractionIsInit) {
		// set the gui is init :
		guiAbstractionIsInit = true;
		EWOL_INFO("INIT for X11 environement");
		myX11Access = new guiAbstraction::X11systemInterface();
	} else {
		EWOL_CRITICAL("Can not INIT X11 ==> already init before");
	}
}


void guiAbstraction::Run(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("Start Running");
		myX11Access->Run();
		EWOL_INFO("Stop Running");
	} else {
		EWOL_CRITICAL("Can not Run X11 ==> not init ... ");
	}
}

void guiAbstraction::Stop(void)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->Stop();
	} else {
		EWOL_CRITICAL("Can not Stop X11 ==> not init ... ");
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows * newOne)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->Setwindow(newOne);
	} else {
		EWOL_CRITICAL("Can not set Windows X11 ==> not init ... ");
	}
}

void guiAbstraction::UnInit(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("UN-INIT for X11 environement");
		if (NULL != myX11Access) {
			delete(myX11Access);
		}
		guiAbstractionIsInit = false;
	} else {
		EWOL_CRITICAL("Can not Un-Init X11 ==> not init ... ");
	}
}


void guiAbstraction::ChangeSize(int32_t w, int32_t h)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->ChangeSize(w, h);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::ChangePos(int32_t x, int32_t y)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->ChangePos(x, y);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

