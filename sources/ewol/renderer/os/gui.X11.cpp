/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/key.h>
#include <ewol/config.h>
#include <ewol/commandLine.h>
#include <etk/UString.h>
#include <etk/unicode.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/os/gui.h>

#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/os/eSystem.h>

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
#ifdef DEBUG_X11_EVENT
	#define X11_DEBUG      EWOL_DEBUG
	#define X11_VERBOSE    EWOL_VERBOSE
	#define X11_INFO       EWOL_INFO
	#define X11_CRITICAL   EWOL_CRITICAL
#else
	#define X11_DEBUG      EWOL_VERBOSE
	#define X11_VERBOSE    EWOL_VERBOSE
	#define X11_INFO       EWOL_VERBOSE
	#define X11_CRITICAL   EWOL_VERBOSE
#endif
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

static ewol::SpecialKey guiKeyBoardMode;


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
int32_t        m_currentHeight = 0;
int32_t        m_currentWidth = 0;
XVisualInfo *  m_visual = NULL;
bool           m_doubleBuffered = 0;
bool           m_run = 0;
//forcing the position
bool           m_grabAllEvent = false;    //!< grab mode enable...
vec2           m_forcePos = vec2(0,0);    //!< position to reset the cursor
bool           m_positionChangeRequested = false; //!< the position modifiquation has been requested
vec2           m_curentGrabDelta = vec2(0,0); //!< the position in X11 will arrive by pool

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
	X11_INFO("X11: CreateX11Context");
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
	int32_t tmp_width = 640;//DisplayWidth(m_display, DefaultScreen(m_display))/2;
	int32_t tmp_height = 480;//DisplayHeight(m_display, DefaultScreen(m_display))/2;
	eSystem::Resize(tmp_width, tmp_height);
	x=20;
	y=20;
	m_originX = x;
	m_originY = y;
	EWOL_INFO("X11 request creating windows at pos=(" << m_originX << "," << m_originY << ") size=(" << tmp_width << "," << tmp_height << ")" );
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
	
	ivec2 tmpSize(400, 300);
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
	X11_INFO("X11: Set Title (START)");
	XTextProperty tp;
	tp.value = (unsigned char *)title.c_str();
	tp.encoding = XA_WM_NAME;
	tp.format = 8;
	tp.nitems = strlen((const char*)tp.value);
	XSetWMName(m_display, WindowHandle, &tp);
	XStoreName(m_display, WindowHandle, (const char*)tp.value);
	XSetIconName(m_display, WindowHandle, (const char*)tp.value);
	XSetWMIconName(m_display, WindowHandle, &tp);
	X11_INFO("X11: Set Title (END)");
}

#include <ewol/renderer/resources/image/ImageBMP.h>
#include <ewol/renderer/resources/image/ImagePNG.h>
#include <parserSVG/parserSVG.h>

void SetIcon(etk::UString inputFile)
{
	draw::Image dataImage;
	// load data
	if (true == inputFile.EndWith(".bmp") ) {
		// generate the texture
		if (false == ewol::imageBMP::GenerateImage(inputFile, dataImage)) {
			EWOL_ERROR("Error To load BMP file " << inputFile );
			return;
		}
	} else if (true == inputFile.EndWith(".svg") ) {
		svg::Parser m_element(inputFile);
		if (false == m_element.IsLoadOk()) {
			EWOL_ERROR("Error To load SVG file " << inputFile );
			return;
		} else {
			// generate the texture
			ivec2 tmpSize(32,32);
			m_element.GenerateAnImage(tmpSize, dataImage);
		}
	} else if (true == inputFile.EndWith(".png") ) {
		// generate the texture
		if (false == ewol::imagePNG::GenerateImage(inputFile, dataImage)) {
			EWOL_ERROR("Error To load PNG file " << inputFile );
			return;
		}
	} else {
		EWOL_ERROR("Extention not managed " << inputFile << " Sopported extention : .bmp / .svg / .png");
		return;
	}
	int32_t depth = DefaultDepth(m_display, DefaultScreen(m_display) );
	EWOL_DEBUG("X11 Create icon Size=(" << dataImage.GetWidth() << "," << dataImage.GetHeight() << ") depth=" << depth);
	switch(depth) {
		case 8:
			EWOL_CRITICAL("Not manage pixmap in 8 bit... ==> no icon ...");
			return;
		case 16:
			break;
		case 24:
			break;
		case 32:
			break;
		default:
			EWOL_CRITICAL("Unknow thys type of bitDepth : " << depth);
			return;
	}
	char* tmpVal = new char[4*dataImage.GetWidth()*dataImage.GetHeight()];
	if (NULL == tmpVal) {
		EWOL_CRITICAL("Allocation error ...");
		return;
	}
	char* tmpPointer = tmpVal;
	switch(depth) {
		case 16:
			for(ivec2 pos(0,0); pos.y<dataImage.GetHeight(); pos.y++) {
				for(pos.x=0; pos.x<dataImage.GetHeight(); pos.x++) {
					draw::Color tmpColor = dataImage.Get(pos);
					int16_t tmpVal =   ((uint16_t)((uint16_t)tmpColor.r>>3))<<11
					                 + ((uint16_t)((uint16_t)tmpColor.g>>2))<<5
					                 + ((uint16_t)((uint16_t)tmpColor.b>>3));
					*tmpPointer++ = (uint8_t)(tmpVal>>8);
					*tmpPointer++ = (uint8_t)(tmpVal&0x00FF);
				}
			}
			break;
		case 24:
			for(ivec2 pos(0,0); pos.y<dataImage.GetHeight(); pos.y++) {
				for(pos.x=0; pos.x<dataImage.GetHeight(); pos.x++) {
					draw::Color tmpColor = dataImage.Get(pos);
					*tmpPointer++ = tmpColor.b;
					*tmpPointer++ = tmpColor.g;
					*tmpPointer++ = tmpColor.r;
					tmpPointer++;
				}
			}
			break;
		case 32:
			for(ivec2 pos(0,0); pos.y<dataImage.GetHeight(); pos.y++) {
				for(pos.x=0; pos.x<dataImage.GetHeight(); pos.x++) {
					draw::Color tmpColor = dataImage.Get(pos);
					*tmpPointer++ = tmpColor.a;
					*tmpPointer++ = tmpColor.b;
					*tmpPointer++ = tmpColor.g;
					*tmpPointer++ = tmpColor.r;
				}
			}
			break;
		default:
			return;
	}
	
	XImage* myImage = XCreateImage(m_display,
	                              m_visual->visual,
	                              depth,
	                              ZPixmap,
	                              0,
	                              (char*)tmpVal,
	                              dataImage.GetWidth(),
	                              dataImage.GetHeight(),
	                              32,
	                              0);
	
	Pixmap tmpPixmap = XCreatePixmap(m_display, WindowHandle, dataImage.GetWidth(), dataImage.GetHeight(), depth);
	switch(tmpPixmap) {
		case BadAlloc:
			EWOL_ERROR("X11: BadAlloc");
			break;
		case BadDrawable:
			EWOL_ERROR("X11: BadDrawable");
			break;
		case BadValue:
			EWOL_ERROR("X11: BadValue");
			break;
		default:
			EWOL_DEBUG("Create Pixmap OK");
			break;
	}
	GC tmpGC = DefaultGC(m_display, DefaultScreen(m_display) );
	int error = XPutImage(m_display, tmpPixmap, tmpGC, myImage, 0, 0, 0, 0, dataImage.GetWidth(), dataImage.GetHeight());
	switch(error) {
		case BadDrawable:
			EWOL_ERROR("X11: BadDrawable");
			break;
		case BadGC:
			EWOL_ERROR("X11: BadGC");
			break;
		case BadMatch:
			EWOL_ERROR("X11: BadMatch");
			break;
		case BadValue:
			EWOL_ERROR("X11: BadValue");
			break;
		default:
			EWOL_DEBUG("insert image OK");
			break;
	}
	// allocate a WM hints structure.
	XWMHints* win_hints = XAllocWMHints();
	if (!win_hints) {
		EWOL_ERROR("XAllocWMHints - out of memory");
		return;
	}
	// initialize the structure appropriately. first, specify which size hints we want to fill in. in our case - setting the icon's pixmap.
	win_hints->flags = IconPixmapHint;
	// next, specify the desired hints data. in our case - supply the icon's desired pixmap.
	win_hints->icon_pixmap = tmpPixmap;
	// pass the hints to the window manager.
	XSetWMHints(m_display, WindowHandle, win_hints);
	EWOL_INFO("    ==> might be done ");
	// finally, we can free the WM hints structure.
	XFree(win_hints);
	
	// Note when we free the pixmap ... the icon is removed ... ==> this is a real memory leek ...
	//XFreePixmap(m_display, tmpPixmap);
	
	myImage->data = NULL;
	XDestroyImage(myImage);
	delete[] tmpVal;
	
}


void RemoveDecoration(void)
{
	X11_INFO("X11:RemoveDecoration");
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
	X11_INFO("X11:AddDecoration");
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

static void setVSync(bool sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	typedef int32_t (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	const char *extensions = (char*)glGetString( GL_EXTENSIONS );
	if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 ) {
		EWOL_ERROR("Can not set the vertical synchronisation status" << sync << "   (1)");
		return;
	} else {
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)glXGetProcAddress( (const GLubyte *)"wglSwapIntervalEXT" );
		if(wglSwapIntervalEXT) {
			wglSwapIntervalEXT(sync);
		} else {
			EWOL_ERROR("Can not set the vertical synchronisation status" << sync << "   (2)");
		}
	}
}

bool CreateOGlContext(void)
{
	X11_INFO("X11:CreateOGlContext");
	/* create a GLX context */
	GLXContext RenderContext = glXCreateContext(m_display, m_visual, 0, GL_TRUE);
	/* connect the glx-context to the window */
	glXMakeCurrent(m_display, WindowHandle, RenderContext);
	if (glXIsDirect(m_display, RenderContext)) {
		EWOL_INFO("XF86 DRI enabled\n");
	} else {
		EWOL_INFO("XF86 DRI NOT available\n");
	}
	
	// Enable vertical synchronisation : (some computer has synchronisation disable)
	setVSync(true);
	
	return true;
}

void X11_Init(void)
{
	X11_INFO("X11:INIT");
	if (m_doubleBuffered) {
		glXSwapBuffers(m_display, WindowHandle);
		XSync (m_display,0);
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
			XNextEvent(m_display, &event);
			switch (event.type)
			{
				case ClientMessage:
					{
						X11_INFO("Receive : ClientMessage");
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
					X11_INFO("X11 event SelectionClear");
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
					X11_INFO("X11 event SelectionNotify");
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
							ewol::clipBoard::SetSystem(ewol::clipBoard::clipboardSelection, tmpppp);
							// just transmit an event , we have the data in the system
							eSystem::ClipBoardArrive(ewol::clipBoard::clipboardSelection);
						} else {
							etk::UString tmpppp((char*)buf);
							ewol::clipBoard::SetSystem(ewol::clipBoard::clipboardStd, tmpppp);
							// just transmit an event , we have the data in the system
							eSystem::ClipBoardArrive(ewol::clipBoard::clipboardStd);
						}
					}
					break;
				case SelectionRequest:
					X11_INFO("X11 event SelectionRequest");
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
							tmpData = ewol::clipBoard::Get(ewol::clipBoard::clipboardSelection);
						} else if (req->selection == XAtomeClipBoard) {
							tmpData = ewol::clipBoard::Get(ewol::clipBoard::clipboardStd);
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
					X11_INFO("X11 event Expose");
					break;
				case GraphicsExpose:
					X11_INFO("X11 event GraphicsExpose");
					break;
				case NoExpose:
					X11_INFO("X11 event NoExpose");
					break;
				case CreateNotify:
					X11_INFO("X11 event CreateNotify");
					break;
				case DestroyNotify:
					X11_INFO("X11 event DestroyNotify");
					break;
				case GravityNotify:
					X11_INFO("X11 event GravityNotify");
					break;
				case VisibilityNotify:
					X11_INFO("X11 event VisibilityNotify");
					break;
				case CirculateNotify:
					X11_INFO("X11 event CirculateNotify");
					break;
				case ReparentNotify:
					X11_INFO("X11 event ReparentNotify");
					break;
				case PropertyNotify:
					X11_INFO("X11 event PropertyNotify");
					break;
				case ConfigureNotify:
					X11_INFO("X11 event ConfigureNotify");
					if (m_display == event.xconfigure.display) {
						//EWOL_INFO("X11 event ConfigureNotify event=" << (int32_t)event.xconfigure.event << "  Window=" << (int32_t)event.xconfigure.window << "  above=" << (int32_t)event.xconfigure.above << "  border_width=" << (int32_t)event.xconfigure.border_width );
						m_originX = event.xconfigure.x;
						m_originY = event.xconfigure.y;
						X11_INFO("X11 configure windows position : (" << m_originX << "," << m_originY << ")");
						m_currentHeight = event.xconfigure.height;
						m_currentWidth = event.xconfigure.width;
						X11_INFO("X11 configure windows size : (" << m_currentHeight << "," << m_currentWidth << ")");
						eSystem::Resize(event.xconfigure.width, event.xconfigure.height);
					}
					break;
				case ButtonPress:
					X11_INFO("X11 event ButtonPress");
					m_cursorEventX = event.xbutton.x;
					m_cursorEventY = (m_currentHeight-event.xbutton.y);
					if (event.xbutton.button < NB_MAX_INPUT) {
						inputIsPressed[event.xbutton.button] = true;
					}
					eSystem::SetMouseState(event.xbutton.button, true, (float)event.xbutton.x, (float)m_cursorEventY);
					break;
				case ButtonRelease:
					X11_INFO("X11 event ButtonRelease");
					m_cursorEventX = event.xbutton.x;
					m_cursorEventY = (m_currentHeight-event.xbutton.y);
					if (event.xbutton.button < NB_MAX_INPUT) {
						inputIsPressed[event.xbutton.button] = false;
					}
					eSystem::SetMouseState(event.xbutton.button, false, (float)event.xbutton.x, (float)m_cursorEventY);
					break;
				case EnterNotify:
					X11_INFO("X11 event EnterNotify");
					m_cursorEventX = event.xcrossing.x;
					m_cursorEventY = (m_currentHeight-event.xcrossing.y);
					//EWOL_DEBUG("X11 event : " << event.type << " = \"EnterNotify\" (" << (float)event.xcrossing.x << "," << (float)event.xcrossing.y << ")");
					//gui_uniqueWindows->GenEventInput(0, ewol::EVENT_INPUT_TYPE_ENTER, (float)event.xcrossing.x, (float)event.xcrossing.y);
					m_curentGrabDelta -= vec2(m_originX, -m_originY);
					EWOL_DEBUG("change grab delta of : " << vec2(m_originX, m_originY) );
					break;
				case LeaveNotify:
					X11_INFO("X11 event LeaveNotify");
					m_cursorEventX = event.xcrossing.x;
					m_cursorEventY = (m_currentHeight-event.xcrossing.y);
					//EWOL_DEBUG("X11 event : " << event.type << " = \"LeaveNotify\" (" << (float)event.xcrossing.x << "," << (float)event.xcrossing.y << ")");
					m_curentGrabDelta += vec2(m_originX, -m_originY);
					EWOL_DEBUG("change grab delta of : " << vec2(m_originX, m_originY) );
					break;
				case MotionNotify:
					X11_INFO("X11 event MotionNotify");
					if(    true==m_grabAllEvent
					    && event.xmotion.x == (int32_t)m_forcePos.x
					    && event.xmotion.y == (int32_t)m_forcePos.y) {
						X11_VERBOSE("X11 reject mouse move (grab mode)");
						// we get our requested position...
						m_positionChangeRequested = false;
						m_curentGrabDelta = vec2(0,0);
					} else {
						m_cursorEventX = event.xmotion.x;
						m_cursorEventY = (m_currentHeight-event.xmotion.y);
						if(true==m_grabAllEvent) {
							m_cursorEventX -= m_forcePos.x;
							m_cursorEventY -= (m_currentHeight-m_forcePos.y);
						}
						vec2 newDelta = vec2(m_cursorEventX, m_cursorEventY);
						if(true==m_grabAllEvent) {
							m_cursorEventX -= m_curentGrabDelta.x;
							m_cursorEventY -= m_curentGrabDelta.y;
						}
						m_curentGrabDelta = newDelta;
						// For compatibility of the Android system : 
						bool findOne = false;
						for (int32_t iii=0; iii<NB_MAX_INPUT ; iii++) {
							if (true == inputIsPressed[iii]) {
								X11_DEBUG("X11 event: bt=" << iii << " " << event.type << " = \"MotionNotify\" (" << m_cursorEventX << "," << m_cursorEventY << ")");
								eSystem::SetMouseMotion(iii, m_cursorEventX, m_cursorEventY);
								findOne = true;
							}
						}
						if (false == findOne) {
							X11_DEBUG("X11 event: bt=" << 0 << " " << event.type << " = \"MotionNotify\" (" << m_cursorEventX << "," << m_cursorEventY << ")");
							eSystem::SetMouseMotion(0, m_cursorEventX, m_cursorEventY);
						}
						if (true==m_grabAllEvent) {
							if (m_positionChangeRequested == false) {
								X11_DEBUG("X11 Set pointer position : " << m_forcePos);
								XWarpPointer(m_display, None, WindowHandle, 0,0, 0, 0, m_forcePos.x, m_forcePos.y);
								XFlush(m_display);
								m_positionChangeRequested = true;
							}
						}
					}
					break;
				case FocusIn:
					X11_INFO("X11 event FocusIn");
					break;
				case FocusOut:
					X11_INFO("X11 event : FocusOut");
					break;
				case KeyPress:
				case KeyRelease:
					// filter repeate key : 
					if(    event.type == KeyRelease
					    && XEventsQueued(m_display, QueuedAfterReading)) {
						XEvent nev;
						XPeekEvent(m_display, &nev);
						if(    nev.type == KeyPress
						    && nev.xkey.time == event.xkey.time
						    && nev.xkey.keycode == event.xkey.keycode) {
							// Key was not actually released
							// remove next event too ...
							XNextEvent(m_display, &nev);
							break;
						}
					}
					X11_INFO("X11 event : " << event.type << " = \"KeyPress/KeyRelease\" ");
					{
						X11_DEBUG("eventKey : " << event.xkey.keycode << " state : " << event.xkey.state);
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
							guiKeyBoardMode.numLock = true;
						} else {
							guiKeyBoardMode.numLock = false;
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
						ewol::keyEvent::keyboard_te keyInput;
						switch (event.xkey.keycode) {
							//case 80: // keypad
							case 111:	keyInput = ewol::keyEvent::keyboardUp;            break;
							//case 83: // keypad
							case 113:	keyInput = ewol::keyEvent::keyboardLeft;          break;
							//case 85: // keypad
							case 114:	keyInput = ewol::keyEvent::keyboardRight;         break;
							//case 88: // keypad
							case 116:	keyInput = ewol::keyEvent::keyboardDown;          break;
							//case 81: // keypad
							case 112:	keyInput = ewol::keyEvent::keyboardPageUp;        break;
							//case 89: // keypad
							case 117:	keyInput = ewol::keyEvent::keyboardPageDown;      break;
							//case 79: // keypad
							case 110:	keyInput = ewol::keyEvent::keyboardStart;         break;
							//case 87: // keypad
							case 115:	keyInput = ewol::keyEvent::keyboardEnd;           break;
							case 78:	keyInput = ewol::keyEvent::keyboardStopDefil;     break;
							case 127:	keyInput = ewol::keyEvent::keyboardWait;          break;
							//case 90: // keypad
							case 118:
								keyInput = ewol::keyEvent::keyboardInsert;
								if(event.type == KeyRelease) {
									if (true == guiKeyBoardMode.insert) {
										guiKeyBoardMode.insert = false;
									} else {
										guiKeyBoardMode.insert = true;
									}
								}
								break;
							//case 84:  keyInput = ewol::keyEvent::keyboardCenter; break; // Keypad
							case 67:    keyInput = ewol::keyEvent::keyboardF1; break;
							case 68:    keyInput = ewol::keyEvent::keyboardF2; break;
							case 69:    keyInput = ewol::keyEvent::keyboardF3; break;
							case 70:    keyInput = ewol::keyEvent::keyboardF4; break;
							case 71:    keyInput = ewol::keyEvent::keyboardF5; break;
							case 72:    keyInput = ewol::keyEvent::keyboardF6; break;
							case 73:    keyInput = ewol::keyEvent::keyboardF7; break;
							case 74:    keyInput = ewol::keyEvent::keyboardF8; break;
							case 75:    keyInput = ewol::keyEvent::keyboardF9; break;
							case 76:    keyInput = ewol::keyEvent::keyboardF10; break;
							case 95:    keyInput = ewol::keyEvent::keyboardF11; break;
							case 96:    keyInput = ewol::keyEvent::keyboardF12; break;
							case 66:    keyInput = ewol::keyEvent::keyboardCapLock;     guiKeyBoardMode.capLock = (event.type == KeyPress) ? true : false; break;
							case 50:    keyInput = ewol::keyEvent::keyboardShiftLeft;   guiKeyBoardMode.shift   = (event.type == KeyPress) ? true : false; break;
							case 62:    keyInput = ewol::keyEvent::keyboardShiftRight;  guiKeyBoardMode.shift   = (event.type == KeyPress) ? true : false; break;
							case 37:    keyInput = ewol::keyEvent::keyboardCtrlLeft;    guiKeyBoardMode.ctrl    = (event.type == KeyPress) ? true : false; break;
							case 105:   keyInput = ewol::keyEvent::keyboardCtrlRight;   guiKeyBoardMode.ctrl    = (event.type == KeyPress) ? true : false; break;
							case 133:   keyInput = ewol::keyEvent::keyboardMetaLeft;    guiKeyBoardMode.meta    = (event.type == KeyPress) ? true : false; break;
							case 134:   keyInput = ewol::keyEvent::keyboardMetaRight;   guiKeyBoardMode.meta    = (event.type == KeyPress) ? true : false; break;
							case 64:    keyInput = ewol::keyEvent::keyboardAlt;         guiKeyBoardMode.alt     = (event.type == KeyPress) ? true : false; break;
							case 108:   keyInput = ewol::keyEvent::keyboardAltGr;       guiKeyBoardMode.altGr   = (event.type == KeyPress) ? true : false; break;
							case 135:   keyInput = ewol::keyEvent::keyboardContextMenu; break;
							case 77:    keyInput = ewol::keyEvent::keyboardNumLock;     guiKeyBoardMode.numLock = (event.type == KeyPress) ? true : false; break;
							case 91: // Suppr on keypad
								find = false;
								if(guiKeyBoardMode.numLock==true){
									eSystem::SetKeyboard(guiKeyBoardMode, '.', (event.type==KeyPress));
								} else {
									eSystem::SetKeyboard(guiKeyBoardMode, 0x7F, (event.type==KeyPress));
								}
								break;
							case 23: // special case for TAB
								find = false;
								eSystem::SetKeyboard(guiKeyBoardMode, 0x09, (event.type==KeyPress));
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
										// transform it in unicode
										uniChar_t tmpChar = 0;
										unicode::convertIsoToUnicode(unicode::EDN_CHARSET_ISO_8859_15, buf[0], tmpChar);
										//EWOL_INFO("event Key : " << event.xkey.keycode << " char=\"" << buf << "\"'len=" << strlen(buf) << " unicode=" << unicodeValue);
										eSystem::SetKeyboard(guiKeyBoardMode, tmpChar, (event.type==KeyPress));
									} else {
										EWOL_WARNING("Unknow event Key : " << event.xkey.keycode);
									}
								}
								break;
						}
						if (true == find) {
							//EWOL_DEBUG("eventKey Move type : " << GetCharTypeMoveEvent(keyInput) );
							eSystem::SetKeyboardMove(guiKeyBoardMode, keyInput, (event.type==KeyPress));
						}
					}
					break;
				//case DestroyNotify:
				//	break;
				case MapNotify:
					X11_INFO("X11 event : MapNotify");
					eSystem::Show();
					break;
				case UnmapNotify:
					X11_INFO("X11 event : UnmapNotify");
					eSystem::Hide();
					break;
				default:
					X11_INFO("X11 event : " << event.type << " = \"???\"");
					break;
			}
		}
		if(true == m_run) {
			bool hasDisplay = eSystem::Draw(false);
			if (m_doubleBuffered && hasDisplay) {
				glXSwapBuffers(m_display, WindowHandle);
				XSync(m_display,0);
			}
		}
	}
};


// -------------------------------------------------------------------------
//         ClipBoard AREA :
// -------------------------------------------------------------------------


void guiInterface::ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	switch (clipboardID)
	{
		case ewol::clipBoard::clipboardSelection:
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
		case ewol::clipBoard::clipboardStd:
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


void guiInterface::ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	switch (clipboardID)
	{
		case ewol::clipBoard::clipboardSelection:
			// Request the selection :
			if (false == l_clipBoardOwnerPrimary) {
				XSetSelectionOwner(m_display, XAtomeSelection, WindowHandle, CurrentTime);
				l_clipBoardOwnerPrimary = true;
			}
			break;
		case ewol::clipBoard::clipboardStd:
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


void guiInterface::Stop(void)
{
	X11_INFO("X11-API: Stop");
	m_run = false;
}

void guiInterface::KeyboardShow(void)
{
	// nothing to do : No keyboard on computer ...
}


void guiInterface::KeyboardHide(void)
{
	// nothing to do : No keyboard on computer ...
}


void guiInterface::ChangeSize(ivec2 size)
{
	X11_INFO("X11-API: ChangeSize=" << size);
	m_currentHeight = size.y;
	m_currentWidth = size.x;
	XResizeWindow(m_display, WindowHandle, size.x, size.y);
}


void guiInterface::ChangePos(ivec2 pos)
{
	X11_INFO("X11-API: ChangePos=" << pos);
	XMoveWindow(m_display, WindowHandle, pos.x, pos.y);
	m_originX = pos.x;
	m_originY = pos.y;
}


void guiInterface::GetAbsPos(ivec2& pos)
{
	X11_INFO("X11-API: GetAbsPos");
	int tmp;
	unsigned int tmp2;
	Window fromroot, tmpwin;
	XQueryPointer(m_display, WindowHandle, &fromroot, &tmpwin, &pos.x, &pos.y, &tmp, &tmp, &tmp2);
}

#include <X11/cursorfont.h>
// select the current cursor to display :
static ewol::cursorDisplay_te l_currentCursor = ewol::cursorArrow;

void guiInterface::SetCursor(ewol::cursorDisplay_te newCursor)
{
	if (newCursor != l_currentCursor) {
		X11_DEBUG("X11-API: Set New Cursor : " << newCursor);
		// undefine previous cursors ...
		XUndefineCursor(m_display, WindowHandle);
		// set the new one :
		l_currentCursor = newCursor;
		Cursor myCursor = None;
		switch (l_currentCursor) {
			case ewol::cursorNone:
				{
					Pixmap bitmapNoData;
					XColor black;
					static char noData[] = { 0,0,0,0,0,0,0,0 };
					black.red = 0;
					black.green = 0;
					black.blue = 0;
					bitmapNoData = XCreateBitmapFromData(m_display, WindowHandle, noData, 8, 8);
					myCursor = XCreatePixmapCursor(m_display, bitmapNoData, bitmapNoData, 
					                               &black, &black, 0, 0);
				}
				break;
			case ewol::cursorLeftArrow:
				myCursor = XCreateFontCursor(m_display, XC_top_left_arrow);
				break;
			case ewol::cursorInfo:
				myCursor = XCreateFontCursor(m_display, XC_hand1);
				break;
			case ewol::cursorDestroy:
				myCursor = XCreateFontCursor(m_display, XC_pirate);
				break;
			case ewol::cursorHelp:
				myCursor = XCreateFontCursor(m_display, XC_question_arrow);
				break;
			case ewol::cursorCycle:
				myCursor = XCreateFontCursor(m_display, XC_exchange);
				break;
			case ewol::cursorSpray:
				myCursor = XCreateFontCursor(m_display, XC_spraycan);
				break;
			case ewol::cursorWait:
				myCursor = XCreateFontCursor(m_display, XC_watch);
				break;
			case ewol::cursorText:
				myCursor = XCreateFontCursor(m_display, XC_xterm);
				break;
			case ewol::cursorCrossHair:
				myCursor = XCreateFontCursor(m_display, XC_crosshair);
				break;
			case ewol::cursorSlideUpDown:
				myCursor = XCreateFontCursor(m_display, XC_sb_v_double_arrow);
				break;
			case ewol::cursorSlideLeftRight:
				myCursor = XCreateFontCursor(m_display, XC_sb_h_double_arrow);
				break;
			case ewol::cursorResizeUp:
				myCursor = XCreateFontCursor(m_display, XC_top_side);
				break;
			case ewol::cursorResizeDown:
				myCursor = XCreateFontCursor(m_display, XC_bottom_side);
				break;
			case ewol::cursorResizeLeft:
				myCursor = XCreateFontCursor(m_display, XC_left_side);
				break;
			case ewol::cursorResizeRight:
				myCursor = XCreateFontCursor(m_display, XC_right_side);
				break;
			case ewol::cursorCornerTopLeft:
				myCursor = XCreateFontCursor(m_display, XC_top_left_corner);
				break;
			case ewol::cursorCornerTopRight:
				myCursor = XCreateFontCursor(m_display, XC_top_right_corner);
				break;
			case ewol::cursorCornerButtomLeft:
				myCursor = XCreateFontCursor(m_display, XC_bottom_right_corner);
				break;
			case ewol::cursorCornerButtomRight:
				myCursor = XCreateFontCursor(m_display, XC_bottom_left_corner);
				break;
			default :
				// nothing to do ... basic pointer ...
				break;
		}
		if (myCursor != None) {
			XDefineCursor(m_display,WindowHandle, myCursor);
			XFreeCursor(m_display, myCursor);
		}
	}
}

void guiInterface::GrabPointerEvents(bool isGrabbed, vec2 forcedPosition)
{
	if (true == isGrabbed) {
		X11_DEBUG("X11-API: Grab Events");
		int32_t test = XGrabPointer(m_display,RootWindow(m_display, DefaultScreen(m_display)), True,
		                            ButtonPressMask |
		                            ButtonReleaseMask |
		                            PointerMotionMask |
		                            FocusChangeMask |
		                            EnterWindowMask |
		                            LeaveWindowMask,
		                            GrabModeAsync,
		                            GrabModeAsync,
		                            RootWindow(m_display, DefaultScreen(m_display)),
		                            None,
		                            CurrentTime);
		
		if (GrabSuccess != test)
		{
			EWOL_CRITICAL("Display error " << test);
			switch (test)
			{
				case BadCursor:
					EWOL_CRITICAL(" BadCursor");
					break;
				case BadValue:
					EWOL_CRITICAL(" BadValue");
					break;
				case BadWindow:
					EWOL_CRITICAL(" BadWindow");
					break;
			}
		}
		m_forcePos = forcedPosition;
		m_forcePos.y = m_currentHeight - m_forcePos.y;
		m_grabAllEvent = true;
		// change the pointer position to generate a good mouving at the start ...
		X11_DEBUG("X11-API: Set pointer position : " << m_forcePos);
		XWarpPointer(m_display, None, WindowHandle, 0,0, 0, 0, m_forcePos.x, m_forcePos.y);
		XFlush(m_display);
		m_positionChangeRequested = true;
		m_curentGrabDelta = vec2(0,0);
	} else {
		X11_DEBUG("X11-API: Un-Grab Events");
		XUngrabPointer(m_display, CurrentTime);
		m_grabAllEvent = false;
		m_forcePos = vec2(0,0);
		m_curentGrabDelta = vec2(0,0);
	}
}


/**
 * @brief Main of the program
 * @param std IO
 * @return std IO
 */
int guiInterface::main(int argc, const char *argv[])
{
	
	for (int32_t iii=0; iii<NB_MAX_INPUT; iii++) {
		inputIsPressed[iii] = false;
	}
	m_grabAllEvent = false;
	// start X11 thread ...
	X11_Init();
	//start the basic thread : 
	eSystem::Init();
	// get the icon file : 
	etk::UString myIcon = APP_Icon();
	SetIcon(myIcon);
	// Run ...
	X11_Run();
	// close X11 :
	guiInterface::Stop();
	// uninit ALL :
	eSystem::UnInit();
	return 0;
}


void guiInterface::ForceOrientation(ewol::orientation_te orientation)
{
	// nothing to do ...
}
