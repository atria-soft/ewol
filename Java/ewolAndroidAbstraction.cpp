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
static jobject   javaObjectActivity = 0;
static jmethodID javaClassActivityEntryPoint = 0;  // basic methode to call ...
static jmethodID javaClassActivityEntryPoint__CPP_keyboardShow = 0;  // basic methode to call ...
// generic classes
static jclass    javaDefaultClassString = 0;       // default string class


static JavaVM* g_JavaVM = NULL;

// jni doc : /usr/lib/jvm/java-1.6.0-openjdk/include


// for exemple test :
void displayKeyboard(bool pShow) {
    // Attaches the current thread to the JVM.
    jint lResult;
    jint lFlags = 0;

    JavaVM* lJavaVM = g_JavaVM;
    JNIEnv* lJNIEnv = NULL;

	int status = g_JavaVM->GetEnv((void **) &lJNIEnv, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		JavaVMAttachArgs lJavaVMAttachArgs;
		lJavaVMAttachArgs.version = JNI_VERSION_1_6;
		lJavaVMAttachArgs.name = "EwolNativeThread";
		lJavaVMAttachArgs.group = NULL; 
		status = g_JavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
		if (status != JNI_OK) {
			APPL_DEBUG("C->java : AttachCurrentThread failed : " << status);
			return;
		}
		if (JavaVirtualMachinePointer->ExceptionOccurred()) {
			JavaVirtualMachinePointer->ExceptionDescribe();
			JavaVirtualMachinePointer->ExceptionClear();
		}
	}
	/*
	if (lResult == JNI_ERR) {
		return;
	}
	*/
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}

    // Retrieves NativeActivity.
    jobject lNativeActivity = javaClassActivity;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE = lJNIEnv->GetStaticFieldID(ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE = lJNIEnv->GetStaticObjectField(ClassContext, FieldINPUT_METHOD_SERVICE);
    // TODO : jniCheck(INPUT_METHOD_SERVICE);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = lJNIEnv->FindClass("android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(ClassNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetSystemService, INPUT_METHOD_SERVICE);

    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(ClassNativeActivity, "getWindow", "()Landroid/view/Window;");
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetWindow);
    jclass ClassWindow = lJNIEnv->FindClass("android/view/Window");
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow, MethodGetDecorView);

    if (pShow) {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
        jboolean lResult = lJNIEnv->CallBooleanMethod(lInputMethodManager, MethodShowSoftInput, lDecorView, lFlags);
    } else {
        // Runs lWindow.getViewToken()
        jclass ClassView = lJNIEnv->FindClass("android/view/View");
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView, MethodGetWindowToken);
        // lInputMethodManager.hideSoftInput(...).
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
        jboolean lRes = lJNIEnv->CallBooleanMethod( lInputMethodManager, MethodHideSoftInput, lBinder, lFlags);
    }

    // Finished with the JVM.
    lJavaVM->DetachCurrentThread();

}


void SendJava_KeyboardShow(void)
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

	APPL_DEBUG("C->java : 111");
	if (NULL == JavaVirtualMachinePointer) {
		APPL_DEBUG("C->java : JVM not initialised");
		return;
	}

	APPL_DEBUG("C->java : 333");
	//Call java ...
	//JavaVirtualMachinePointer->CallVoidMethod(javaClassActivity, javaClassActivityEntryPoint__CPP_keyboardShow);
	//JavaVirtualMachinePointer->CallStaticVoidMethod(javaClassActivity, javaClassActivityEntryPoint__CPP_keyboardShow);
	JavaVirtualMachinePointer->CallVoidMethod(javaObjectActivity, javaClassActivityEntryPoint__CPP_keyboardShow);
	

	APPL_DEBUG("C->java : 444");
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
		EWOL_ThreadSetArchiveDir(mode, str);
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
			if (javaClassActivityEntryPoint == 0) {
				APPL_DEBUG("C->java : Can't find com/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.CPP_keyboardShow" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaObjectActivity = JavaVirtualMachinePointer->NewGlobalRef(obj);
			
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
		if (env->ExceptionOccurred()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
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

