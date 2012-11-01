/**
 *******************************************************************************
 * @file os/gui.X11.cpp
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
#include <ewol/ewol.h>
#include <etk/UString.h>
#include <etk/unicode.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/os/gui.h>

#include <ewol/ResourceManager.h>
#include <ewol/os/eSystem.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glx.h>
#include <X11/Xatom.h>
#include <sys/times.h>

/*
#define GUI_LOCK()          XLockDisplay(m_display)
#define GUI_UNLOCK()        XUnlockDisplay(m_display)
*/

//#define DEBUG_X11_EVENT

int64_t guiInterface::GetTime(void)
{
	struct timespec now;
	int ret = clock_gettime(CLOCK_REALTIME, &now);
	if (ret != 0) {
		// Error to get the time ...
		now.tv_sec = time(NULL);
		now.tv_nsec = 0;
	}
	//EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
	return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_nsec/(int64_t)1000);
}

#undef __class__
#define __class__	"guiInterface"

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

static ewol::specialKey_ts guiKeyBoardMode;


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

bool inputIsPressed[20];

// internal copy of the clipBoard ...
static bool                          l_clipBoardRequestPrimary = false;   // if false : request the copy/past buffer, if true : request current selection
static bool                          l_clipBoardOwnerPrimary = false; // we are the owner of the current selection
static bool                          l_clipBoardOwnerStd = false; // we are the owner of the current copy buffer
// Atom access...
static Atom XAtomeSelection = 0;
static Atom XAtomeClipBoard = 0;
static Atom XAtomeTargetString = 0;
static Atom XAtomeTargetStringUTF8 = 0;
static Atom XAtomeTargetTarget = 0;
static Atom XAtomeEWOL = 0;
static Atom XAtomeDeleteWindows = 0;


bool CreateX11Context(void)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: CreateX11Context");
	#endif
	int x,y, attr_mask;
	XSizeHints hints;
	XWMHints *StartupState;
	XTextProperty textprop;
	XSetWindowAttributes attr;
	// basic title of the windows ...
	static char *title = (char*)"Ewol";
	
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
	eSystem::Resize(tmp_width, tmp_height);
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
	
	etk::Vector2D<int32_t> tmpSize(400, 300);
	guiInterface::ChangeSize(tmpSize);
	
	return true;
}


/**
 * @brief Set the new title of the windows
 * @param title New desired title
 * @return ---
 */
void guiInterface::SetTitle(etk::UString& title)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: Set Title (START)");
	#endif
	XTextProperty tp;
	tp.value = (unsigned char *)title.c_str();
	tp.encoding = XA_WM_NAME;
	tp.format = 8;
	tp.nitems = strlen((const char*)tp.value);
	XSetWMName(m_display, WindowHandle, &tp);
	XStoreName(m_display, WindowHandle, (const char*)tp.value);
	XSetIconName(m_display, WindowHandle, (const char*)tp.value);
	XSetWMIconName(m_display, WindowHandle, &tp);
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: Set Title (END)");
	#endif
}

/* this variable will contain the ID of the newly created pixmap.    */
Pixmap icon_pixmap;

// TODO : I don't understand why it does not work ....
void SetIcon(etk::FSNode bitmapFile)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: try to set icon : " << bitmapFile);
	#endif
	// load bitmap
	if (false == bitmapFile.Exist()) {
		EWOL_ERROR("X11 Icon File does not Exist ... " << bitmapFile);
	} else {
		etk::UString fileExtention = bitmapFile.FileGetExtention();
		if (fileExtention ==  "bmp") {
			// pointer to the WM hints structure.
			XWMHints* win_hints;
			
			unsigned int bitmap_width, bitmap_height;
			int hotspot_x, hotspot_y;
			int rc = XReadBitmapFile(m_display, WindowHandle,
			                         "assets/iconEdn.xpm",
			                         &bitmap_width, &bitmap_height,
			                         &icon_pixmap,
			                         &hotspot_x, &hotspot_y);
			switch (rc) {
				case BitmapOpenFailed:
					EWOL_ERROR("XReadBitmapFile - could not open file ");
					return;
				case BitmapFileInvalid:
					EWOL_ERROR("XReadBitmapFile - file doesn't contain a valid bitmap.");
					return;
				case BitmapNoMemory:
					EWOL_ERROR("XReadBitmapFile - not enough memory.");
					return;
				case BitmapSuccess:
					/* bitmap loaded successfully - do something with it... */
					EWOL_INFO("XReadBitmapFile - bitmap loaded successfully.");
					break;
			}
			// allocate a WM hints structure.
			win_hints = XAllocWMHints();
			if (!win_hints) {
				EWOL_ERROR("XAllocWMHints - out of memory");
				return;
			}
			// initialize the structure appropriately. first, specify which size hints we want to fill in. in our case - setting the icon's pixmap.
			win_hints->flags = IconPixmapHint;
			// next, specify the desired hints data. in our case - supply the icon's desired pixmap.
			win_hints->icon_pixmap = icon_pixmap;
			// pass the hints to the window manager.
			XSetWMHints(m_display, WindowHandle, win_hints);
			EWOL_INFO("    ==> might be done ");
			// finally, we can free the WM hints structure.
			XFree(win_hints);
		} else {
			EWOL_ERROR("X11 Icon Extention not managed " << bitmapFile << " Sopported extention : .bmp ");
		}
	}
}


void RemoveDecoration(void)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11:RemoveDecoration");
	#endif
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
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11:AddDecoration");
	#endif
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
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11:CreateOGlContext");
	#endif
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

void X11_Init(void)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11:INIT");
	#endif
	if (m_doubleBuffered) {
		glXSwapBuffers(m_display, WindowHandle);
	}
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
	CreateX11Context();
	CreateOGlContext();
	// reset clipBoard
	l_clipBoardRequestPrimary = false;
	l_clipBoardOwnerPrimary = false;
	l_clipBoardOwnerStd = false;
	// reset the Atom properties ...
	XAtomeSelection        = XInternAtom(m_display, "PRIMARY", 0);
	XAtomeClipBoard        = XInternAtom(m_display, "CLIPBOARD", 0);
	XAtomeTargetString     = XInternAtom(m_display, "STRING", 0);
	XAtomeTargetStringUTF8 = XInternAtom(m_display, "UTF8_STRING", 0);
	XAtomeTargetTarget     = XInternAtom(m_display, "TARGETS", 0);
	XAtomeEWOL             = XInternAtom(m_display, "EWOL", 0);
	XAtomeDeleteWindows    = XInternAtom(m_display, "WM_DELETE_WINDOW", 0);
	
	m_run = true;
}


void X11_Run(void)
{
	// main cycle
	while(true == m_run) {
		//EWOL_ERROR("plop1");
		XEvent event;
		XEvent respond;
		// main X boucle :
		while (XPending(m_display)) {
			#ifdef DEBUG_X11_EVENT
				EWOL_INFO("X11:Event");
			#endif
			XNextEvent(m_display, &event);
			switch (event.type)
			{
				case ClientMessage:
					{
						#ifdef DEBUG_X11_EVENT
							EWOL_INFO("Receive : ClientMessage");
						#endif
						if(XAtomeDeleteWindows == (int64_t)event.xclient.data.l[0]) {
							EWOL_INFO("    ==> Kill Requested ...");
							eSystem::OnKill();
							m_run = false;
						}
					}
					break;
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//                               Selection AREA                                                              //
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////
				case SelectionClear:
					// Selection has been done on an other program ==> clear ours ...
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event SelectionClear");
					#endif
					{
						#ifdef DEBUG_X11_EVENT
						{
							XSelectionRequestEvent *req=&(event.xselectionrequest);
							char * atomNameProperty = XGetAtomName(m_display, req->property);
							char * atomNameTarget = XGetAtomName(m_display, req->target);
							EWOL_INFO("X11    property: \"" << atomNameProperty << "\"");
							EWOL_INFO("X11    target:   \"" << atomNameTarget << "\"");
							if (NULL != atomNameProperty) { XFree(atomNameProperty); }
							if (NULL != atomNameTarget) { XFree(atomNameTarget); }
						}
						#endif
						if (true == l_clipBoardOwnerPrimary) {
							l_clipBoardOwnerPrimary = false;
						} else if (true == l_clipBoardOwnerStd) {
							l_clipBoardOwnerStd = false;
						} else {
							EWOL_ERROR("X11 event SelectionClear ==> but no selection requested anymore ...");
						}
					}
					break;
				case SelectionNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event SelectionNotify");
					#endif
					if (event.xselection.property == None) {
						EWOL_VERBOSE("    ==> no data ...");
					} else {
						unsigned char *buf = 0;
						Atom type;
						int format;
						unsigned long nitems, bytes;
						XGetWindowProperty(m_display,
						                   WindowHandle,
						                   event.xselection.property,
						                   0, // offset
						                   (~0L), // length
						                   False, // delete
						                   AnyPropertyType, // reg_type
						                   &type,// *actual_type_return,
						                   &format,// *actual_format_return
						                   &nitems,// *nitems_return
						                   &bytes, // *bytes_after_return
						                   &buf// **prop_return);
						                   );
						if (true == l_clipBoardRequestPrimary) {
							etk::UString tmpppp((char*)buf);
							ewol::clipBoard::SetSystem(ewol::clipBoard::CLIPBOARD_SELECTION, tmpppp);
							// just transmit an event , we have the data in the system
							eSystem::ClipBoardArrive(ewol::clipBoard::CLIPBOARD_SELECTION);
						} else {
							etk::UString tmpppp((char*)buf);
							ewol::clipBoard::SetSystem(ewol::clipBoard::CLIPBOARD_STD, tmpppp);
							// just transmit an event , we have the data in the system
							eSystem::ClipBoardArrive(ewol::clipBoard::CLIPBOARD_STD);
						}
					}
					break;
				case SelectionRequest:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event SelectionRequest");
					#endif
					{
						XSelectionRequestEvent *req=&(event.xselectionrequest);
						#ifdef DEBUG_X11_EVENT
						{
							char * atomNameProperty = XGetAtomName(m_display, req->property);
							char * atomNameSelection = XGetAtomName(m_display, req->selection);
							char * atomNameTarget = XGetAtomName(m_display, req->target);
							EWOL_INFO("    from: " << atomNameProperty << "  request=" << atomNameSelection << " in " << atomNameTarget);
							if (NULL != atomNameProperty) { XFree(atomNameProperty); }
							if (NULL != atomNameSelection) { XFree(atomNameSelection); }
							if (NULL != atomNameTarget) { XFree(atomNameTarget); }
						}
						#endif
						const char * magatTextToSend = NULL;
						
						etk::UString tmpData = "";
						if (req->selection == XAtomeSelection) {
							tmpData = ewol::clipBoard::Get(ewol::clipBoard::CLIPBOARD_SELECTION);
						} else if (req->selection == XAtomeClipBoard) {
							tmpData = ewol::clipBoard::Get(ewol::clipBoard::CLIPBOARD_STD);
						}
						magatTextToSend = tmpData.c_str();
						Atom listOfAtom[4];
						if(strlen(magatTextToSend) == 0 ) {
							respond.xselection.property= None;
						} else if(XAtomeTargetTarget == req->target) {
							// We need to generate the list of the possibles target element of atom
							int32_t nbAtomSupported = 0;
							listOfAtom[nbAtomSupported++] = XAtomeTargetTarget;
							listOfAtom[nbAtomSupported++] = XAtomeTargetString;
							listOfAtom[nbAtomSupported++] = XAtomeTargetStringUTF8;
							listOfAtom[nbAtomSupported++] = None;
							XChangeProperty( m_display,
							                 req->requestor,
							                 req->property,
							                 XA_ATOM,
							                 32,
							                 PropModeReplace,
							                 (unsigned char*)listOfAtom,
							                 nbAtomSupported );
							respond.xselection.property=req->property;
							EWOL_INFO("            ==> Respond ... (test)");
						} else if(XAtomeTargetString == req->target) {
							XChangeProperty( m_display,
							                 req->requestor,
							                 req->property,
							                 req->target,
							                 8,
							                 PropModeReplace,
							                 (unsigned char*)magatTextToSend,
							                 strlen(magatTextToSend));
							respond.xselection.property=req->property;
							EWOL_INFO("            ==> Respond ...");
						} else if (XAtomeTargetStringUTF8 == req->target) {
							XChangeProperty( m_display,
							                 req->requestor,
							                 req->property,
							                 req->target,
							                 8,
							                 PropModeReplace,
							                 (unsigned char*)magatTextToSend,
							                 strlen(magatTextToSend));
							respond.xselection.property=req->property;
							EWOL_INFO("            ==> Respond ...");
						} else {
							respond.xselection.property= None;
						}
						respond.xselection.type= SelectionNotify;
						respond.xselection.display= req->display;
						respond.xselection.requestor= req->requestor;
						respond.xselection.selection=req->selection;
						respond.xselection.target= req->target;
						respond.xselection.time = req->time;
						XSendEvent (m_display, req->requestor,0,0,&respond);
						// Flush the message on the pipe ...
						XFlush (m_display);
					}
					break;
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////
				case Expose:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event Expose");
					#endif
					break;
				case GraphicsExpose:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event GraphicsExpose");
					#endif
					break;
				case NoExpose:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event NoExpose");
					#endif
					break;
				case CreateNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event CreateNotify");
					#endif
					break;
				case DestroyNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event DestroyNotify");
					#endif
					break;
				case GravityNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event GravityNotify");
					#endif
					break;
				case VisibilityNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event VisibilityNotify");
					#endif
					break;
				case CirculateNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event CirculateNotify");
					#endif
					break;
				case ReparentNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event ReparentNotify");
					#endif
					break;
				case PropertyNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event PropertyNotify");
					#endif
					break;
				case ConfigureNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event ConfigureNotify");
					#endif
					m_originX = event.xconfigure.x;
					m_originY = event.xconfigure.y;
					eSystem::Resize(event.xconfigure.width, event.xconfigure.height);
					break;
				case ButtonPress:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event ButtonPress");
					#endif
					m_cursorEventX = event.xbutton.x;
					m_cursorEventY = event.xbutton.y;
					if (event.xbutton.button < NB_MAX_INPUT) {
						inputIsPressed[event.xbutton.button] = true;
					}
					eSystem::SetMouseState(event.xbutton.button, true, (float)event.xbutton.x, (float)event.xbutton.y);
					break;
				case ButtonRelease:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event ButtonRelease");
					#endif
					m_cursorEventX = event.xbutton.x;
					m_cursorEventY = event.xbutton.y;
					if (event.xbutton.button < NB_MAX_INPUT) {
						inputIsPressed[event.xbutton.button] = false;
					}
					eSystem::SetMouseState(event.xbutton.button, false, (float)event.xbutton.x, (float)event.xbutton.y);
					break;
				case EnterNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event EnterNotify");
					#endif
					m_cursorEventX = event.xcrossing.x;
					m_cursorEventY = event.xcrossing.y;
					//EWOL_DEBUG("X11 event : " << event.type << " = \"EnterNotify\" (" << (float)event.xcrossing.x << "," << (float)event.xcrossing.y << ")");
					//gui_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (float)event.xcrossing.x, (float)event.xcrossing.y);
					break;
				case LeaveNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event LeaveNotify");
					#endif
					m_cursorEventX = event.xcrossing.x;
					m_cursorEventY = event.xcrossing.y;
					//EWOL_DEBUG("X11 event : " << event.type << " = \"LeaveNotify\" (" << (float)event.xcrossing.x << "," << (float)event.xcrossing.y << ")");
					break;
				case MotionNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event MotionNotify");
					#endif
					m_cursorEventX = event.xmotion.x;
					m_cursorEventY = event.xmotion.y;
					{
						// For compatibility of the Android system : 
						bool findOne = false;
						for (int32_t iii=0; iii<NB_MAX_INPUT ; iii++) {
							if (true == inputIsPressed[iii]) {
								EWOL_VERBOSE("X11 event: bt=" << iii << " " << event.type << " = \"MotionNotify\" (" << (float)event.xmotion.x << "," << (float)event.xmotion.y << ")");
								eSystem::SetMouseMotion(iii, (float)event.xmotion.x, (float)event.xmotion.y);
								findOne = true;
							}
						}
						if (false == findOne) {
							EWOL_VERBOSE("X11 event: bt=" << 0 << " " << event.type << " = \"MotionNotify\" (" << (float)event.xmotion.x << "," << (float)event.xmotion.y << ")");
							eSystem::SetMouseMotion(0, (float)event.xmotion.x, (float)event.xmotion.y);
						}
					}
					break;
				case FocusIn:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event FocusIn");
					#endif
					break;
				case FocusOut:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event : FocusOut");
					#endif
					break;
				case KeyPress:
				case KeyRelease:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event : " << event.type << " = \"KeyPress/KeyRelease\" ");
					#endif
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
							case 66:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CAPLOCK;		guiKeyBoardMode.capLock = (event.type == KeyPress) ? true : false; break;
							case 50:	keyInput = ewol::EVENT_KB_MOVE_TYPE_SHIFT_LEFT;		guiKeyBoardMode.shift   = (event.type == KeyPress) ? true : false; break;
							case 62:	keyInput = ewol::EVENT_KB_MOVE_TYPE_SHIFT_RIGHT;	guiKeyBoardMode.shift   = (event.type == KeyPress) ? true : false; break;
							case 37:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CTRL_LEFT;		guiKeyBoardMode.ctrl    = (event.type == KeyPress) ? true : false; break;
							case 105:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CTRL_RIGHT;		guiKeyBoardMode.ctrl    = (event.type == KeyPress) ? true : false; break;
							case 133:	keyInput = ewol::EVENT_KB_MOVE_TYPE_META_LEFT;		guiKeyBoardMode.meta    = (event.type == KeyPress) ? true : false; break;
							case 134:	keyInput = ewol::EVENT_KB_MOVE_TYPE_META_RIGHT;		guiKeyBoardMode.meta    = (event.type == KeyPress) ? true : false; break;
							case 64:	keyInput = ewol::EVENT_KB_MOVE_TYPE_ALT;			guiKeyBoardMode.alt     = (event.type == KeyPress) ? true : false; break;
							case 108:	keyInput = ewol::EVENT_KB_MOVE_TYPE_ALT_GR;			guiKeyBoardMode.altGr   = (event.type == KeyPress) ? true : false; break;
							case 135:	keyInput = ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU;	break;
							case 77:	keyInput = ewol::EVENT_KB_MOVE_TYPE_VER_NUM;		guiKeyBoardMode.verNum  = (event.type == KeyPress) ? true : false; break;
							case 91: // Suppr on keypad
								find = false;
								{
									eSystem::keyboardKey_ts specialEvent;
									specialEvent.special = guiKeyBoardMode;
									specialEvent.myChar = 0x0000007F;
									if(event.type == KeyPress) {
										specialEvent.isDown = true;
									} else {
										specialEvent.isDown = false;
									}
									eSystem::SetKeyboard(specialEvent);
								}
								break;
							case 23: // special case for TAB
								find = false;
								{
									eSystem::keyboardKey_ts specialEvent;
									specialEvent.special = guiKeyBoardMode;
									specialEvent.myChar = 0x00000009;
									if(event.type == KeyPress) {
										specialEvent.isDown = true;
									} else {
										specialEvent.isDown = false;
									}
									eSystem::SetKeyboard(specialEvent);
								}
								break;
							default:
								find = false;
								{
									char buf[11];
									//EWOL_DEBUG("Keycode: " << event.xkey.keycode);
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
										eSystem::keyboardKey_ts specialEvent;
										specialEvent.special = guiKeyBoardMode;
										unicode::convertIsoToUnicode(unicode::EDN_CHARSET_ISO_8859_15, buf[0], specialEvent.myChar);
										//EWOL_INFO("event Key : " << event.xkey.keycode << " char=\"" << buf << "\"'len=" << strlen(buf) << " unicode=" << unicodeValue);
										if(event.type == KeyPress) {
											specialEvent.isDown = true;
										} else {
											specialEvent.isDown = false;
										}
										eSystem::SetKeyboard(specialEvent);
									} else {
										EWOL_WARNING("Unknow event Key : " << event.xkey.keycode);
									}
								}
								break;
						}
						if (true == find) {
							//EWOL_DEBUG("eventKey Move type : " << GetCharTypeMoveEvent(keyInput) );
							eSystem::keyboardMove_ts specialEvent;
							specialEvent.special = guiKeyBoardMode;
							if(event.type == KeyPress) {
								specialEvent.isDown = true;
							} else {
								specialEvent.isDown = false;
							}
							specialEvent.move = keyInput;
							eSystem::SetKeyboardMove(specialEvent);
						}
					}
					break;
				//case DestroyNotify:
				//	break;
				case MapNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event : MapNotify");
					#endif
					eSystem::Show();
					break;
				case UnmapNotify:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event : UnmapNotify");
					#endif
					eSystem::Hide();
					break;
				default:
					#ifdef DEBUG_X11_EVENT
						EWOL_INFO("X11 event : " << event.type << " = \"???\"");
					#endif
					break;
			}
		}
		if(true == m_run) {
			(void)eSystem::Draw(false);
			if (m_doubleBuffered) {
				glXSwapBuffers(m_display, WindowHandle);
			}
		}
		#ifdef DEBUG_X11_EVENT
			//EWOL_INFO("X11 endEvent --- ");
		#endif
	}
};


// -------------------------------------------------------------------------
//         ClipBoard AREA :
// -------------------------------------------------------------------------

/**
 * @brief Inform the Gui that we want to have a copy of the clipboard
 * @param ID of the clipboard (STD/SELECTION) only apear here
 * @return ---
 */
void guiInterface::ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	switch (clipboardID)
	{
		case ewol::clipBoard::CLIPBOARD_SELECTION:
			if (false == l_clipBoardOwnerPrimary) {
				l_clipBoardRequestPrimary = true;
				// Generate a request on X11
				XConvertSelection(m_display,
				                  XAtomeSelection,
				                  XAtomeTargetStringUTF8,
				                  XAtomeEWOL,
				                  WindowHandle,
				                  CurrentTime);
			} else {
				// just transmit an event , we have the data in the system
				eSystem::ClipBoardArrive(clipboardID);
			}
			break;
		case ewol::clipBoard::CLIPBOARD_STD:
			if (false == l_clipBoardOwnerStd) {
				l_clipBoardRequestPrimary = false;
				// Generate a request on X11
				XConvertSelection(m_display,
				                  XAtomeClipBoard,
				                  XAtomeTargetStringUTF8,
				                  XAtomeEWOL,
				                  WindowHandle,
				                  CurrentTime);
			} else {
				// just transmit an event , we have the data in the system
				eSystem::ClipBoardArrive(clipboardID);
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}

/**
 * @brief Inform the Gui that we are the new owner of the clipboard
 * @param ID of the clipboard (STD/SELECTION) only apear here
 * @return ---
 */
void guiInterface::ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	switch (clipboardID)
	{
		case ewol::clipBoard::CLIPBOARD_SELECTION:
			// Request the selection :
			if (false == l_clipBoardOwnerPrimary) {
				XSetSelectionOwner(m_display, XAtomeSelection, WindowHandle, CurrentTime);
				l_clipBoardOwnerPrimary = true;
			}
			break;
		case ewol::clipBoard::CLIPBOARD_STD:
			// Request the clipBoard :
			if (false == l_clipBoardOwnerStd) {
				XSetSelectionOwner(m_display, XAtomeClipBoard, WindowHandle, CurrentTime);
				l_clipBoardOwnerStd = true;
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}



#undef __class__
#define __class__ "guiInterface"


/**
 * @brief Stop the current program
 * @param ---
 * @return ---
 */
void guiInterface::Stop(void)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: Stop");
	#endif
	m_run = false;
}

/**
 * @brief Display the virtal keyboard (for touch system only)
 * @param ---
 * @return ---
 */
void guiInterface::KeyboardShow(void)
{
	// nothing to do : No keyboard on computer ...
}


/**
 * @brief Hide the virtal keyboard (for touch system only)
 * @param ---
 * @return ---
 */
void guiInterface::KeyboardHide(void)
{
	// nothing to do : No keyboard on computer ...
}


/**
 * @brief Change the current Windows size
 * @param size The requested size
 * @return ---
 */
void guiInterface::ChangeSize(etk::Vector2D<int32_t> size)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: ChangeSize");
	#endif
	XResizeWindow(m_display, WindowHandle, size.x, size.y);
}


/**
 * @brief Change the current Windows position
 * @param pos The position where the winsdows might be placed.
 * @return ---
 */
void guiInterface::ChangePos(etk::Vector2D<int32_t> pos)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: ChangePos");
	#endif
	XMoveWindow(m_display, WindowHandle, pos.x, pos.y);
}


/**
 * @brief Get the current Windows position
 * @param pos The position where the winsdows is.
 * @return ---
 */
void guiInterface::GetAbsPos(etk::Vector2D<int32_t>& pos)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("X11: GetAbsPos");
	#endif
	int tmp;
	unsigned int tmp2;
	Window fromroot, tmpwin;
	XQueryPointer(m_display, WindowHandle, &fromroot, &tmpwin, &pos.x, &pos.y, &tmp, &tmp, &tmp2);
}


/**
 * @brief Main of the program
 * @param std IO
 * @return std IO
 */
int main(int argc, char *argv[])
{
	ewol::CmdLine::Clean();
	for( int32_t i=1 ; i<argc; i++) {
		EWOL_INFO("CmdLine : \"" << argv[i] << "\"" );
		if (0==strncmp("-l0", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_NONE);
		} else if (0==strncmp("-l1", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_CRITICAL);
		} else if (0==strncmp("-l2", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_ERROR);
		} else if (0==strncmp("-l3", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_WARNING);
		} else if (0==strncmp("-l4", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_INFO);
		} else if (0==strncmp("-l5", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_DEBUG);
		} else if (0==strncmp("-l6", argv[i], 256)) {
			GeneralDebugSetLevel(etk::LOG_LEVEL_VERBOSE);
		} else {
			etk::UString tmpString(argv[i]);
			ewol::CmdLine::Add(tmpString);
		}
	}
	
	for (int32_t iii=0; iii<NB_MAX_INPUT; iii++) {
		inputIsPressed[iii] = false;
	}
	guiKeyBoardMode.capLock = false;
	guiKeyBoardMode.shift = false;
	guiKeyBoardMode.ctrl = false;
	guiKeyBoardMode.meta = false;
	guiKeyBoardMode.alt = false;
	guiKeyBoardMode.altGr = false;
	guiKeyBoardMode.verNum = false;
	guiKeyBoardMode.insert = false;
	
	// start X11 thread ...
	X11_Init();
	//start the basic thread : 
	eSystem::Init();
	// get the icon file : 
	etk::FSNode myIcon = APP_Icon();
	SetIcon(myIcon);
	
	// Run ...
	X11_Run();
	// close X11 :
	guiInterface::Stop();
	// uninit ALL :
	eSystem::UnInit();
	ewol::CmdLine::Clean();
	return 0;
}

/**
 * @brief Force the screen orientation (availlable on portable elements ...
 * @param orientation Selected orientation.
 * @return ---
 */
void guiInterface::ForceOrientation(ewol::orientation_te orientation)
{
	// nothing to do ...
}
