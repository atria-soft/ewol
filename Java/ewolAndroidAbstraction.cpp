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
#include <Debug.h>

#include <ewol/threadMsg.h>

// declaration of the ewol android abstraction ...

void EWOL_SystemStart(void);
void EWOL_SystemStop(void);
void EWOL_ThreadSetArchiveDir(int mode, const char* str);
void EWOL_ThreadResize(int w, int h );
void EWOL_ThreadEventInputMotion(int pointerID, float x, float y);
void EWOL_ThreadEventInputState(int pointerID, bool isUp, float x, float y);
void EWOL_NativeRender(void);
void EWOL_NativeGLDestroy(void);


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
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Create                   **");
		APPL_DEBUG("*******************************************");
		EWOL_SystemStart();
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnStart( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Start                    **");
		APPL_DEBUG("*******************************************");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnReStart( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Re-Start                 **");
		APPL_DEBUG("*******************************************");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnResume( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Resume                   **");
		APPL_DEBUG("*******************************************");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnPause( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Pause                    **");
		APPL_DEBUG("*******************************************");
		// All the openGl has been destroyed ...
		// TODO : Mark all the texture to be reloaded ...
		EWOL_NativeGLDestroy();
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnStop( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Stop                     **");
		APPL_DEBUG("*******************************************");
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityOnDestroy( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Destroy                  **");
		APPL_DEBUG("*******************************************");
		EWOL_SystemStop();
	}
	
	
	/* Call to initialize the graphics state */
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeInit( JNIEnv*  env )
	{
		
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		EWOL_ThreadResize(w, h);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		EWOL_ThreadEventInputMotion(pointerID+1, x, y);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		EWOL_ThreadEventInputState(pointerID+1, isUp, x, y);
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeApplicationInit( JNIEnv* env)
	{
		//ewol::threadMsg::SendMessage(androidJniMsg, JNI_APP_INIT);
		//EWOL_NativeApplicationInit();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeApplicationUnInit( JNIEnv* env)
	{
		//ewol::threadMsg::SendMessage(androidJniMsg, JNI_APP_UN_INIT);
		//EWOL_NativeApplicationUnInit();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeRender( JNIEnv*  env )
	{
		EWOL_NativeRender();
	}

}

