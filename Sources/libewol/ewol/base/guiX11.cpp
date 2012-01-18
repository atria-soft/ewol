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


#include <ewol/Debug.h>
#include <etk/String.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>

#include <ewol/Texture.h>

#include <unistd.h>
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


static int64_t GetCurrentTime(void)
{
	return times(NULL);
}

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
			int32_t        m_originX;
			int32_t        m_originY;
			int32_t        m_cursorEventX;
			int32_t        m_cursorEventY;
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
				
				ChangeSize(400, 300);
				
				return true;
			}
			
			void RemoveDecoration(void)
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
			
			void AddDecoration(void)
			{
				Hints hints;
				Atom property;
				hints.flags = 2;// Specify that we're changing the window decorations.
				hints.decorations = 1;// 1 (true) means that window decorations should enable
				property = XInternAtom(m_display, "_MOTIF_WM_HINTS", true);
				if (0 != property) {
					XChangeProperty(m_display,WindowHandle,property,property,32,PropModeReplace,(unsigned char *)&hints,5);
					XMapWindow(m_display, WindowHandle);
				} else {
					EWOL_ERROR("Can not get the property for the rmoving decoration of the X11 system ....");
				}
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
				//ewol::TextureOGLContext(true);
				return true;
			}
			
			void Draw(void)
			{
				// TODO : set this otherwise
				ewol::TextureOGLContext(true);
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
					glOrtho(0., (etkFloat_t)m_width, 0., (etkFloat_t)m_height, 1., 20.);
					
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(0, 0, -5);
					
					glBegin(GL_QUADS);
						glColor3f(1., 0., 0.); glVertex3f( .25*(etkFloat_t)m_width, .25*(etkFloat_t)m_height, 0.);
						glColor3f(0., 1., 0.); glVertex3f( .75*(etkFloat_t)m_width, .25*(etkFloat_t)m_height, 0.);
						glColor3f(0., 0., 1.); glVertex3f( .75*(etkFloat_t)m_width, .75*(etkFloat_t)m_height, 0.);
						glColor3f(1., 1., 0.); glVertex3f( .25*(etkFloat_t)m_width, .75*(etkFloat_t)m_height, 0.);
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
				m_originX = 0;
				m_originY = 0;
				m_cursorEventX = 0;
				m_cursorEventY = 0;
				for (int32_t iii=0; iii<NB_MAX_INPUT; iii++) {
					inputIsPressed[iii] = false;
				}
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
					m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
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
								m_originX = event.xconfigure.x;
								m_originY = event.xconfigure.y;
								break;
							case ButtonPress:
								m_cursorEventX = event.xbutton.x;
								m_cursorEventY = event.xbutton.y;
								if (event.xbutton.button < NB_MAX_INPUT) {
									inputIsPressed[event.xbutton.button] = true;
								}
								break;
							case ButtonRelease:
								m_cursorEventX = event.xbutton.x;
								m_cursorEventY = event.xbutton.y;
								if (event.xbutton.button < NB_MAX_INPUT) {
									inputIsPressed[event.xbutton.button] = false;
								}
								break;
							case EnterNotify:
							case LeaveNotify:
								m_cursorEventX = event.xcrossing.x;
								m_cursorEventY = event.xcrossing.y;
								break;
							case MotionNotify:
								m_cursorEventX = event.xmotion.x;
								m_cursorEventY = event.xmotion.y;
								break;
						}
						// parse event
						if(NULL == m_uniqueWindows) {
							EWOL_DEBUG("Has No Windows set...");
						} else {
							switch (event.type)
							{
								case ConfigureNotify:
									EWOL_VERBOSE("X11 event : " << event.type << " = \"ConfigureNotify\" Origin(" << event.xconfigure.x << "," << event.xconfigure.y << ") Size(" << event.xconfigure.width << "," << event.xconfigure.height << ")");
									m_uniqueWindows->CalculateSize((etkFloat_t)event.xconfigure.width, (etkFloat_t)event.xconfigure.height);
									m_uniqueWindows->SetOrigin(event.xconfigure.x, event.xconfigure.y);
									break;
								case Expose:
									EWOL_VERBOSE("X11 event : " << event.type << " = \"Expose\"");
									m_uniqueWindows->SysOnExpose();
									break;
								case ButtonPress:
									{
										int32_t btId = event.xbutton.button;
										EWOL_VERBOSE("X11 bt=" << btId << " event : " << event.type << "=\"ButtonPress\" (" << (etkFloat_t)event.xbutton.x << "," << (etkFloat_t)event.xbutton.y << ")");
										// Send Down message
										m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_DOWN, (etkFloat_t)event.xbutton.x, (etkFloat_t)event.xbutton.y);
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
										EWOL_VERBOSE("X11 bt=" << btId << " event : " << event.type << "=\"ButtonRelease\" (" << (etkFloat_t)event.xbutton.x << "," << (etkFloat_t)event.xbutton.y << ")");
										// send Up event ...
										m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_UP, (etkFloat_t)event.xbutton.x, (etkFloat_t)event.xbutton.y);
										
										if (m_previousBouttonId != btId) {
											m_previousDown_x = -1;
											m_previousDown_y = -1;
											m_previousBouttonId = 0;
											m_previous_x = -1;
											m_previous_y = -1;
											m_previousTime = 0;
											m_previousDouble = false;
										} else {
											int64_t currentTime = GetCurrentTime(); // return the tic in 10ms
											//EWOL_DEBUG("time is : " << currentTime << "    "<< currentTime/100 <<"s " << (currentTime%100)*10 << "ms");
											if (currentTime - m_previousTime >= SEPARATED_CLICK_TIME) {
												//check if the same area click : 
												if(    abs(m_previousDown_x - event.xbutton.x) < 5
												    && abs(m_previousDown_y - event.xbutton.y) < 5 )
												{
													// might generate an sigle event :
													//EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonClickedSingle\" (" << (etkFloat_t)event.xbutton.x << "," << (etkFloat_t)event.xbutton.y << ")");
													m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_SINGLE, (etkFloat_t)event.xbutton.x, (etkFloat_t)event.xbutton.y);
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
														//EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonClickedDouble\" (" << (etkFloat_t)event.xbutton.x << "," << (etkFloat_t)event.xbutton.y << ")");
														m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_DOUBLE, (etkFloat_t)event.xbutton.x, (etkFloat_t)event.xbutton.y);
														m_previousTime = currentTime;
														m_previousDouble = true;
													} else {
														//EWOL_DEBUG("X11 event : " << event.type << " = \"ButtonClickedTriple\" (" << (etkFloat_t)event.xbutton.x << "," << (etkFloat_t)event.xbutton.y << ")");
														m_uniqueWindows->GenEventInput(btId, ewol::EVENT_INPUT_TYPE_TRIPLE, (etkFloat_t)event.xbutton.x, (etkFloat_t)event.xbutton.y);
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
									//EWOL_DEBUG("X11 event : " << event.type << " = \"EnterNotify\" (" << (etkFloat_t)event.xcrossing.x << "," << (etkFloat_t)event.xcrossing.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (etkFloat_t)event.xcrossing.x, (etkFloat_t)event.xcrossing.y);
									break;
								case MotionNotify:
									{
										// For compatibility of the Android system : 
										bool findOne = false;
										for (int32_t iii=0; iii<NB_MAX_INPUT ; iii++) {
											if (true == inputIsPressed[iii]) {
												EWOL_VERBOSE("X11 event: bt=" << iii+1 << " " << event.type << " = \"MotionNotify\" (" << (etkFloat_t)event.xmotion.x << "," << (etkFloat_t)event.xmotion.y << ")");
												m_uniqueWindows->GenEventInput(iii+1, ewol::EVENT_INPUT_TYPE_MOVE, (etkFloat_t)event.xmotion.x, (etkFloat_t)event.xmotion.y);
												findOne = true;
											}
										}
										if (false == findOne) {
											EWOL_VERBOSE("X11 event: bt=" << 0 << " " << event.type << " = \"MotionNotify\" (" << (etkFloat_t)event.xmotion.x << "," << (etkFloat_t)event.xmotion.y << ")");
											m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_MOVE, (etkFloat_t)event.xmotion.x, (etkFloat_t)event.xmotion.y);
										}
									}
									break;
								case LeaveNotify:
									//EWOL_DEBUG("X11 event : " << event.type << " = \"LeaveNotify\" (" << (etkFloat_t)event.xcrossing.x << "," << (etkFloat_t)event.xcrossing.y << ")");
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_LEAVE, (etkFloat_t)event.xcrossing.x, (etkFloat_t)event.xcrossing.y);
									break;
								case FocusIn:
									EWOL_VERBOSE("X11 event : " << event.type << " = \"FocusIn\"");
									m_uniqueWindows->SetFocus();
									break;
								case FocusOut:
									EWOL_VERBOSE("X11 event : " << event.type << " = \"FocusOut\"");
									m_uniqueWindows->RmFocus();
									break;
								case KeyPress:
								case KeyRelease:
									//EWOL_DEBUG("X11 event : " << event.type << " = \"KeyPress/KeyRelease\" ");
									{
										EWOL_DEBUG("eventKey : " << event.xkey.keycode);
										bool find = true;
										ewol::eventKbMoveType_te keyInput;
										switch (event.xkey.keycode) {
											//case 80: // keypad
											case 111:	keyInput = ewol::EVENT_KB_MOVE_TYPE_UP;				break;
											//case 83: // keypad
											case 113:	keyInput = ewol::EVENT_KB_MOVE_TYPE_LEFT;			break;
											//case 85: // keypad
											case 114:	keyInput = ewol::EVENT_KB_MOVE_TYPE_RIGHT;			break;
											//case 88: // keypad
											case 116:	keyInput = ewol::EVENT_KB_MOVE_TYPE_DOWN;			break;
											//case 81: // keypad
											case 112:	keyInput = ewol::EVENT_KB_MOVE_TYPE_PAGE_UP;		break;
											//case 89: // keypad
											case 117:	keyInput = ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN;		break;
											//case 79: // keypad
											case 110:	keyInput = ewol::EVENT_KB_MOVE_TYPE_START;			break;
											//case 87: // keypad
											case 115:	keyInput = ewol::EVENT_KB_MOVE_TYPE_END;			break;
											case 78:	keyInput = ewol::EVENT_KB_MOVE_TYPE_ARRET_DEFIL;	break;
											case 127:	keyInput = ewol::EVENT_KB_MOVE_TYPE_WAIT;			break;
											//case 90: // keypad
											case 118:	keyInput = ewol::EVENT_KB_MOVE_TYPE_INSERT;			break;
											//case 84:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CENTER;			break; // Keypad
											case 67:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F1;				break;
											case 68:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F2;				break;
											case 69:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F3;				break;
											case 70:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F4;				break;
											case 71:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F5;				break;
											case 72:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F6;				break;
											case 73:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F7;				break;
											case 74:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F8;				break;
											case 75:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F9;				break;
											case 76:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F10;			break;
											case 95:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F11;			break;
											case 96:	keyInput = ewol::EVENT_KB_MOVE_TYPE_F12;			break;
											case 66:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CAPLOCK;		break;
											case 50:	keyInput = ewol::EVENT_KB_MOVE_TYPE_SHIFT_LEFT;		break;
											case 62:	keyInput = ewol::EVENT_KB_MOVE_TYPE_SHIFT_RIGHT;	break;
											case 37:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CTRL_LEFT;		break;
											case 105:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CTRL_RIGHT;		break;
											case 133:	keyInput = ewol::EVENT_KB_MOVE_TYPE_META_LEFT;		break;
											case 134:	keyInput = ewol::EVENT_KB_MOVE_TYPE_META_RIGHT;		break;
											case 64:	keyInput = ewol::EVENT_KB_MOVE_TYPE_ALT;			break;
											case 108:	keyInput = ewol::EVENT_KB_MOVE_TYPE_ALT_GR;			break;
											case 135:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU;	break;
											case 77:	keyInput = ewol::EVENT_KB_MOVE_TYPE_VER_NUM;		break;
											case 91: // Suppr on keypad
												find = false;
												{
													char buf[2];
													buf[0] = 0x7F;
													buf[1] = 0x00;
													etk::String tmpData = buf;
													if(event.type == KeyPress) {
														SendKeyboardEvent(true, tmpData);
													} else {
														SendKeyboardEvent(false, tmpData);
													}
												}
											default:
												find = false;
												{
													char buf[11];
													KeySym keysym;
													XComposeStatus status;
													int count = XLookupString(&event.xkey, buf, 10, &keysym, &status);
													buf[count] = '\0';
													EWOL_WARNING("Unknow event Key : " << event.xkey.keycode << " char=" << (int32_t)buf[0]);
													if (buf[0] == '\r') {
														buf[0] = '\n';
														buf[1] = '\0';
													}
													if (count>0) {
														etk::String tmpData = buf;
														if(event.type == KeyPress) {
															SendKeyboardEvent(true, tmpData);
														} else {
															SendKeyboardEvent(false, tmpData);
														}
													} else {
														EWOL_WARNING("Unknow event Key : " << event.xkey.keycode);
													}
												}
												break;
										}
										if (true == find) {
											EWOL_DEBUG("eventKey Move type : " << GetCharTypeMoveEvent(keyInput) );
											if(event.type == KeyPress) {
												SendKeyboardEventMove(true, keyInput);
											} else {
												SendKeyboardEventMove(false, keyInput);
											}
										}
										break;
									}
								//case DestroyNotify:
								//	break;
								case MapNotify:
									EWOL_VERBOSE("X11 event : " << event.type << " = \"MapNotify\"");
									m_uniqueWindows->SysOnShow();
									break;
								case UnmapNotify:
									EWOL_VERBOSE("X11 event : " << event.type << " = \"UnmapNotify\"");
									m_uniqueWindows->SysOnHide();
									break;
								default:
									EWOL_DEBUG("X11 event : " << event.type << " = \"???\"");
							}
						}
					}
					Draw();
					//usleep( 100000 );
				}
			};
			
			void Stop(void)
			{
				m_run = false;
			};
			
			void ChangeSize(int32_t w, int32_t h)
			{
				XResizeWindow(m_display, WindowHandle, w, h);
			};
			
			void ChangePos(int32_t x, int32_t y)
			{
				XMoveWindow(m_display, WindowHandle, x, y);
			};
			
			void GetAbsPos(int32_t & x, int32_t & y)
			{
				int tmp;
				unsigned int tmp2;
				Window fromroot, tmpwin;
				XQueryPointer(m_display, WindowHandle, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp2);
			};
			
			void KeyboardShow(ewol::keyboardMode_te mode)
			{
				if (NULL != m_uniqueWindows) {
					m_uniqueWindows->KeyboardShow(mode);
				}
			}
			void KeyboardHide(void)
			{
				if (NULL != m_uniqueWindows) {
					m_uniqueWindows->KeyboardHide();
				}
				ForceRedrawAll();
			}
			
			void ForceRedrawAll(void)
			{
				if (NULL != m_uniqueWindows) {
					m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
				}
			};
		
		private:
			/*
			int32_t m_startX;
			int32_t m_startY;
			int32_t m_screenOffsetX;
			int32_t m_screenOffsetY;
			*/
		private:
			bool inputIsPressed[20];
		public:
			bool IsPressedInput(int32_t inputID)
			{
				if(    NB_MAX_INPUT > inputID
				    && 0 <= inputID)
				{
					return inputIsPressed[inputID];
				} else {
					EWOL_WARNING("Wrong input ID : " << inputID);
					return false;
				}
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

void guiAbstraction::GetAbsPos(int32_t & x, int32_t & y)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->GetAbsPos(x, y);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

bool guiAbstraction::IsPressedInput(int32_t inputID)
{
	if (true == guiAbstractionIsInit) {
		return myX11Access->IsPressedInput(inputID);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
		return false;
	}
}

void guiAbstraction::KeyboardShow(ewol::keyboardMode_te mode)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->KeyboardShow(mode);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::KeyboardHide(void)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->KeyboardHide();
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::ForceRedrawAll(void)
{
	if (true == guiAbstractionIsInit) {
		myX11Access->ForceRedrawAll();
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::SendKeyboardEvent(bool isDown, etk::String &keyInput)
{
	// Get the current Focused Widget :
	ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
	if (NULL != tmpWidget) {
		if(true == isDown) {
			EWOL_DEBUG("X11 PRESSED : \"" << keyInput << "\" size=" << keyInput.Size());
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_DOWN, keyInput.c_str());
		} else {
			EWOL_DEBUG("X11 Release : \"" << keyInput << "\" size=" << keyInput.Size());
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_UP, keyInput.c_str());
		}
	}
}


void guiAbstraction::SendKeyboardEventMove(bool isDown, ewol::eventKbMoveType_te &keyInput)
{
	// Get the current Focused Widget :
	ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
	if (NULL != tmpWidget) {
		if(true == isDown) {
			tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_DOWN, keyInput);
		} else {
			tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_UP, keyInput);
		}
	}
}

#include <ewol/ewol.h>





int main(int argc, char *argv[])
{
	guiAbstraction::Init(argc, argv);
	// init Ewol
	ewol::Init(argc, argv);
	// Init Application ...
	APP_Init(argc, argv);
	// Start Ewol diwplay while
	guiAbstraction::Run();
	// unset all windows
	ewol::DisplayWindows(NULL);
	// call application to uninit
	APP_UnInit();
	// basic abstraction un-init
	guiAbstraction::UnInit();
	// uninit Ewol
	ewol::UnInit();
	
	return 0;
}

