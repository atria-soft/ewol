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
#include <X11/extensions/Xrender.h>

//#define TEST_MODE_1

/*
bool guiAbstraction(Display *d, XEvent *e, char *arg)
{
	return (e->type == MapNotify) && (e->xmap.window == *(Window*)arg);
}
*/

namespace guiAbstraction {
#ifdef TEST_MODE_1

	#undef __class__
	#define __class__ "guiAbstraction::X11display"
	class X11display
	{
		public:
			X11display( etk::String name ) {
				m_display = XOpenDisplay( name.c_str() );
				if(NULL == m_display) {
					EWOL_CRITICAL("Could not open display named='" << name << "'.");
				} else {
					EWOL_INFO("Display opened named='" << name << "'.");
				}
			}
			~X11display() {
				if( NULL != m_display ) {
					XCloseDisplay ( m_display );
					m_display = 0;
				}
			}
			Display * GetDisplay(void)
			{
				return m_display;
			}
		private:
			Display* m_display;
	};

	#undef __class__
	#define __class__ "guiAbstraction::X11eventMng"
	class X11eventMng
	{
		private:
			ewol::Windows* m_uniqueWindows;
			Display * m_display;
			bool m_run;
		public:
			X11eventMng(X11display& d) : m_run(true), m_display(d.GetDisplay() )
			{
				m_uniqueWindows = NULL;
			}
			
			~X11eventMng()
			{
				
			}
			
			void Setwindow(ewol::Windows* newWindows)
			{
				m_uniqueWindows = newWindows;
			}
			
			void Run()
			{
				m_run = true;
				XEvent event;
				while (m_run) {
					while (XPending(m_display)) {
						//...
						// draw the current windows in every case ... 
						m_uniqueWindows->SysDraw();
						// For test, otherwithe the display leach all the CPU
						usleep( 100000 );
					}//end if m_uniqueWindows
				}
			}
			
			void Stop()
			{
				m_run = false;
			}
			
			Display * GetDisplay(void)
			{
				return m_display;
			}
		
	};

	const int event_mask =   ExposureMask
	                       | ButtonPressMask
	                       | ButtonReleaseMask
	                       | EnterWindowMask
	                       | LeaveWindowMask
	                       | PointerMotionMask
	                       | FocusChangeMask
	                       | KeyPressMask
	                       | KeyReleaseMask
	                       | SubstructureNotifyMask
	                       | StructureNotifyMask
	                       | SubstructureRedirectMask;
	

	#undef __class__
	#define __class__ "guiAbstraction::X11Windows"
	class X11Windows
	{
		private:
			Display * m_display;
			Window m_window;
			X11eventMng& m_event_dispatcher;
			Atom m_atom[1];
			
			size_ts      m_size;
			position_ts  m_position;
		private:
			// Not copyable
			X11Windows(const X11Windows&);
			void operator=(X11Windows&);
		public:
			X11Windows(X11eventMng& e)
			    : m_display(e.GetDisplay()),
			      m_event_dispatcher(e)
			{
				m_window = 0;
				m_atom[0] = 0;
				if(!m_window) {
					int Xscreen = DefaultScreen((void*)m_display);
					Window Xroot = RootWindow((void*)m_display, Xscreen);
					//RootWindow((void*)m_display,0)
					
					m_window = XCreateSimpleWindow(m_display,
					                               Xroot,
					                               20, // origin X
					                               20, // origin Y
					                               300, // Width
					                               200, //Height
					                               0,
					                               WhitePixel((void*)m_display,0),
					                               WhitePixel((void*)m_display,0));
					//set_background ( m_background );
					if(0==m_window) {
						EWOL_CRITICAL("Could not create the basic window");
					}
				}
				//m_event_dispatcher.Setwindow(this);
				Show();
			}
			
			~X11Windows(void)
			{
				Hide();
				if(m_window) {
					XDestroyWindow(m_display, m_window);
					m_window = 0;
				}
				m_event_dispatcher.Setwindow(NULL);
			}
		private:
			//
			// From window_base:
			//
			virtual void Show(void)
			{
				XSelectInput(m_display, m_window, guiAbstraction::event_mask);
				XMapWindow(m_display, m_window);
				XFlush(m_display);
			}
			
			virtual void Hide(void)
			{
				XUnmapWindow(m_display, m_window);
				XFlush(m_display);
			}
			
			/*
			
			virtual void set_background(color& c)
			{
				// hold a ref to the alloc'ed color
				m_background.set(c);
				XSetWindowBackground(m_display, m_window, c.pixel());
				refresh();
			}
			
			virtual void set_focus(void)
			{
				XSetInputFocus(m_display, id(), RevertToParent, CurrentTime );
				refresh();
			}
			virtual void refresh(void)
			{
				XClearWindow(m_display, m_window);
				XFlush(m_display);
				on_expose();
			}
			
			virtual rectangle get_rect(void)
			{
				Window root;
				int x = 0, y = 0;
				unsigned int width = 0, height = 0, border_width = 0, depth = 0;
				XGetGeometry(m_display,
				             m_window,
				             &root,
				             &x,
				             &y,
				             &width,
				             &height,
				             &border_width,
				             &depth);
				return rectangle(point(x,y), width, height );
			}
			*/
			virtual long Id(void)
			{
				return m_window;
			}
			
			
			Display * GetDisplay(void)
			{
				return m_display;
			}
			
			virtual X11eventMng& get_event_dispatcher(void)
			{
				return m_event_dispatcher;
			}
			
	};
#else

	class X11systemInterface
	{
		private:
			Atom del_atom;
			Display *m_display;
			GLXFBConfig fbconfig;
			Window WindowHandle, GLXWindowHandle;
			int width, height;
			
			bool           m_run;
			ewol::Windows* m_uniqueWindows;
			
			bool CreateX11Context(void)
			{
				XEvent event;
				int x,y, attr_mask;
				XSizeHints hints;
				XWMHints *StartupState;
				XTextProperty textprop;
				XSetWindowAttributes attr;
				static char *title = (char*)"FTB's little OpenGL example";
				
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
				GLXFBConfig *fbconfigs = glXChooseFBConfig(m_display, Xscreen, VisualData, &numfbconfigs);
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
						break;
					}
				}
				
				// Create a colormap - only needed on some X clients, eg. IRIX
				attr.colormap = XCreateColormap(m_display, Xroot, visual->visual, AllocNone);
				
				
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
				return true;
			}
			
			bool CreateOGlContext(void)
			{
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
				return true;
			}
			
			void Draw(void)
			{
				EWOL_DEBUG("redraw (" << width << "," << height << ")");
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
				glXSwapBuffers(m_display, GLXWindowHandle);
			}
		
		public:
			X11systemInterface(void)
			{
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
					XConfigureEvent *xc;
					// main X boucle : 
					while (XPending(m_display)) {
						XNextEvent(m_display, &event);
						
						switch (event.type)
						{
							case ClientMessage:
								{
									Atom atom = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
									if(atom == event.xclient.data.l[0]) {
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
									EWOL_DEBUG("Change Windows Size : (" << event.xconfigure.width << "," << event.xconfigure.height << ")");
									m_uniqueWindows->CalculateSize((double)event.xconfigure.width, (double)event.xconfigure.height);
									break;
								case Expose:
									m_uniqueWindows->SysOnExpose();
									break;
								case ButtonPress:
									if ( event.xbutton.button & Button2 ) {
										m_uniqueWindows->GenEventInput(2, ewol::EVENT_INPUT_TYPE_DOWN, (double)event.xbutton.x, (double)event.xbutton.y);
									} else if (event.xbutton.button & Button1) {
										m_uniqueWindows->GenEventInput(1, ewol::EVENT_INPUT_TYPE_DOWN, (double)event.xbutton.x, (double)event.xbutton.y);
									}
									break;
								case ButtonRelease:
									if(event.xbutton.button & Button2) {
										m_uniqueWindows->GenEventInput(2, ewol::EVENT_INPUT_TYPE_UP, (double)event.xbutton.x, (double)event.xbutton.y);
									} else if (event.xbutton.button & Button1) {
										m_uniqueWindows->GenEventInput(1, ewol::EVENT_INPUT_TYPE_UP, (double)event.xbutton.x, (double)event.xbutton.y);
									}
									break;
								case EnterNotify:
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (double)event.xcrossing.x, (double)event.xcrossing.y);
									break;
								case MotionNotify:
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_MOVE, (double)event.xmotion.x, (double)event.xmotion.y);
									break;
								case LeaveNotify:
									m_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_LEAVE, (double)event.xcrossing.x, (double)event.xcrossing.y);
									break;
								case FocusIn:
									m_uniqueWindows->SetFocus();
									break;
								case FocusOut:
									m_uniqueWindows->RmFocus();
									break;
								case KeyPress:
								case KeyRelease:
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
									m_uniqueWindows->SysOnShow();
									break;
								case UnmapNotify:
									m_uniqueWindows->SysOnHide();
									break;
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
	
#endif



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


