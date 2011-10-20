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



namespace guiAbstraction {
	
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
			operator Display*() { return m_display; }
		private:
			Display* m_display;
	};

	#undef __class__
	#define __class__ "guiAbstraction::X11eventMng"
	class X11eventMng
	{
		private:
			ewol::Windows* m_uniqueWindows;
			X11display& m_display;
			bool m_run;
		public:
			X11eventMng(X11display& d) : m_run(true), m_display(d)
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
				XEvent report;
				while (m_run) {
					XNextEvent(m_display, &report);
					HasEvent(report);
				}
			}
			
			void Stop()
			{
				m_run = false;
			}
		private:
			bool HasEvent(XEvent& report)
			{
				if(NULL != m_uniqueWindows) {
					switch ( report.type )
					{
						case Expose:
							m_uniqueWindows->SysOnExpose();
							break;
						case ButtonPress:
							if ( report.xbutton.button & Button2 ) {
								m_uniqueWindows->GenEventInput(2, EVENT_INPUT_TYPE_DOWN, (double)report.xbutton.x, (double)report.xbutton.y);
							} else if (report.xbutton.button & Button1) {
								m_uniqueWindows->GenEventInput(1, EVENT_INPUT_TYPE_DOWN, (double)report.xbutton.x, (double)report.xbutton.y);
							}
							break;
						case ButtonRelease:
							if(report.xbutton.button & Button2) {
								m_uniqueWindows->GenEventInput(2, EVENT_INPUT_TYPE_UP, (double)report.xbutton.x, (double)report.xbutton.y);
							} else if (report.xbutton.button & Button1) {
								m_uniqueWindows->GenEventInput(1, EVENT_INPUT_TYPE_UP, (double)report.xbutton.x, (double)report.xbutton.y);
							}
							break;
						case EnterNotify:
							m_uniqueWindows->GenEventInput(0, EVENT_INPUT_TYPE_ENTER, (double)report.xcrossing.x, (double)report.xcrossing.y);
							break;
						case MotionNotify:
							m_uniqueWindows->GenEventInput(0, EVENT_INPUT_TYPE_MOVE, (double)report.xmotion.x, (double)report.xmotion.y);
							break;
						case LeaveNotify:
							m_uniqueWindows->GenEventInput(0, EVENT_INPUT_TYPE_LEAVE, (double)report.xcrossing.x, (double)report.xcrossing.y);
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
								int count = XLookupString(&report.xkey, buf, 10, &keysym, &status);
								buf[count] = '\0';
								if(report.type == KeyPress) {
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
						case ClientMessage:
							{
								Atom atom = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
								if(atom == report.xclient.data.l[0]) {
									m_uniqueWindows->SysOnKill();
								}
								break;
							}
					}
					return true;
				} else {
					return false;
				}
			}
			
			X11display& get_display(void)
			{
				return m_display;
			}
		
	};




};



#undef __class__
#define __class__ "guiAbstraction"

static bool guiAbstractionIsInit = false;
static X11display * myDisplay = NULL;
static X11eventMng * myEventManager = NULL;

void guiAbstraction::Init(int32_t argc, char *argv[])
{
	if (false == guiAbstractionIsInit) {
		// set the gui is init :
		guiAbstractionIsInit = true;
		EWOL_INFO("INIT for X11 environement");
		myDisplay = new X11display("");
		myEventManager = X11eventMng(myDisplay);
	} else {
		EWOL_CRITICAL("Can not INIT X11 ==> already init before");
	}
}


void guiAbstraction::Run(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("Start Running");
		myEventManager.Run();
		EWOL_INFO("Stop Running");
	} else {
		EWOL_CRITICAL("Can not Run X11 ==> not init ... ");
	}
}

void guiAbstraction::Stop(void)
{
	if (true == guiAbstractionIsInit) {
		myEventManager.Stop();
	} else {
		EWOL_CRITICAL("Can not Stop X11 ==> not init ... ");
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows & newOne)
{
	if (true == guiAbstractionIsInit) {
		myEventManager.Setwindow(&newOne);
	} else {
		EWOL_CRITICAL("Can not set Windows X11 ==> not init ... ");
	}
}

void guiAbstraction::UnInit(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("UN-INIT for X11 environement");
		if (NULL != myEventManager) {
			delete(myEventManager);
		}
		if (NULL != myDisplay) {
			delete(myDisplay);
		}
		guiAbstractionIsInit = false;
	} else {
		EWOL_CRITICAL("Can not Un-Init X11 ==> not init ... ");
	}
}


