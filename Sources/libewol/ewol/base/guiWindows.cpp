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
#include <etk/UString.h>
#include <etk/unicode.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>

#include <ewol/Texture.h>
#include <ewol/Texture/TextureBMP.h>
#include <ewol/base/MainThread.h>
#include <ewol/threadMsg.h>
#include <ewol/importgl.h>


int32_t separateClickTime = 300000;
int32_t offsetMoveClicked = 10000;
int32_t offsetMoveClickedDouble = 20000;

void ewol::SetTitle(etk::UString title)
{
	// TODO ...
}


void EWOL_NativeRender(void)
{
	EWOL_GenericDraw(false);
	glFlush();
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
	return false;
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
	HINSTANCE hPrevInstance = 0;
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL quit = FALSE;
	float theta = 0.0f;
	
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
			EWOL_DEBUG("DRAW ... ");
			guiSystem::Draw();
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
	switch (message)
	{
		
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
			
		case WM_KEYDOWN:
			EWOL_DEBUG("WM_KEYDOWN");
			switch ( wParam ) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
			}
			return 0;
		case WM_MOVE:
			EWOL_DEBUG("WM_MOVE");
			return 0;
		case WM_SIZE:
			EWOL_DEBUG("WM_SIZE");
			return 0;
		case WM_MOUSEMOVE:
			{
				int32_t posX = GET_X_LPARAM(lParam);
				int32_t posY = GET_Y_LPARAM(lParam);
				EWOL_DEBUG("WM_MOUSEMOVE : (" << posX << "," << posY << ")");
			}
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

