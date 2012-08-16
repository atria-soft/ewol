/**
 *******************************************************************************
 * @file guiWindows.cpp
 * @brief Gui abstraction layer for windows (Sources)
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

#include <etk/TypesCoordonate.h>
#include <etk/UString.h>
#include <etk/unicode.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>

#include <ewol/Texture.h>
#include <ewol/Texture/TextureBMP.h>
#include <ewol/base/MainThread.h>
#include <ewol/threadMsg.h>
#include <ewol/importgl.h>

#include <sys/time.h>

int64_t ewol::GetTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    //EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
    return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_usec);
}


int32_t separateClickTime = 300000;
int32_t offsetMoveClicked = 10000;
int32_t offsetMoveClickedDouble = 20000;

bool inputIsPressed[20];

static guiSystem::event::specialKey_ts guiKeyBoardMode;


void ewol::SetTitle(etk::UString title)
{
	// TODO ...
}


static etk::Vector<etk::UString*> listArgs;

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


#undef __class__
#define __class__ "guiAbstraction"

bool m_run = true;

void guiAbstraction::Stop(void)
{
	m_run = false;
	// To exit program ...
	PostQuitMessage(0);
}


void guiAbstraction::KeyboardShow(ewol::keyboardMode_te mode)
{
	// nothing to do : No keyboard on computer ...
}

void guiAbstraction::KeyboardHide(void)
{
	// nothing to do : No keyboard on computer ...
}



void guiAbstraction::ChangeSize(int32_t w, int32_t h)
{
	
}

void guiAbstraction::ChangePos(int32_t x, int32_t y)
{
	
}

void guiAbstraction::GetAbsPos(int32_t & x, int32_t & y)
{
	
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



// ClipBoard AREA :

etk::UString l_clipBoardStd("");
etk::UString l_clipBoardPrimary("");

void guiAbstraction::ClipBoardGet(etk::UString &newData, clipBoardMode_te mode)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("Request Get of a clipboard : " << mode << " size=" << newData.Size() );
	#endif
	newData = "";
	switch (mode)
	{
		case CLIPBOARD_MODE_PRIMARY:
			// get our own buffer ...
			newData = l_clipBoardPrimary;
			break;
		case CLIPBOARD_MODE_STD:
			// get our own buffer ...
			newData = l_clipBoardStd;
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}


void guiAbstraction::ClipBoardSet(etk::UString &newData, clipBoardMode_te mode)
{
	#ifdef DEBUG_X11_EVENT
		EWOL_INFO("Request set of a clipboard : " << mode << " size=" << newData.Size() );
	#endif
	switch (mode)
	{
		case CLIPBOARD_MODE_PRIMARY:
			if (newData.Size() > 0) {
				// copy it ...
				l_clipBoardPrimary = newData;
			}
			break;
		case CLIPBOARD_MODE_STD:
			if (newData.Size() > 0) {
				// copy it ...
				l_clipBoardStd = newData;
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}

// might be declared by the application ....
etk::File APP_Icon(void);

#include <windows.h>
#include <windowsx.h>

// Function Declarations

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
int plop(void);

int main(int argc, char *argv[])
{
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
			etk::UString* tmpString = new etk::UString(argv[i]);
			if (NULL != tmpString) {
				listArgs.PushBack(tmpString);
			}
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
	// TODO : ...
	//start the basic thread : 
	guiSystem::Init();
	// get the icon file : 
	etk::File myIcon = APP_Icon();
	//SetIcon(myIcon);
	
	// Run ...
	plop();
	// close X11 :
	guiAbstraction::Stop();
	// uninit ALL :
	guiSystem::UnInit();
	for (int32_t iii=0; iii<listArgs.Size(); iii++) {
		if (NULL != listArgs[iii]) {
			delete listArgs[iii];
			listArgs[iii] = NULL;
		}
	}
	listArgs.Clear();
	return 0;
}


// WinMain

int plop(void)
{
	HINSTANCE hInstance = 0;
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	
	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "EwolMainWindows";
	RegisterClass( &wc );
	
	// create main window
	hWnd = CreateWindow( "EwolMainWindows", "Ewol ... TODO Title", 
	                     WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_SIZEBOX,
	                     0, 0, 800, 600,
	                     NULL, NULL, hInstance, NULL );
	
	guiSystem::event::Resize(800, 600-25);
	
	// enable OpenGL for the window
	EnableOpenGL( hWnd, &hDC, &hRC );
	
	// program main loop
	while(true == m_run) {
		
		// check for messages
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) {
			
			// handle or dispatch messages
			if ( msg.message == WM_QUIT ) {
				m_run = false;
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		} else {
			(void)guiSystem::Draw(true);
			SwapBuffers( hDC );
		}
	}
	// shutdown OpenGL
	DisableOpenGL( hWnd, hDC, hRC );
	
	// destroy the window explicitly
	DestroyWindow( hWnd );
	
	return msg.wParam;
	
}

// Window Procedure

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool buttonIsDown = true;
	int32_t mouseButtonId = 0;
	Vector2D<int32_t> pos;
	// to know all message : http://wiki.winehq.org/List_Of_Windows_Messages
	switch (message)
	{
		/* **************************************************************************
		 *                             Gui event
		 * **************************************************************************/
		case WM_CREATE:
			EWOL_DEBUG("WM_CREATE");
			return 0;
		case WM_CLOSE:
			EWOL_DEBUG("WM_CLOSE");
			PostQuitMessage( 0 );
			return 0;
		case WM_DESTROY:
			EWOL_DEBUG("WM_DESTROY");
			return 0;
		case WM_MOVE:
			EWOL_DEBUG("WM_MOVE");
			return 0;
		case WM_SIZE:
			EWOL_DEBUG("WM_SIZE");
			return 0;
		/*
		case WM_GETMINMAXINFO:
			{
				MINMAXINFO* tmpVal = (MINMAXINFO*)lParam;
				EWOL_DEBUG("WM_GETMINMAXINFO : ");
				EWOL_DEBUG("    ptMaxSize : " << tmpVal->ptMaxSize.x << "," << tmpVal->ptMaxSize.y << ")");
				EWOL_DEBUG("    ptMaxPosition : " << tmpVal->ptMaxPosition.x << "," << tmpVal->ptMaxPosition.y << ")");
				EWOL_DEBUG("    ptMinTrackSize : " << tmpVal->ptMinTrackSize.x << "," << tmpVal->ptMinTrackSize.y << ")");
				EWOL_DEBUG("    ptMaxTrackSize : " << tmpVal->ptMaxTrackSize.x << "," << tmpVal->ptMaxTrackSize.y << ")");
			}
			return 0;
		*/
		case WM_WINDOWPOSCHANGING:
			{
				WINDOWPOS* tmpVal = (WINDOWPOS*)lParam;
				if (NULL != tmpVal) {
					//EWOL_DEBUG("WM_WINDOWPOSCHANGING : : (" << tmpVal->x << "," << tmpVal->y << ") ( " << tmpVal->cx << "," << tmpVal->cy << ")");
					guiSystem::event::Resize(tmpVal->cx-8, tmpVal->cy - 28);
				}
			}
			return 0;
		// these message are not parse by us ...
		case WM_SETCURSOR: // Call the windows if we want the mouse event :
		case WM_NCHITTEST: // inform the application the position of the mouse is moving
			return DefWindowProc( hWnd, message, wParam, lParam );
		
		/* **************************************************************************
		 *                             Keyboard management
		 * **************************************************************************/
		case WM_KEYUP:
			buttonIsDown = false;
		case WM_KEYDOWN:
			{
				uniChar_t tmpChar = 0;
				ewol::eventKbMoveType_te keyInput;
				switch (wParam) {
					//case 80: // keypad
					case VK_UP:     keyInput = ewol::EVENT_KB_MOVE_TYPE_UP; break;
					//case 83: // keypad
					case VK_LEFT:   keyInput = ewol::EVENT_KB_MOVE_TYPE_LEFT; break;
					//case 85: // keypad
					case VK_RIGHT:  keyInput = ewol::EVENT_KB_MOVE_TYPE_RIGHT; break;
					//case 88: // keypad
					case VK_DOWN:   keyInput = ewol::EVENT_KB_MOVE_TYPE_DOWN; break;
					//case 81: // keypad
					case VK_PRIOR:  keyInput = ewol::EVENT_KB_MOVE_TYPE_PAGE_UP; break;
					//case 89: // keypad
					case VK_NEXT:   keyInput = ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN; break;
					//case 79: // keypad
					case VK_HOME:   keyInput = ewol::EVENT_KB_MOVE_TYPE_START; break;
					//case 87: // keypad
					case VK_END:    keyInput = ewol::EVENT_KB_MOVE_TYPE_END; break;
					//case VK_:     keyInput = ewol::EVENT_KB_MOVE_TYPE_ARRET_DEFIL; break;
					case VK_PAUSE:  keyInput = ewol::EVENT_KB_MOVE_TYPE_WAIT; break;
					//case 90: // keypad
					case VK_INSERT:
						keyInput = ewol::EVENT_KB_MOVE_TYPE_INSERT;
						guiKeyBoardMode.insert = buttonIsDown;
						break;
					case VK_F1: keyInput = ewol::EVENT_KB_MOVE_TYPE_F1; break;
					case VK_F2: keyInput = ewol::EVENT_KB_MOVE_TYPE_F2; break;
					case VK_F3: keyInput = ewol::EVENT_KB_MOVE_TYPE_F3; break;
					case VK_F4: keyInput = ewol::EVENT_KB_MOVE_TYPE_F4; break;
					case VK_F5: keyInput = ewol::EVENT_KB_MOVE_TYPE_F5; break;
					case VK_F6: keyInput = ewol::EVENT_KB_MOVE_TYPE_F6; break;
					case VK_F7: keyInput = ewol::EVENT_KB_MOVE_TYPE_F7; break;
					case VK_F8: keyInput = ewol::EVENT_KB_MOVE_TYPE_F8; break;
					case VK_F9: keyInput = ewol::EVENT_KB_MOVE_TYPE_F9; break;
					case VK_F10: keyInput = ewol::EVENT_KB_MOVE_TYPE_F10; break;
					case VK_F11: keyInput = ewol::EVENT_KB_MOVE_TYPE_F11; break;
					case VK_F12:
					case VK_F13:
					case VK_F14:
					case VK_F15:
					case VK_F16:
					case VK_F17:
					case VK_F18:
					case VK_F19:
					case VK_F20:
					case VK_F21:
					case VK_F22:
					case VK_F23:
					case VK_F24: keyInput = ewol::EVENT_KB_MOVE_TYPE_F12; break;
					case VK_CAPITAL:   keyInput = ewol::EVENT_KB_MOVE_TYPE_CAPLOCK;     guiKeyBoardMode.capLock = buttonIsDown; break;
					
					case VK_SHIFT:
					case VK_LSHIFT:    keyInput = ewol::EVENT_KB_MOVE_TYPE_SHIFT_LEFT;  guiKeyBoardMode.shift   = buttonIsDown; break;
					case VK_RSHIFT:    keyInput = ewol::EVENT_KB_MOVE_TYPE_SHIFT_RIGHT; guiKeyBoardMode.shift   = buttonIsDown; break;
					
					case VK_CONTROL:
					case VK_LCONTROL:  keyInput = ewol::EVENT_KB_MOVE_TYPE_CTRL_LEFT;   guiKeyBoardMode.ctrl    = buttonIsDown; break;
					case VK_RCONTROL:  keyInput = ewol::EVENT_KB_MOVE_TYPE_CTRL_RIGHT;  guiKeyBoardMode.ctrl    = buttonIsDown; break;
					
					case VK_LWIN:      keyInput = ewol::EVENT_KB_MOVE_TYPE_META_LEFT;   guiKeyBoardMode.meta    = buttonIsDown; break;
					case VK_RWIN:      keyInput = ewol::EVENT_KB_MOVE_TYPE_META_RIGHT;  guiKeyBoardMode.meta    = buttonIsDown; break;
					
					case VK_MENU:
					case VK_LMENU:     keyInput = ewol::EVENT_KB_MOVE_TYPE_ALT;         guiKeyBoardMode.alt     = buttonIsDown; break;
					case VK_RMENU:     keyInput = ewol::EVENT_KB_MOVE_TYPE_ALT_GR;      guiKeyBoardMode.altGr   = buttonIsDown; break;
					//case :   keyInput = ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU; break;
					case VK_NUMLOCK:   keyInput = ewol::EVENT_KB_MOVE_TYPE_VER_NUM;     guiKeyBoardMode.verNum  = buttonIsDown; break;
					case VK_BACK: // DEL
						tmpChar = 0x00000008;
						break;
					// TODO : Really strang, need to understand why ...
					case 46: // Suppr
						tmpChar = 0x0000007F;
						break;
					case VK_TAB: // special case for TAB
						tmpChar = 0x00000009;
						break;
					case VK_RETURN: // special case for TAB
						tmpChar = '\n';
						break;
					default:
						{
							BYTE kbd[256];
							GetKeyboardState(kbd);
							const int BUFFER_LENGTH = 8; //Length of the buffer
							WCHAR chars[BUFFER_LENGTH];
							
							ToUnicode(wParam,lParam,kbd,chars,BUFFER_LENGTH,0);
							unicode::convertUtf8ToUnicode((char*)chars, tmpChar);
						}
						break;
				}
				EWOL_DEBUG("kjhkjhkjhkjhkj = " << wParam);
				if (tmpChar == 0) {
					//EWOL_DEBUG("eventKey Move type : " << GetCharTypeMoveEvent(keyInput) );
					guiSystem::event::keyboardMove_ts specialEvent;
					specialEvent.special = guiKeyBoardMode;
					specialEvent.move = keyInput;
					specialEvent.isDown = buttonIsDown;
					guiSystem::event::SetKeyboardMove(specialEvent);
				} else {
					guiSystem::event::keyboardKey_ts specialEvent;
					specialEvent.special = guiKeyBoardMode;
					specialEvent.myChar = tmpChar;
					specialEvent.isDown = buttonIsDown;
					guiSystem::event::SetKeyboard(specialEvent);
				}
			}
			return 0;
		/* **************************************************************************
		 *                             Mouse management
		 * **************************************************************************/
		case WM_LBUTTONUP:
			buttonIsDown = false;
		case WM_LBUTTONDOWN:
			mouseButtonId = 1;
			pos.x = GET_X_LPARAM(lParam);
			pos.y = GET_Y_LPARAM(lParam);
			inputIsPressed[mouseButtonId] = buttonIsDown;
			guiSystem::event::SetMouseState(mouseButtonId, buttonIsDown, (float)pos.x, (float)pos.y);
			return 0;
		
		case WM_MBUTTONUP:
			buttonIsDown = false;
		case WM_MBUTTONDOWN:
			mouseButtonId = 2;
			pos.x = GET_X_LPARAM(lParam);
			pos.y = GET_Y_LPARAM(lParam);
			inputIsPressed[mouseButtonId] = buttonIsDown;
			guiSystem::event::SetMouseState(mouseButtonId, buttonIsDown, (float)pos.x, (float)pos.y);
			return 0;
		
		case WM_RBUTTONUP:
			buttonIsDown = false;
		case WM_RBUTTONDOWN:
			mouseButtonId = 3;
			pos.x = GET_X_LPARAM(lParam);
			pos.y = GET_Y_LPARAM(lParam);
			inputIsPressed[mouseButtonId] = buttonIsDown;
			guiSystem::event::SetMouseState(mouseButtonId, buttonIsDown, (float)pos.x, (float)pos.y);
			return 0;
		
		case WM_MOUSEWHEEL:
			if (wParam & 0x200000) {
				EWOL_DEBUG("event SCROOL UP");
				mouseButtonId = 4;
			} else{
				EWOL_DEBUG("event SCROOL DOWN");
				mouseButtonId = 5;
			}
			pos.x = GET_X_LPARAM(lParam);
			pos.y = GET_Y_LPARAM(lParam);
			guiSystem::event::SetMouseState(mouseButtonId, true,  (float)pos.x, (float)pos.y);
			guiSystem::event::SetMouseState(mouseButtonId, false, (float)pos.x, (float)pos.y);
			return 0;
		
		case WM_MOUSEHOVER:
		case WM_MOUSEMOVE:
			pos.x = GET_X_LPARAM(lParam);
			pos.y = GET_Y_LPARAM(lParam);
			for (int32_t iii=0; iii<NB_MAX_INPUT ; iii++) {
				if (true == inputIsPressed[iii]) {
					EWOL_VERBOSE("Windows event: bt=" << iii << " " << message << " = \"WM_MOUSEMOVE\" " << pos );
					guiSystem::event::SetMouseMotion(iii, (float)pos.x, (float)pos.y);
					return 0;
				}
			}
			EWOL_VERBOSE("Windows event: bt=" << 0 << " " << message << " = \"WM_MOUSEMOVE\" " << pos );
			guiSystem::event::SetMouseMotion(0, (float)pos.x, (float)pos.y);
			return 0;
		
		default:
			EWOL_DEBUG("event ..." << message );
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	
}

// Enable OpenGL

void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	*hDC = GetDC( hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, format, &pfd );
	
	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hRC );
	
}

// Disable OpenGL

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}

