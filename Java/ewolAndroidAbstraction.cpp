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
void EWOL_NativeDone(void);
void EWOL_NativeApplicationInit(void);
void EWOL_NativeApplicationUnInit(void);
void EWOL_NativeRender(void);


extern "C"
{
	/* Call to initialize the graphics state */
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityParamSetArchiveDir( JNIEnv* env, jobject  thiz, jint mode, jstring myString)
	{
		// direct setting of the date in the string system ...
		const char* str = env->GetStringUTFChars(myString,0);
		EWOL_ThreadSetArchiveDir(mode, str);
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

