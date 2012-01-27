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
	JNI_UN_INIT,
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

typedef struct {
	int w;
	int h;
} eventResize_ts;

typedef struct {
	int pointerID;
	float x;
	float y;
} eventInputMotion_ts;

typedef struct {
	int pointerID;
	bool state;
	float x;
	float y;
} eventInputState_ts;



static void* BaseAppEntry(void* param)
{
	bool requestEndProcessing = false;
	EDN_DEBUG("start Ewol Basic thread ...");
	while(false == requestEndProcessing) {
		ewol::threadMsg::threadMsgContent_ts data;
		ewol::threadMsg::WaitMessage(androidJniMsg, data);
		switch (data.type) {
			case JNI_NONE:
				EDN_DEBUG("Receive MSG : JNI_NONE");
				break;
			case JNI_INIT:
				EDN_DEBUG("Receive MSG : JNI_INIT");
				EWOL_NativeApplicationInit();
				break;
			case JNI_UN_INIT:
				EDN_DEBUG("Receive MSG : JNI_UN_INIT");
				EWOL_NativeApplicationUnInit();
				requestEndProcessing = true;
				break;
			case JNI_DONE:
				EDN_DEBUG("Receive MSG : JNI_DONE");
				break;
			case JNI_RESIZE:
				EDN_DEBUG("Receive MSG : JNI_RESIZE");
				{
					eventResize_ts * tmpData = (eventResize_ts*)data.data;
					EWOL_NativeResize(tmpData->w, tmpData->h);
					EWOL_NativeInit();
				}
				break;
			case JNI_INPUT_MOTION:
				EDN_DEBUG("Receive MSG : JNI_INPUT_MOTION");
				{
					eventInputMotion_ts * tmpData = (eventInputMotion_ts*)data.data;
					EWOL_NativeEventInputMotion(tmpData->pointerID, tmpData->x, tmpData->y);
				}
				break;
			case JNI_INPUT_STATE:
				EDN_DEBUG("Receive MSG : JNI_INPUT_STATE");
				{
					eventInputState_ts * tmpData = (eventInputState_ts*)data.data;
					EWOL_NativeEventInputState(tmpData->pointerID, tmpData->state, tmpData->x, tmpData->y);
				}
				break;
			case JNI_DATA_ARCHIVE_DIR:
				EDN_DEBUG("Receive MSG : JNI_DATA_ARCHIVE_DIR");
				break;
			case JNI_APP_INIT:
				EDN_DEBUG("Receive MSG : JNI_APP_INIT");
				break;
			case JNI_APP_UN_INIT:
				EDN_DEBUG("Receive MSG : JNI_APP_UN_INIT");
				break;
			case JNI_APP_RENDERER:
				EDN_DEBUG("Receive MSG : JNI_APP_RENDERER");
				break;
			default:
				EDN_DEBUG("Receive MSG : UNKNOW");
				break;
		}
	}
	EDN_DEBUG("End Ewol Basic thread ...");
	pthread_exit(NULL);
}

bool isGlobalSystemInit = false;

void BaseInit(void)
{
	if (false == isGlobalSystemInit) {
		// create interface mutex :
		ewol::threadMsg::Init(androidJniMsg);
		// init the thread :
		pthread_create(&androidJniThread, NULL, BaseAppEntry, NULL);
		isGlobalSystemInit = true;
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
	}
}

void BaseUnInit(void)
{
	if (true == isGlobalSystemInit) {
		isGlobalSystemInit = false;
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_UN_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
		
		EDN_DEBUG("Wait end of the thread ...");
		// Wait end of the thread
		pthread_join(androidJniThread, NULL);
		ewol::threadMsg::UnInit(androidJniMsg);
	}
}

extern "C"
{
	/* Call to initialize the graphics state */
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityParamSetArchiveDir( JNIEnv* env, jobject  thiz, jint mode, jstring myString)
	{
		// direct setting of the date in the string system ...
		const char* str = env->GetStringUTFChars(myString,0);
		EWOL_NativeParamSetArchiveDir(mode, str);
		//env->ReleaseStringUTFChars(str,myString,0);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnCreate( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Create");
		BaseInit();
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnStart( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Start");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnReStart( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Re-Start");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnResume( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Resume");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnPause( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Pause");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnStop( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Stop");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnDestroy( JNIEnv*  env )
	{
		EDN_DEBUG("Activity On Destroy");
		BaseUnInit();
	}
	
	
	/* Call to initialize the graphics state */
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeInit( JNIEnv*  env )
	{
		
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		eventResize_ts tmpData;
		tmpData.w = w;
		tmpData.h = h;
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_RESIZE, ewol::threadMsg::MSG_PRIO_MEDIUM, &tmpData, sizeof(eventResize_ts) );
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		eventInputMotion_ts tmpData;
		tmpData.pointerID = pointerID;
		tmpData.x = x;
		tmpData.y = y;
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INPUT_MOTION, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputMotion_ts) );
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		eventInputState_ts tmpData;
		tmpData.pointerID = pointerID;
		tmpData.state = isUp;
		tmpData.x = x;
		tmpData.y = y;
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INPUT_STATE, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputState_ts) );
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventUnknow( JNIEnv* env, jobject  thiz, jint ID)
	{
		//EWOL_NativeEventUnknow(ID);
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
		EWOL_NativeRender();
	}

}

