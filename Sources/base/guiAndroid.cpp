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
#include <base/guiAndroid.h>



#include <app.h>

#undef __class__
#define __class__ "AndroidJNI"


int   gAppAlive   = 1;

static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static int32_t currentWidth = 320;
static int32_t currentHeight = 480;


static long _getTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

extern "C"
{
	
	/* Call to initialize the graphics state */
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeInit( JNIEnv*  env )
	{
		EWOL_INFO("Init : Start All Application");
		appInit();
		gAppAlive    = 1;
		sDemoStopped = 0;
		sTimeOffsetInit = 0;
	}
	
	
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		currentWidth = w;
		currentHeight = h;
		EWOL_INFO("Resize w=" << w << " h=" << h);
	}
	
	/* Call to finalize the graphics state */
	void Java_com_example_ewolAbstraction_EwolRenderer_nativeDone( JNIEnv*  env )
	{
		EWOL_INFO("Renderer : Close All Application");
		appDeinit();
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
			appMove(x,y);
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
	
		appRender(curTime, currentWidth, currentHeight);
	}

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
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("Start Running");
		//myX11Access->Run();
		EWOL_INFO("Stop Running");
	} else {
		EWOL_CRITICAL("Can not Run X11 ==> not init ... ");
	}
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
		//myX11Access->Setwindow(newOne);
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
