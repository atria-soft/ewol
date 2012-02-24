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
#include <etk/UString.h>
#include <etk/unicode.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>

#include <ewol/Texture.h>
#include <ewol/base/MainThread.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glx.h>
#include <X11/Xatom.h>
#include <sys/times.h>


int64_t GetCurrentTime(void)
{
	struct timespec now;
	int ret = clock_gettime(CLOCK_REALTIME, &now);
	if (ret != 0) {
		// Error to get the time ...
		now.tv_sec = time(NULL);
		now.tv_nsec = 0;
	}
	//EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
	return (int64_t)((int64_t)now.tv_sec*(int64_t)1000 + (int64_t)now.tv_nsec/(int64_t)1000000);
}

#undef __class__
#define __class__	"guiAbstraction"

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

static eventSpecialKey_ts guiKeyBoardMode;


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

// for double and triple click selection, we need to save the previous click up and down position , and the previous time ...
int32_t m_previousBouttonId = -1;
int32_t m_previousDown_x = 0;
int32_t m_previousDown_y = 0;
int32_t m_previous_x = 0;
int32_t m_previous_y = 0;
int64_t m_previousTime = 0;
bool    m_previousDouble = 0;

Atom           m_delAtom;
Display *      m_display = NULL;;
Window         WindowHandle;
int32_t        m_originX = 0;
int32_t        m_originY = 0;
int32_t        m_cursorEventX = 0;
int32_t        m_cursorEventY = 0;
XVisualInfo *  m_visual = NULL;
bool           m_doubleBuffered = 0;
bool           m_run = 0;
extern ewol::Windows* gui_uniqueWindows;
extern etkFloat_t     gui_width;
extern etkFloat_t     gui_height;

int32_t separateClickTime = 300;
int32_t offsetMoveClicked = 10;
int32_t offsetMoveClickedDouble = 20;


bool inputIsPressed[20];


static void X11_ChangeSize(int32_t w, int32_t h);
static void X11_ChangePos(int32_t x, int32_t y);
static void X11_GetAbsPos(int32_t & x, int32_t & y);

bool CreateX11Context(void)
{
	int x,y, attr_mask;
	XSizeHints hints;
	XWMHints *StartupState;
	XTextProperty textprop;
	XSetWindowAttributes attr;
	static char *title = (char*)"APPLICATION Title ... (todo)";
	
	EWOL_INFO("X11 Mode XF86 Video");
	
	// Connect to the X server
	m_display = XOpenDisplay(NULL);
	if(NULL == m_display) {
		EWOL_CRITICAL("Could not open display X.");
		exit(-1);
	} else {
		EWOL_INFO("Display opened.");
	}
	int Xscreen = DefaultScreen(m_display);
	// get an appropriate visual
	m_visual = glXChooseVisual(m_display, Xscreen, attrListDbl);
	if (NULL == m_visual) {
		m_visual = glXChooseVisual(m_display, Xscreen, attrListSgl);
		m_doubleBuffered = false;
		EWOL_INFO("GL-X singlebuffered rendering will be used, no doublebuffering available");
	} else {
		m_doubleBuffered = true;
		EWOL_INFO("GL-X doublebuffered rendering available");
	}
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
	int32_t tmp_width = DisplayWidth(m_display, DefaultScreen(m_display))/2;
	int32_t tmp_height = DisplayHeight(m_display, DefaultScreen(m_display))/2;
	EWOL_NativeResize(tmp_width, tmp_height);
	x=tmp_width/2;
	y=tmp_height/4;
	
	// Real create of the window
	WindowHandle = XCreateWindow(m_display,
	                             Xroot,
	                             x, y, tmp_width, tmp_height,
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
	hints.width = tmp_width;
	hints.height = tmp_height;
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
	
	X11_ChangeSize(400, 300);
	
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
	/* create a GLX context */
	GLXContext RenderContext = glXCreateContext(m_display, m_visual, 0, GL_TRUE);
	/* connect the glx-context to the window */
	glXMakeCurrent(m_display, WindowHandle, RenderContext);
	if (glXIsDirect(m_display, RenderContext)) {
		EWOL_INFO("XF86 DRI enabled\n");
	} else {
		EWOL_INFO("XF86 DRI NOT available\n");
	}
	return true;
}

void EWOL_NativeRender(void)
{
	ewol::UpdateTextureContext();
	//EWOL_DEBUG("redraw (" << gui_width << "," << gui_height << ")");
	if(NULL == gui_uniqueWindows) {
		//EWOL_DEBUG("Has No Windows set...");
		
		// set the size of the open GL system
		glViewport(0,0,gui_width,gui_height);
		
		// Clear the screen with transparency ...
		glClearColor(0.750, 0.750, 0.750, 0.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0., (etkFloat_t)gui_width, 0., (etkFloat_t)gui_height, 1., 20.);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -5);
		
		glBegin(GL_QUADS);
			glColor3f(1., 0., 0.); glVertex3f( .25*(etkFloat_t)gui_width, .25*(etkFloat_t)gui_height, 0.);
			glColor3f(0., 1., 0.); glVertex3f( .75*(etkFloat_t)gui_width, .25*(etkFloat_t)gui_height, 0.);
			glColor3f(0., 0., 1.); glVertex3f( .75*(etkFloat_t)gui_width, .75*(etkFloat_t)gui_height, 0.);
			glColor3f(1., 1., 0.); glVertex3f( .25*(etkFloat_t)gui_width, .75*(etkFloat_t)gui_height, 0.);
		glEnd();
	} else {
		EWOL_GenericDraw(false);
	}
	glFlush();
	if (m_doubleBuffered) {
		glXSwapBuffers(m_display, WindowHandle);
	}
}

void X11_Init(void)
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


void X11_Run(void)
{
	// main cycle
	while(true == m_run) {
		//EWOL_ERROR("plop1");
		XEvent event;
		// main X boucle :
		while (XPending(m_display)) {
			//EWOL_ERROR("plop 22222");
			XNextEvent(m_display, &event);
			
			switch (event.type)
			{
				case ClientMessage:
					{
						Atom atom = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
						if((int64_t)atom == (int64_t)event.xclient.data.l[0]) {
							if (NULL != gui_uniqueWindows) {
								gui_uniqueWindows->SysOnKill();
							}
							m_run = false;
						}
					}
					break;
				case Expose:
					EWOL_DEBUG("X11 event Expose");
					break;
				case GraphicsExpose:
					EWOL_DEBUG("X11 event GraphicsExpose");
					break;
				case NoExpose:
					EWOL_DEBUG("X11 event NoExpose");
					break;
				case CreateNotify:
					EWOL_DEBUG("X11 event CreateNotify");
					break;
				case DestroyNotify:
					EWOL_DEBUG("X11 event DestroyNotify");
					break;
				case GravityNotify:
					EWOL_DEBUG("X11 event GravityNotify");
					break;
				case VisibilityNotify:
					EWOL_DEBUG("X11 event VisibilityNotify");
					break;
				case CirculateNotify:
					EWOL_DEBUG("X11 event CirculateNotify");
					break;
				case ReparentNotify:
					EWOL_DEBUG("X11 event ReparentNotify");
					break;
				case PropertyNotify:
					EWOL_DEBUG("X11 event PropertyNotify");
					break;
				case ConfigureNotify:
					m_originX = event.xconfigure.x;
					m_originY = event.xconfigure.y;
					EWOL_ThreadResize(event.xconfigure.width, event.xconfigure.height);
					break;
				case ButtonPress:
					m_cursorEventX = event.xbutton.x;
					m_cursorEventY = event.xbutton.y;
					if (event.xbutton.button < NB_MAX_INPUT) {
						inputIsPressed[event.xbutton.button] = true;
					}
					EWOL_ThreadEventInputState(event.xbutton.button, true, (float)event.xbutton.x, (float)event.xbutton.y);
					break;
				case ButtonRelease:
					m_cursorEventX = event.xbutton.x;
					m_cursorEventY = event.xbutton.y;
					if (event.xbutton.button < NB_MAX_INPUT) {
						inputIsPressed[event.xbutton.button] = false;
					}
					EWOL_ThreadEventInputState(event.xbutton.button, false, (float)event.xbutton.x, (float)event.xbutton.y);
					break;
				case EnterNotify:
					m_cursorEventX = event.xcrossing.x;
					m_cursorEventY = event.xcrossing.y;
					//EWOL_DEBUG("X11 event : " << event.type << " = \"EnterNotify\" (" << (etkFloat_t)event.xcrossing.x << "," << (etkFloat_t)event.xcrossing.y << ")");
					//gui_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (etkFloat_t)event.xcrossing.x, (etkFloat_t)event.xcrossing.y);
					break;
				case LeaveNotify:
					m_cursorEventX = event.xcrossing.x;
					m_cursorEventY = event.xcrossing.y;
					//EWOL_DEBUG("X11 event : " << event.type << " = \"LeaveNotify\" (" << (etkFloat_t)event.xcrossing.x << "," << (etkFloat_t)event.xcrossing.y << ")");
					break;
				case MotionNotify:
					m_cursorEventX = event.xmotion.x;
					m_cursorEventY = event.xmotion.y;
					{
						// For compatibility of the Android system : 
						bool findOne = false;
						for (int32_t iii=0; iii<NB_MAX_INPUT ; iii++) {
							if (true == inputIsPressed[iii]) {
								EWOL_VERBOSE("X11 event: bt=" << iii << " " << event.type << " = \"MotionNotify\" (" << (etkFloat_t)event.xmotion.x << "," << (etkFloat_t)event.xmotion.y << ")");
								EWOL_ThreadEventInputMotion(iii, (float)event.xmotion.x, (float)event.xmotion.y);
								findOne = true;
							}
						}
						if (false == findOne) {
							EWOL_VERBOSE("X11 event: bt=" << 0 << " " << event.type << " = \"MotionNotify\" (" << (etkFloat_t)event.xmotion.x << "," << (etkFloat_t)event.xmotion.y << ")");
							EWOL_ThreadEventInputMotion(0, (float)event.xmotion.x, (float)event.xmotion.y);
						}
					}
					break;
				case FocusIn:
					EWOL_VERBOSE("X11 event : " << event.type << " = \"FocusIn\"");
					break;
				case FocusOut:
					EWOL_VERBOSE("X11 event : " << event.type << " = \"FocusOut\"");
					break;
				case KeyPress:
				case KeyRelease:
					//EWOL_DEBUG("X11 event : " << event.type << " = \"KeyPress/KeyRelease\" ");
					{
						EWOL_DEBUG("eventKey : " << event.xkey.keycode << " state : " << event.xkey.state);
						if (event.xkey.state & (1<<0) ) {
							//EWOL_DEBUG("    Special Key : SHIFT");
							guiKeyBoardMode.shift = true;
						} else {
							guiKeyBoardMode.shift = false;
						}
						if (event.xkey.state & (1<<1) ) {
							//EWOL_DEBUG("    Special Key : CAPS_LOCK");
							guiKeyBoardMode.capLock = true;
						} else {
							guiKeyBoardMode.capLock = false;
						}
						if (event.xkey.state & (1<<2) ) {
							//EWOL_DEBUG("    Special Key : Ctrl");
							guiKeyBoardMode.ctrl = true;
						} else {
							guiKeyBoardMode.ctrl = false;
						}
						if (event.xkey.state & (1<<3) ) {
							//EWOL_DEBUG("    Special Key : Alt");
							guiKeyBoardMode.alt = true;
						} else {
							guiKeyBoardMode.alt = false;
						}
						if (event.xkey.state & (1<<4) ) {
							//EWOL_DEBUG("    Special Key : VER_num");
							guiKeyBoardMode.verNum = true;
						} else {
							guiKeyBoardMode.verNum = false;
						}
						if (event.xkey.state & (1<<5) ) {
							EWOL_DEBUG("    Special Key : MOD");
						}
						if (event.xkey.state & (1<<6) ) {
							//EWOL_DEBUG("    Special Key : META");
							guiKeyBoardMode.meta = true;
						} else {
							guiKeyBoardMode.meta = false;
						}
						if (event.xkey.state & (1<<7) ) {
							//EWOL_DEBUG("    Special Key : ALT_GR");
							guiKeyBoardMode.altGr = true;
						} else {
							guiKeyBoardMode.altGr = false;
						}
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
							case 118:
								keyInput = ewol::EVENT_KB_MOVE_TYPE_INSERT;
								if(event.type == KeyRelease) {
									if (true == guiKeyBoardMode.insert) {
										guiKeyBoardMode.insert = false;
									} else {
										guiKeyBoardMode.insert = true;
									}
								}
								break;
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
									eventKeyboardKey_ts specialEvent;
									specialEvent.special = guiKeyBoardMode;
									specialEvent.myChar = 0x0000007F;
									if(event.type == KeyPress) {
										specialEvent.isDown = true;
									} else {
										specialEvent.isDown = false;
									}
									EWOL_ThreadKeyboardEvent(specialEvent);
								}
							default:
								find = false;
								{
									char buf[11];
									EWOL_DEBUG("Keycode: " << event.xkey.keycode);
									// change keystate for simple reson of the ctrl error...
									int32_t keyStateSave = event.xkey.state;
									if (event.xkey.state & (1<<2) ) {
										event.xkey.state = event.xkey.state & 0xFFFFFFFB;
									}
									KeySym keysym;
									XComposeStatus status;
									int count = XLookupString(&event.xkey, buf, 10, &keysym, &status);
									// retreave real keystate
									event.xkey.state = keyStateSave;
									buf[count] = '\0';
									// Replace \r error ...
									if (buf[0] == '\r') {
										buf[0] = '\n';
										buf[1] = '\0';
									}
									if (count>0) {
										// transform iun unicode
										eventKeyboardKey_ts specialEvent;
										specialEvent.special = guiKeyBoardMode;
										unicode::convertIsoToUnicode(unicode::EDN_CHARSET_ISO_8859_15, buf[0], specialEvent.myChar);
										//EWOL_INFO("event Key : " << event.xkey.keycode << " char=\"" << buf << "\"'len=" << strlen(buf) << " unicode=" << unicodeValue);
										if(event.type == KeyPress) {
											specialEvent.isDown = true;
										} else {
											specialEvent.isDown = false;
										}
										EWOL_ThreadKeyboardEvent(specialEvent);
									} else {
										EWOL_WARNING("Unknow event Key : " << event.xkey.keycode);
									}
								}
								break;
						}
						if (true == find) {
							EWOL_DEBUG("eventKey Move type : " << GetCharTypeMoveEvent(keyInput) );
							eventKeyboardMove_ts specialEvent;
							specialEvent.special = guiKeyBoardMode;
							if(event.type == KeyPress) {
								specialEvent.isDown = true;
							} else {
								specialEvent.isDown = false;
							}
							specialEvent.move = keyInput;
							EWOL_ThreadKeyboardEventMove(specialEvent);
						}
						break;
					}
				//case DestroyNotify:
				//	break;
				case MapNotify:
					EWOL_VERBOSE("X11 event : " << event.type << " = \"MapNotify\"");
					EWOL_ThreadEventShow();
					break;
				case UnmapNotify:
					EWOL_VERBOSE("X11 event : " << event.type << " = \"UnmapNotify\"");
					EWOL_ThreadEventHide();
					break;
				default:
					EWOL_DEBUG("X11 event : " << event.type << " = \"???\"");
			}
		}
		EWOL_NativeRender();
	}
};

void X11_ChangeSize(int32_t w, int32_t h)
{
	XResizeWindow(m_display, WindowHandle, w, h);
};

void X11_ChangePos(int32_t x, int32_t y)
{
	XMoveWindow(m_display, WindowHandle, x, y);
};

void X11_GetAbsPos(int32_t & x, int32_t & y)
{
	int tmp;
	unsigned int tmp2;
	Window fromroot, tmpwin;
	XQueryPointer(m_display, WindowHandle, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp2);
};





#undef __class__
#define __class__ "guiAbstraction"


void guiAbstraction::Stop(void)
{
	m_run = false;
}




void guiAbstraction::ChangeSize(int32_t w, int32_t h)
{
	X11_ChangeSize(w, h);
}

void guiAbstraction::ChangePos(int32_t x, int32_t y)
{
	X11_ChangePos(x, y);
}

void guiAbstraction::GetAbsPos(int32_t & x, int32_t & y)
{
	X11_GetAbsPos(x, y);
}

bool guiAbstraction::IsPressedInput(int32_t inputID)
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

#include <ewol/ewol.h>

static etk::VectorType<etk::UString*> listArgs;

int32_t ewol::CmdLineNb(void)
{
	return listArgs.Size();
}

etk::UString ewol::CmdLineGet(int32_t id)
{
	if (id<0 && id>=listArgs.Size()) {
		return "";
	}
	if (NULL == listArgs[id]) {
		return "";
	}
	return *listArgs[id];
}


int main(int argc, char *argv[])
{
	for( int32_t i=1 ; i<argc; i++) {
		EWOL_INFO("CmdLine : \"" << argv[i] << "\"" );
		etk::UString* tmpString = new etk::UString(argv[i]);
		if (NULL != tmpString) {
			listArgs.PushBack(tmpString);
		}
	}
	// start X11 thread ...
	X11_Init();
	//start the basic thread : 
	EWOL_SystemStart();
	usleep(500);
	// Run ...
	X11_Run();
	// close X11 :
	guiAbstraction::Stop();
	// uninit ALL :
	EWOL_SystemStop();
	for (int32_t iii=0; iii<listArgs.Size(); iii++) {
		if (NULL != listArgs[iii]) {
			delete listArgs[iii];
			listArgs[iii] = NULL;
		}
	}
	listArgs.Clear();
	return 0;
}

