/**
 *******************************************************************************
 * @file guiAndroid.cpp
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



#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>

#include <ewol/Debug.h>
#include <etk/UString.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>
#include <ewol/ewol.h>

#include <ewol/importgl.h>
#include <ewol/Texture.h>

#undef __class__
#define __class__ "AndroidJNI"
int32_t separateClickTime = 800000;
int32_t offsetMoveClicked = 40000;
int32_t offsetMoveClickedDouble = 300000;

extern etkFloat_t     gui_width;
extern etkFloat_t     gui_height;
extern ewol::Windows* gui_uniqueWindows;

int64_t GetCurrentTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    //EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
    return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_usec);
}


void Draw(void);
#define OFFSET_MOVE_CLICKED             (20)
#define OFFSET_MOVE_CLICKED_DOUBLE      (40)
#undef SEPARATED_CLICK_TIME
#define SEPARATED_CLICK_TIME          (300)

//bool    inputIsPressed[NB_MAX_INPUT];// = {false, false, false};
int32_t m_previousBouttonId = -1;
int32_t m_previousDown_x = -1;
int32_t m_previousDown_y = -1;
int32_t m_previous_x = -1;
int32_t m_previous_y = -1;
int64_t m_previousTime = 0;
bool    m_previousDouble = false;


static etk::UString l_clipBoardPrimary(""); // local copy of the selection
static etk::UString l_clipBoardStd(""); // local copy of the clipboard


static etkFloat_t gTriangleVertices[] = { 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 200.0f };
static etkFloat_t gTriangleVertices5[] = { 200.0f, 200.0f, 100.0f, 200.0f, 200.0f, 100.0f,
                                           200.0f, 200.0f, 300.0f, 200.0f, 200.0f, 300.0f };

void EWOL_NativeRender(void)
{
	ewol::texture::UpdateContext();
	//EWOL_DEBUG("redraw (" << gui_width << "," << gui_height << ")");
	if(NULL == gui_uniqueWindows) {
		// set the size of the open GL system
		glViewport(0,0,gui_width,gui_height);
		
		// Clear the screen with transparency ...
		glClearColor(0.0,0.0,0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0., width, 0., -height, 1., 20.);
		glOrthoEwol(-gui_width/2, gui_width/2, gui_height/2, -gui_height/2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//glTranslatef(0, -height/2, -5);
		glTranslatef(-gui_width/2, -gui_height/2, -1.0);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
		
		glEnableClientState( GL_VERTEX_ARRAY );
		
		//LOGI("engine_draw_frame (%d,%d)",width,height);
		
		glColor4f(0.0, 1.0, 1.0, 1.0);
		glVertexPointer(2, oglTypeFloat_t, 0, gTriangleVertices5 );
		glDrawArrays( GL_TRIANGLES, 0, 6);
		static int vallllll = 0;
		static float transparency = 0.0;
		if (vallllll <= 1) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(1.0, 0.0, 0.0, 1.0);
			} else {
				glColor4f(1.0, 0.0, 0.0, transparency);
			}
		} else if (vallllll <= 2) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(1.0, 1.0, 0.0, 1.0);
			} else {
				glColor4f(1.0, 1.0, 0.0, transparency);
			}
		} else if (vallllll <= 3) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(0.0, 1.0, 0.0, 1.0);
			} else {
				glColor4f(0.0, 1.0, 0.0, transparency);
			}
		} else if (vallllll <= 4) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(0.0, 1.0, 1.0, 1.0);
			} else {
				glColor4f(0.0, 1.0, 1.0, transparency);
			}
		} else if (vallllll <= 5) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(0.0, 0.0, 1.0, 1.0);
			} else {
				glColor4f(0.0, 0.0, 1.0, transparency);
			}
		} else {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll = 0;
				transparency = 0.0;
				glColor4f(1.0, 0.0, 1.0, 1.0);
			} else {
				glColor4f(1.0, 0.0, 1.0, transparency);
			}
		}
		glVertexPointer(2, oglTypeFloat_t, 0, gTriangleVertices );
		glDrawArrays( GL_TRIANGLES, 0, 3);
		
		glDisableClientState( GL_VERTEX_ARRAY );
	
		glDisable(GL_BLEND);
	} else {
		EWOL_GenericDraw(true);
		//EWOL_GenericDraw(false);
	}
	glFlush();
}



void ewol::SetTitle(etk::UString title)
{
	// can not set the title in Android ...
}

// ClipBoard AREA :
void guiAbstraction::ClipBoardGet(etk::UString &newData, clipBoardMode_te mode)
{
	EWOL_INFO("Request Get of a clipboard : " << mode << " size=" << newData.Size() );
	newData = "";
	switch (mode)
	{
		case CLIPBOARD_MODE_PRIMARY:
			// get our own buffer ... (no current selectin on Android ...
			newData = l_clipBoardPrimary;
			break;
		case CLIPBOARD_MODE_STD:
			EWOL_TODO("implement on Android");
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
	EWOL_VERBOSE("Request set of a clipboard : " << mode << " size=" << newData.Size() );
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
				// Request the clipBoard :
				EWOL_TODO("implement on Android copy the copy data");
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}

#undef __class__
#define __class__ "guiAbstraction"

void guiAbstraction::Stop(void)
{
	// TODo : send a message to the android system to stop ...
}


void guiAbstraction::ChangeSize(int32_t w, int32_t h)
{
	// nothing to do with Android
}

void guiAbstraction::ChangePos(int32_t x, int32_t y)
{
	// nothing to do with Android
}

void guiAbstraction::GetAbsPos(int32_t & x, int32_t & y)
{
}

bool guiAbstraction::IsPressedInput(int32_t inputID)
{
	//if (true == guiAbstractionIsInit) {
	//	return myX11Access->IsPressedInput(inputID);
	//} else {
	//	EWOL_CRITICAL("X11 ==> not init ... ");
		return false;
	//}
}

#include <ewol/ewol.h>
int32_t ewol::CmdLineNb(void)
{
	return 0;
}

etk::UString ewol::CmdLineGet(int32_t id)
{
	return "";
}