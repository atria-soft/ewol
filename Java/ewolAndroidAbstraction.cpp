/**
 *******************************************************************************
 * @file ewolAndroidAbstraction.cpp
 * @brief User abstraction for Android (Sources)
 * @author Edouard DUPIN
 * @date 14/12/2011
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
#include <stdint.h>
#include <pthread.h>
#include <tools_debug.h>

#include <ewol/threadMsg.h>

// declaration of the ewol android abstraction ...
void EWOL_NativeInit(void);
void EWOL_NativeResize(int w, int h );
void EWOL_NativeDone(void);
void EWOL_NativeEventInputMotion(int pointerID, float x, float y );
void EWOL_NativeEventInputState(int pointerID, bool isUp, float x, float y );
void EWOL_NativeEventUnknow(int ID);
void EWOL_NativeParamSetArchiveDir(int mode, const char* str);
void EWOL_NativeApplicationInit(void);
void EWOL_NativeApplicationUnInit(void);
void EWOL_NativeRender(void);

static ewol::threadMsg::threadMsg_ts    androidJniMsg;
static pthread_t                        androidJniThread;
static pthread_attr_t                   androidJniThreadAttr;

enum {
	JNI_NONE,
	JNI_INIT,
	JNI_DONE,
	JNI_RESIZE,
	JNI_INPUT_MOTION,
	JNI_INPUT_STATE,
	JNI_DATA_ARCHIVE_DIR,
	JNI_APP_INIT,
	JNI_APP_UN_INIT,
	JNI_APP_RENDERER,
	
};


#include <unistd.h>


static void* BaseAppEntry(void* param)
{
	EDN_DEBUG("start Ewol Basic thread ...");
	ewol::threadMsg::tmppp5656("start Ewol Basic thread ...");
	while(1) {
		ewol::threadMsg::threadMsgContent_ts data;
		ewol::threadMsg::tmppp5656("start waiting");
		ewol::threadMsg::WaitMessage(androidJniMsg, data);
		ewol::threadMsg::tmppp5656("end waiting");
		switch (data.type) {
			case JNI_NONE:
				ewol::threadMsg::tmppp5656("JNI_NONE");
				break;
			case JNI_INIT:
				ewol::threadMsg::tmppp5656("JNI_INIT");
				break;
			case JNI_DONE:
				ewol::threadMsg::tmppp5656("JNI_DONE");
				break;
			case JNI_RESIZE:
				ewol::threadMsg::tmppp5656("JNI_RESIZE");
				break;
			case JNI_INPUT_MOTION:
				ewol::threadMsg::tmppp5656("JNI_INPUT_MOTION");
				break;
			case JNI_INPUT_STATE:
				ewol::threadMsg::tmppp5656("JNI_INPUT_STATE");
				break;
			case JNI_DATA_ARCHIVE_DIR:
				ewol::threadMsg::tmppp5656("JNI_DATA_ARCHIVE_DIR");
				break;
			case JNI_APP_INIT:
				ewol::threadMsg::tmppp5656("JNI_APP_INIT");
				break;
			case JNI_APP_UN_INIT:
				ewol::threadMsg::tmppp5656("JNI_APP_UN_INIT");
				break;
			case JNI_APP_RENDERER:
				ewol::threadMsg::tmppp5656("JNI_APP_RENDERER");
				break;
			default:
				ewol::threadMsg::tmppp5656("UNKNOW");
				break;
		}
	}
	EDN_DEBUG("End Ewol Basic thread ...");
	pthread_exit(NULL);
}



void BaseInit(void)
{
	static bool isInit = false;
	if (false == isInit) {
		// create interface mutex :
		ewol::threadMsg::Init(androidJniMsg);
		// init the thread :
		//pthread_attr_init(&androidJniThreadAttr);
		//pthread_attr_setdetachstate(&androidJniThreadAttr, PTHREAD_CREATE_DETACHED);
		//pthread_create(&androidJniThread, &androidJniThreadAttr, BaseAppEntry, NULL);
		pthread_create(&androidJniThread, NULL, BaseAppEntry, NULL);
		isInit = true;
	}
}
extern "C"
{
	
	/* Call to initialize the graphics state */
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeInit( JNIEnv*  env )
	{
		BaseInit();
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INIT);
		//EWOL_NativeInit();
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_RESIZE);
		//EWOL_NativeResize(w, h);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeDone( JNIEnv*  env )
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_DONE);
		//EWOL_NativeDone();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INPUT_MOTION);
		//EWOL_NativeEventInputMotion(pointerID, x, y);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INPUT_STATE);
		//EWOL_NativeEventInputState(pointerID, isUp, x, y);
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventUnknow( JNIEnv* env, jobject  thiz, jint ID)
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_DATA_ARCHIVE_DIR);
		//EWOL_NativeEventUnknow(ID);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeParamSetArchiveDir( JNIEnv* env, jobject  thiz, jint mode, jstring myString)
	{
		ewol::threadMsg::SendMessage(androidJniMsg, 0);
		const char* str = env->GetStringUTFChars(myString,0);
		//EWOL_NativeParamSetArchiveDir(mode, str);
		//env->ReleaseStringUTFChars(str,myString,0);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeApplicationInit( JNIEnv* env)
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_APP_INIT);
		//EWOL_NativeApplicationInit();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeApplicationUnInit( JNIEnv* env)
	{
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_APP_UN_INIT);
		//EWOL_NativeApplicationUnInit();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeRender( JNIEnv*  env )
	{
		//pthread_mutex_lock(&androidJniMutex);
		//EWOL_NativeRender();
		//messageID = JNI_APP_RENDERER;
		//pthread_cond_signal(&androidJniCond);
		//pthread_mutex_unlock(&androidJniMutex);
	}

}

