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

// get a resources from the java environement : 
static JNIEnv*   JavaVirtualMachinePointer = NULL; // the JVM
static jclass    javaClassActivity = 0;            // main activity class (android ...)
static jmethodID javaClassActivityEntryPoint = 0;  // basic methode to call ...
// generic classes
static jclass    javaDefaultClassString = 0;       // default string class


static JavaVM* g_JavaVM = NULL;


void SendSystemMessage(const char * dataString)
{
	APPL_DEBUG("C->java : send message to the java : \"" << dataString << "\"");
	if (NULL == g_JavaVM) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}
	JNIEnv *JavaVirtualMachinePointer;
	int status = g_JavaVM->GetEnv((void **) &JavaVirtualMachinePointer, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		status = g_JavaVM->AttachCurrentThread(&JavaVirtualMachinePointer, NULL);
		if (status != JNI_OK) {
			APPL_DEBUG("C->java : AttachCurrentThread failed : " << status);
			return;
		}
		jclass javaClassActivity = JavaVirtualMachinePointer->FindClass("com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__.__PROJECT_NAME__" );
		if (javaClassActivity == 0) {
			APPL_DEBUG("C->java : Can't find com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__.__PROJECT_NAME__ class");
			// remove access on the virtual machine : 
			JavaVirtualMachinePointer = NULL;
			return;
		}
		jmethodID javaClassActivityEntryPoint = JavaVirtualMachinePointer->GetStaticMethodID(javaClassActivity, "eventFromCPP", "([Ljava/lang/String;)V" );
		if (javaClassActivityEntryPoint == 0) {
			APPL_DEBUG("C->java : Can't find com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__.__PROJECT_NAME__.eventFromCPP" );
			// remove access on the virtual machine : 
			JavaVirtualMachinePointer = NULL;
			return;
		}
		jclass javaDefaultClassString = JavaVirtualMachinePointer->FindClass("java/lang/String" );
		if (javaDefaultClassString == 0) {
			APPL_DEBUG("C->java : Can't find java/lang/String" );
			// remove access on the virtual machine : 
			JavaVirtualMachinePointer = NULL;
			return;
		}
		// create the string to the java
		jstring jstr = JavaVirtualMachinePointer->NewStringUTF(dataString);
		if (jstr == 0) {
			APPL_DEBUG("C->java : Out of memory" );
			return;
		}
		// create argument list
		jobjectArray args = JavaVirtualMachinePointer->NewObjectArray(1, javaDefaultClassString, jstr);
		if (args == 0) {
			APPL_DEBUG("C->java : Out of memory" );
			return;
		}
		//Call java ...
		JavaVirtualMachinePointer->CallStaticVoidMethod(javaClassActivity, javaClassActivityEntryPoint, args);
		
		// manage execption : 
		if (JavaVirtualMachinePointer->ExceptionOccurred()) {
			JavaVirtualMachinePointer->ExceptionDescribe();
		}
	} else {
		APPL_DEBUG("C->java : do nothing ... ");
	}


/*
	if (NULL == JavaVirtualMachinePointer) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}
	// create the string to the java
	jstring jstr = JavaVirtualMachinePointer->NewStringUTF(dataString);
	if (jstr == 0) {
		APPL_DEBUG("C->java : Out of memory" );
		return;
	}
	// create argument list
	jobjectArray args = JavaVirtualMachinePointer->NewObjectArray(1, javaDefaultClassString, jstr);
	if (args == 0) {
		APPL_DEBUG("C->java : Out of memory" );
		return;
	}
	//Call java ...
	JavaVirtualMachinePointer->CallStaticVoidMethod(javaClassActivity, javaClassActivityEntryPoint, args);
	
	// manage execption : 
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		JavaVirtualMachinePointer->ExceptionDescribe();
	}
*/
}


extern "C"
{
	// JNI OnLoad
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved)
	{
		// get the java virtual machine handle ...
		g_JavaVM = jvm;
		APPL_DEBUG("JNI-> load the jvm ..." );
		return JNI_VERSION_1_6;
	}
	// JNI OnUnLoad
	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
	{
		g_JavaVM = NULL;
		APPL_DEBUG("JNI-> Un-load the jvm ..." );
	}

	
	/* Call to initialize the graphics state */
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityParamSetArchiveDir( JNIEnv* env, jobject  thiz, jint mode, jstring myString)
	{
		// direct setting of the date in the string system ...
		const char* str = env->GetStringUTFChars(myString,0);
		EWOL_ThreadSetArchiveDir(mode, str);
		//env->ReleaseStringUTFChars(str,myString,0);
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivitySetJavaVortualMachineStart( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Set JVM Pointer                      **");
		APPL_DEBUG("*******************************************");
		JavaVirtualMachinePointer = env;
		// get default needed all time elements : 
		if (NULL != JavaVirtualMachinePointer) {
			javaClassActivity = JavaVirtualMachinePointer->FindClass("com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__.__PROJECT_NAME__" );
			if (javaClassActivity == 0) {
				APPL_DEBUG("C->java : Can't find com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__.__PROJECT_NAME__ class");
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaClassActivityEntryPoint = JavaVirtualMachinePointer->GetStaticMethodID(javaClassActivity, "eventFromCPP", "([Ljava/lang/String;)V" );
			if (javaClassActivityEntryPoint == 0) {
				APPL_DEBUG("C->java : Can't find com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__.__PROJECT_NAME__.eventFromCPP" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaDefaultClassString = JavaVirtualMachinePointer->FindClass("java/lang/String" );
			if (javaDefaultClassString == 0) {
				APPL_DEBUG("C->java : Can't find java/lang/String" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
		}
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivitySetJavaVortualMachineStop( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Remove JVM Pointer                   **");
		APPL_DEBUG("*******************************************");
		JavaVirtualMachinePointer = NULL;
	}
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___TouchEvent( JNIEnv*  env )
	{
		APPL_DEBUG(" ==> Touch Event");
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
		//SendSystemMessage(" testmessages ... ");
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

