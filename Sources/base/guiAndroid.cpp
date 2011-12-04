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
#include <etk/String.h>
#include <ewol/WidgetManager.h>
#include <base/gui.h>
#include <ewol/ewol.h>

#include <importgl.h>


#include <app.h>

#undef __class__
#define __class__ "AndroidJNI"


int   gAppAlive   = 1;

static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static etkFloat_t m_width = 320;
static etkFloat_t m_height = 480;

ewol::Windows* m_uniqueWindows = NULL;

static long _getTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}


void Draw(void);

extern "C"
{
	
	/* Call to initialize the graphics state */
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeInit( JNIEnv*  env )
	{
		EWOL_INFO("Init : Start All Application");
		ewol::Init(0, NULL);
		APP_Init(0, NULL);
		gAppAlive    = 1;
		sDemoStopped = 0;
		sTimeOffsetInit = 0;
	}
	
	
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		m_width = w;
		m_height = h;
		EWOL_INFO("Resize w=" << w << " h=" << h);
		if (NULL != m_uniqueWindows) {
			m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
		}
	}
	
	/* Call to finalize the graphics state */
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeDone( JNIEnv*  env )
	{
		EWOL_INFO("Renderer : Close All Application");
		// unset all windows
		ewol::DisplayWindows(NULL);
		// call application to uninit
		APP_UnInit();
		// uninit Ewol
		ewol::UnInit();
	}
	
	/* This is called to indicate to the render loop that it should
	 * stop as soon as possible.
	 */
	void Java_com_example_ewolAbstraction_EwolGLSurfaceView_nativePause( JNIEnv* env )
	{
		sDemoStopped = !sDemoStopped;
		if (sDemoStopped) {
			/* we paused the animation, so store the current
			 * time in sTimeStopped for future nativeRender calls */
			sTimeStopped = _getTime();
		} else {
			/* we resumed the animation, so adjust the time offset
			 * to take care of the pause interval. */
			sTimeOffset -= _getTime() - sTimeStopped;
		}
	}
	
	
	void Java_com_example_ewolAbstraction_EwolGLSurfaceView_nativeEventInputMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		EWOL_INFO("Event : Input Motion ID=" << pointerID << " x=" << x << " y=" << y);
		if (0 == pointerID) {
			//appMove(x,y);
			if(NULL != m_uniqueWindows) {
				m_uniqueWindows->GenEventInput(ewol::FLAG_EVENT_INPUT_1, ewol::EVENT_INPUT_TYPE_DOWN, (etkFloat_t)x, (etkFloat_t)y);
				m_uniqueWindows->GenEventInput(ewol::FLAG_EVENT_INPUT_1 | ewol::FLAG_EVENT_INPUT_CLICKED, ewol::EVENT_INPUT_TYPE_SINGLE, (etkFloat_t)x, (etkFloat_t)y);
				m_uniqueWindows->GenEventInput(ewol::FLAG_EVENT_INPUT_1, ewol::EVENT_INPUT_TYPE_UP, (etkFloat_t)x, (etkFloat_t)y);
				m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
			}
		}
	}
	
	void Java_com_example_ewolAbstraction_EwolGLSurfaceView_nativeEventInputState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp)
	{
		if (isUp) {
			EWOL_INFO("Event : Input ID=" << pointerID << " [DOWN]");
		} else {
			EWOL_INFO("Event : Input ID=" << pointerID << " [UP]");
		}
	}
	
	void Java_com_example_ewolAbstraction_EwolGLSurfaceView_nativeEventUnknow( JNIEnv* env, jobject  thiz, jint eventID)
	{
		EWOL_WARNING("Event : Unknow ID=" << eventID);
	}
	
	/* Call to render the next GL frame */
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeRender( JNIEnv*  env )
	{
		long   curTime;
	
		/* NOTE: if sDemoStopped is TRUE, then we re-render the same frame
		 *       on each iteration.
		 */
		if (sDemoStopped) {
			curTime = sTimeStopped + sTimeOffset;
		} else {
			curTime = _getTime() + sTimeOffset;
			if (sTimeOffsetInit == 0) {
				sTimeOffsetInit = 1;
				sTimeOffset     = -curTime;
				curTime         = 0;
			}
		}
	
		Draw();
	}

}




static void Setwindow(ewol::Windows* newWindows)
{
	m_uniqueWindows = newWindows;
	if (NULL != m_uniqueWindows) {
		m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
	}
}



static etkFloat_t gTriangleVertices[] = { 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 200.0f };
static etkFloat_t gTriangleVertices5[] = { 200.0f, 200.0f, 100.0f, 200.0f, 200.0f, 100.0f,
                                           200.0f, 200.0f, 300.0f, 200.0f, 200.0f, 300.0f };

void Draw(void)
{
	//EWOL_DEBUG("redraw (" << m_width << "," << m_height << ")");
	if(NULL == m_uniqueWindows) {
		// set the size of the open GL system
		glViewport(0,0,m_width,m_height);
		
		// Clear the screen with transparency ...
		glClearColor(0.0,0.0,0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0., width, 0., -height, 1., 20.);
		glOrtho(-m_width/2, m_width/2, m_height/2, -m_height/2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//glTranslatef(0, -height/2, -5);
		glTranslatef(-m_width/2, -m_height/2, -1.0);
		
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
		m_uniqueWindows->SysDraw();
	}
	glFlush();
}




#undef __class__
#define __class__ "guiAbstraction"

static bool guiAbstractionIsInit = false;
//static guiAbstraction::X11systemInterface * myX11Access = NULL;


void guiAbstraction::Init(int32_t argc, char *argv[])
{
	if (false == guiAbstractionIsInit) {
		// set the gui is init :
		guiAbstractionIsInit = true;
		EWOL_INFO("INIT for X11 environement");
		//myX11Access = new guiAbstraction::X11systemInterface();
	} else {
		EWOL_CRITICAL("Can not INIT X11 ==> already init before");
	}
}


void guiAbstraction::Run(void)
{
	EWOL_INFO("Noting to run in android mode ...");
}

void guiAbstraction::Stop(void)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->Stop();
	} else {
		EWOL_CRITICAL("Can not Stop X11 ==> not init ... ");
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows * newOne)
{
	if (true == guiAbstractionIsInit) {
		Setwindow(newOne);
	} else {
		EWOL_CRITICAL("Can not set Windows X11 ==> not init ... ");
	}
}

void guiAbstraction::UnInit(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("UN-INIT for X11 environement");
		//if (NULL != myX11Access) {
		//	delete(myX11Access);
		//}
		guiAbstractionIsInit = false;
	} else {
		EWOL_CRITICAL("Can not Un-Init X11 ==> not init ... ");
	}
}


void guiAbstraction::ChangeSize(int32_t w, int32_t h)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->ChangeSize(w, h);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::ChangePos(int32_t x, int32_t y)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->ChangePos(x, y);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::GetAbsPos(int32_t & x, int32_t & y)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->GetAbsPos(x, y);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::StartResizeSystem(void)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->StartResizeSystem();
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::StartMoveSystem(void)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->StartMoveSystem();
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
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

// never had main in android ...
/*
int main(int argc, char *argv[])
{
	return -1;
}
*/

void glOrtho(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal)
{
	GLfloat myMatrix[4*4];
	int iii;
	for(iii=0; iii<4*4 ; iii++) {
		myMatrix[iii] = 0;
	}
	myMatrix[0] = 2.0 / (right - left);
	myMatrix[5] = 2.0 / (top - bottom);
	myMatrix[10] = -2.0 / (farVal - nearVal);
#if 1
	myMatrix[3]  = -1*(right + left) / (right - left);
	myMatrix[7]  = -1*(top + bottom) / (top - bottom);
	myMatrix[11] = -1*(farVal + nearVal) / (farVal - nearVal);
#else
	// test if matrix is not corectly instanciate ...
	myMatrix[12]  = -1*(right + left) / (right - left);
	myMatrix[13]  = -1*(top + bottom) / (top - bottom);
	myMatrix[14] = -1*(farVal + nearVal) / (farVal - nearVal);
#endif
	myMatrix[15] = 1;

	glLoadMatrixf(myMatrix);


}
