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
#include <ewol/base/MainThread.h>

#include <ewol/threadMsg.h>
#include <ewol/Audio/audio.h>

// declaration of the ewol android abstraction ...
void EWOL_NativeRender(void);
void EWOL_NativeGLDestroy(void);

// get a resources from the java environement : 
static JNIEnv*   JavaVirtualMachinePointer = NULL; // the JVM
static jclass    javaClassActivity = 0;            // main activity class (android ...)
static jobject   javaObjectActivity = 0;
static jmethodID javaClassActivityEntryPoint = 0;  // basic methode to call ...
static jmethodID javaClassActivityEntryPoint__CPP_keyboardShow = 0;  // basic methode to call ...
static jmethodID javaClassActivityEntryPoint__CPP_keyboardHide = 0;  // basic methode to call ...
// generic classes
static jclass    javaDefaultClassString = 0;       // default string class


static JavaVM* g_JavaVM = NULL;

// jni doc : /usr/lib/jvm/java-1.6.0-openjdk/include


void SendJava_KeyboardShow(bool showIt)
{
	APPL_DEBUG("C->java : call java");
	if (NULL == g_JavaVM) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}
	JNIEnv *JavaVirtualMachinePointer_tmp;
	int status = g_JavaVM->GetEnv((void **) &JavaVirtualMachinePointer_tmp, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		JavaVMAttachArgs lJavaVMAttachArgs;
		lJavaVMAttachArgs.version = JNI_VERSION_1_6;
		lJavaVMAttachArgs.name = "EwolNativeThread";
		lJavaVMAttachArgs.group = NULL; 
		status = g_JavaVM->AttachCurrentThread(&JavaVirtualMachinePointer_tmp, &lJavaVMAttachArgs);
		if (status != JNI_OK) {
			APPL_DEBUG("C->java : AttachCurrentThread failed : " << status);
			return;
		}
		if (JavaVirtualMachinePointer->ExceptionOccurred()) {
			APPL_DEBUG("C->java : EXEPTION ...");
			JavaVirtualMachinePointer->ExceptionDescribe();
			JavaVirtualMachinePointer->ExceptionClear();
		}
	}
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		APPL_DEBUG("C->java : EXEPTION ...");
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}

	if (NULL == JavaVirtualMachinePointer) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}

	//Call java ...
	if (true == showIt) {
		JavaVirtualMachinePointer->CallVoidMethod(javaObjectActivity, javaClassActivityEntryPoint__CPP_keyboardShow);
	} else {
		JavaVirtualMachinePointer->CallVoidMethod(javaObjectActivity, javaClassActivityEntryPoint__CPP_keyboardHide);
	}

	// manage execption : 
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		APPL_DEBUG("C->java : EXEPTION ...");
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}
	
	// Finished with the JVM.
	g_JavaVM->DetachCurrentThread();
}

void SendSystemMessage(const char * dataString)
{
	APPL_DEBUG("C->java : send message to the java : \"" << dataString << "\"");
	if (NULL == g_JavaVM) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}
	JNIEnv *JavaVirtualMachinePointer_tmp;
	int status = g_JavaVM->GetEnv((void **) &JavaVirtualMachinePointer_tmp, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		JavaVMAttachArgs lJavaVMAttachArgs;
		lJavaVMAttachArgs.version = JNI_VERSION_1_6;
		lJavaVMAttachArgs.name = "EwolNativeThread";
		lJavaVMAttachArgs.group = NULL; 
		status = g_JavaVM->AttachCurrentThread(&JavaVirtualMachinePointer_tmp, &lJavaVMAttachArgs);
		if (status != JNI_OK) {
			APPL_DEBUG("C->java : AttachCurrentThread failed : " << status);
			return;
		}
	}
	APPL_DEBUG("C->java : 111");
	if (NULL == JavaVirtualMachinePointer) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}
	APPL_DEBUG("C->java : 222");
	if (NULL == dataString) {
		APPL_DEBUG("C->java : No data to send ...");
		return;
	}
	APPL_DEBUG("C->java : 333");
	// create the string to the java
	jstring jstr = JavaVirtualMachinePointer->NewStringUTF(dataString);
	if (jstr == 0) {
		APPL_DEBUG("C->java : Out of memory" );
		return;
	}
	APPL_DEBUG("C->java : 444");
	// create argument list
	jobjectArray args = JavaVirtualMachinePointer->NewObjectArray(1, javaDefaultClassString, jstr);
	if (args == 0) {
		APPL_DEBUG("C->java : Out of memory" );
		return;
	}
	APPL_DEBUG("C->java : 555");
	//Call java ...
	JavaVirtualMachinePointer->CallStaticVoidMethod(javaClassActivity, javaClassActivityEntryPoint, args);
	
	APPL_DEBUG("C->java : 666");
	// manage execption : 
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}
	
	// Finished with the JVM.
	g_JavaVM->DetachCurrentThread();
}

namespace guiAbstraction {
void SendKeyboardEvent(bool isDown, uniChar_t keyInput);
};

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
		jboolean isCopy;
		const char* str = env->GetStringUTFChars(myString, &isCopy);
		guiSystem::SetArchiveDir(mode, str);
		if (isCopy == JNI_TRUE) {
			// from here str is reset ...
			env->ReleaseStringUTFChars(myString, str);
			str = NULL;
		}
	}
	
	void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivitySetJavaVortualMachineStart( JNIEnv*  env, jclass classBase, jobject obj)
	//void Java_com___PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivitySetJavaVortualMachineStart( JNIEnv*  env)
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Set JVM Pointer                      **");
		APPL_DEBUG("*******************************************");
		JavaVirtualMachinePointer = env;
		// get default needed all time elements : 
		if (NULL != JavaVirtualMachinePointer) {
			javaClassActivity = JavaVirtualMachinePointer->FindClass("com/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__" );
			if (javaClassActivity == 0) {
				APPL_DEBUG("C->java : Can't find com/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__ class");
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			// get the activity object : 
			javaClassActivityEntryPoint = JavaVirtualMachinePointer->GetStaticMethodID(javaClassActivity, "eventFromCPP", "([Ljava/lang/String;)V" );
			if (javaClassActivityEntryPoint == 0) {
				APPL_DEBUG("C->java : Can't find com/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.eventFromCPP" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaClassActivityEntryPoint__CPP_keyboardShow = JavaVirtualMachinePointer->GetMethodID(javaClassActivity, "CPP_keyboardShow", "()V" );
			if (javaClassActivityEntryPoint__CPP_keyboardShow == 0) {
				APPL_DEBUG("C->java : Can't find com/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.CPP_keyboardShow" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaClassActivityEntryPoint__CPP_keyboardHide = JavaVirtualMachinePointer->GetMethodID(javaClassActivity, "CPP_keyboardHide", "()V" );
			if (javaClassActivityEntryPoint__CPP_keyboardHide == 0) {
				APPL_DEBUG("C->java : Can't find com/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.CPP_keyboardHide" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			//javaObjectActivity = JavaVirtualMachinePointer->NewGlobalRef(obj);
			javaObjectActivity = obj;
			
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
	void Java_org_ewol_interfaceJNI_TouchEvent( JNIEnv*  env )
	{
		APPL_DEBUG(" ==> Touch Event");
		if (env->ExceptionOccurred()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
	
	void Java_org_ewol_interfaceJNI_ActivityOnCreate( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Create                   **");
		APPL_DEBUG("*******************************************");
		guiSystem::Init();
	}
	void Java_org_ewol_interfaceJNI_ActivityOnStart( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Start                    **");
		APPL_DEBUG("*******************************************");
		//SendSystemMessage(" testmessages ... ");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnReStart( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Re-Start                 **");
		APPL_DEBUG("*******************************************");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnResume( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Resume                   **");
		APPL_DEBUG("*******************************************");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnPause( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Pause                    **");
		APPL_DEBUG("*******************************************");
		// All the openGl has been destroyed ...
		// TODO : Mark all the texture to be reloaded ...
		EWOL_NativeGLDestroy();
	}
	void Java_org_ewol_interfaceJNI_ActivityOnStop( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Stop                     **");
		APPL_DEBUG("*******************************************");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnDestroy( JNIEnv*  env )
	{
		APPL_DEBUG("*******************************************");
		APPL_DEBUG("**  Activity On Destroy                  **");
		APPL_DEBUG("*******************************************");
		guiSystem::UnInit();
	}
	
	
	
	/* **********************************************************************************************
	 * ** IO section :
	 * ********************************************************************************************** */
	void Java_org_ewol_interfaceJNI_IOInputEventMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		guiSystem::event::SetInputMotion(pointerID+1, x, y);
	}
	
	void Java_org_ewol_interfaceJNI_IOInputEventState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		guiSystem::event::SetInputState(pointerID+1, isUp, x, y);
	}
	
	void Java_org_ewol_interfaceJNI_IOMouseEventMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		guiSystem::event::SetMouseMotion(pointerID+1, x, y);
	}
	
	void Java_org_ewol_interfaceJNI_IOMouseEventState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		guiSystem::event::SetMouseState(pointerID+1, isUp, x, y);
	}
	
	void Java_org_ewol_interfaceJNI_IOUnknowEvent( JNIEnv* env, jobject  thiz, jint pointerID)
	{
		APPL_DEBUG("Unknown IO event : " << pointerID << " ???");
	}
	
	void Java_org_ewol_interfaceJNI_IOKeyboardEventMove( JNIEnv* env, jobject  thiz, jint type, jboolean isdown)
	{
		APPL_DEBUG("IO keyboard Move event : \"" << type << "\" is down=" << isdown);
	}
	
	void Java_org_ewol_interfaceJNI_IOKeyboardEventKey( JNIEnv* env, jobject  thiz, jint uniChar, jboolean isdown)
	{
		APPL_DEBUG("IO keyboard Key event : \"" << uniChar << "\" is down=" << isdown);
		guiSystem::event::keyboardKey_ts keyInput;
		keyInput.myChar = uniChar;
		keyInput.isDown = isdown;
		guiSystem::event::SetKeyboard(keyInput);
	}
	
	enum {
		SYSTEM_KEY__VOLUME_UP = 1,
		SYSTEM_KEY__VOLUME_DOWN,
		SYSTEM_KEY__MENU,
		SYSTEM_KEY__CAMERA,
		SYSTEM_KEY__HOME,
		SYSTEM_KEY__POWER,
	};
	void Java_org_ewol_interfaceJNI_IOKeyboardEventKeySystem( JNIEnv* env, jobject  thiz, jint keyVal, jboolean isdown)
	{
		switch (keyVal)
		{
			case SYSTEM_KEY__VOLUME_UP:
				APPL_DEBUG("IO keyboard Key System \"VOLUME_UP\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__VOLUME_DOWN:
				APPL_DEBUG("IO keyboard Key System \"VOLUME_DOWN\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__MENU:
				APPL_DEBUG("IO keyboard Key System \"MENU\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__CAMERA:
				APPL_DEBUG("IO keyboard Key System \"CAMERA\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__HOME:
				APPL_DEBUG("IO keyboard Key System \"HOME\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__POWER:
				APPL_DEBUG("IO keyboard Key System \"POWER\" is down=" << keyVal);
				break;
			default:
				APPL_DEBUG("IO keyboard Key System event : \"" << keyVal << "\" is down=" << isdown);
				break;
		}
	}
	
	
	/* **********************************************************************************************
	 * **  Renderer section :
	 * ********************************************************************************************** */
	void Java_org_ewol_interfaceJNI_RenderInit( JNIEnv*  env )
	{
		
	}
	
	void Java_org_ewol_interfaceJNI_RenderResize( JNIEnv* env, jobject thiz, jint w, jint h )
	{
		guiSystem::event::Resize(w, h);
	}
	
	void Java_org_ewol_interfaceJNI_RenderDraw( JNIEnv*  env )
	{
		guiSystem::Draw(true);
	}

	void Java_org_ewol_interfaceJNI_IOAudioPlayback(JNIEnv* env, void* reserved, jshortArray location, jint frameRate, jint nbChannels)
	{
		// Get the short* pointer from the Java array
		jboolean isCopy;
		jshort* dst = env->GetShortArrayElements(location, &isCopy);
		if (NULL != dst) {
			ewol::audio::GetData(dst, frameRate, nbChannels);
		}
		//APPL_DEBUG("IO Audio event request: Frames=" << frameRate << " channels=" << nbChannels);
		// TODO : Understand why it did not work corectly ...
		//if (isCopy == JNI_TRUE) {
			// Release the short* pointer
			env->ReleaseShortArrayElements(location, dst, 0);
		//}
	}


};

