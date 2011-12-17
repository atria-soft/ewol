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

extern "C"
{
	
	/* Call to initialize the graphics state */
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeInit( JNIEnv*  env )
	{
		EWOL_NativeInit();
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		EWOL_NativeResize(w, h);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeDone( JNIEnv*  env )
	{
		EWOL_NativeDone();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		EWOL_NativeEventInputMotion(pointerID, x, y);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventInputState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		EWOL_NativeEventInputState(pointerID, isUp, x, y);
	}
	
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeEventUnknow( JNIEnv* env, jobject  thiz, jint ID)
	{
		EWOL_NativeEventUnknow(ID);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeParamSetArchiveDir( JNIEnv* env, jobject  thiz, jint mode, jstring myString)
	{
		const char* str = env->GetStringUTFChars(myString,0);
		EWOL_NativeParamSetArchiveDir(mode, str);
		//env->ReleaseStringUTFChars(str,myString,0);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeApplicationInit( JNIEnv* env)
	{
		EWOL_NativeApplicationInit();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolGLSurfaceView_nativeApplicationUnInit( JNIEnv* env)
	{
		EWOL_NativeApplicationUnInit();
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE___EwolRenderer_nativeRender( JNIEnv*  env )
	{
		EWOL_NativeRender();
	}

}

