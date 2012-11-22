/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <ewol/Debug.h>
#include <ewol/os/eSystem.h>
#include <ewol/audio/audio.h>
#include <ewol/os/gui.h>

// get a resources from the java environement : 
static JNIEnv*   JavaVirtualMachinePointer = NULL; // the JVM
static jclass    javaClassActivity = 0;            // main activity class (android ...)
static jobject   javaObjectActivity = 0;
static jmethodID javaClassActivityEntryPoint = 0;  // basic methode to call ...
static jmethodID javaClassActivityEntryPoint__CPP_keyboardShow = 0;  // basic methode to call ...
static jmethodID javaClassActivityEntryPoint__CPP_keyboardHide = 0;  // basic methode to call ...
static jmethodID javaClassActivityEntryPoint__CPP_OrientationChange = 0;
// generic classes
static jclass    javaDefaultClassString = 0;       // default string class

static int32_t m_currentHeight = 0;

static JavaVM* g_JavaVM = NULL;

// jni doc : /usr/lib/jvm/java-1.6.0-openjdk/include


void SendJava_KeyboardShow(bool showIt)
{
	EWOL_DEBUG("C->java : call java");
	if (NULL == g_JavaVM) {
		EWOL_DEBUG("C->java : JVM not initialised");
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
			EWOL_DEBUG("C->java : AttachCurrentThread failed : " << status);
			return;
		}
		if (JavaVirtualMachinePointer->ExceptionOccurred()) {
			EWOL_DEBUG("C->java : EXEPTION ...");
			JavaVirtualMachinePointer->ExceptionDescribe();
			JavaVirtualMachinePointer->ExceptionClear();
		}
	}
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		EWOL_DEBUG("C->java : EXEPTION ...");
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}

	if (NULL == JavaVirtualMachinePointer) {
		EWOL_DEBUG("C->java : JVM not initialised");
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
		EWOL_DEBUG("C->java : EXEPTION ...");
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}
	if (status == JNI_EDETACHED) {
		// Finished with the JVM.
		g_JavaVM->DetachCurrentThread();
	}
}

// mode 0 : auto; 1 landscape, 2 portrait
void SendJava_OrientationChange(int32_t mode)
{
	EWOL_DEBUG("C->java : call java");
	if (NULL == g_JavaVM) {
		EWOL_DEBUG("C->java : JVM not initialised");
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
			EWOL_DEBUG("C->java : AttachCurrentThread failed : " << status);
			return;
		}
		if (JavaVirtualMachinePointer->ExceptionOccurred()) {
			EWOL_DEBUG("C->java : EXEPTION ...");
			JavaVirtualMachinePointer->ExceptionDescribe();
			JavaVirtualMachinePointer->ExceptionClear();
		}
	}
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		EWOL_DEBUG("C->java : EXEPTION ...");
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}

	if (NULL == JavaVirtualMachinePointer) {
		EWOL_DEBUG("C->java : JVM not initialised");
		return;
	}

	jint param = mode;
	
	//Call java ...
	JavaVirtualMachinePointer->CallVoidMethod(javaObjectActivity, javaClassActivityEntryPoint__CPP_OrientationChange, param);

	// manage execption : 
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		EWOL_DEBUG("C->java : EXEPTION ...");
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}
	
	if (status == JNI_EDETACHED) {
		// Finished with the JVM.
		g_JavaVM->DetachCurrentThread();
	}
}


void SendSystemMessage(const char * dataString)
{
	EWOL_DEBUG("C->java : send message to the java : \"" << dataString << "\"");
	if (NULL == g_JavaVM) {
		EWOL_DEBUG("C->java : JVM not initialised");
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
			EWOL_ERROR("C->java : AttachCurrentThread failed : " << status);
			return;
		}
	}
	EWOL_DEBUG("C->java : 111");
	if (NULL == JavaVirtualMachinePointer) {
		EWOL_ERROR("C->java : JVM not initialised");
		return;
	}
	EWOL_DEBUG("C->java : 222");
	if (NULL == dataString) {
		EWOL_ERROR("C->java : No data to send ...");
		return;
	}
	EWOL_DEBUG("C->java : 333");
	// create the string to the java
	jstring jstr = JavaVirtualMachinePointer->NewStringUTF(dataString);
	if (jstr == 0) {
		EWOL_ERROR("C->java : Out of memory" );
		return;
	}
	EWOL_DEBUG("C->java : 444");
	// create argument list
	jobjectArray args = JavaVirtualMachinePointer->NewObjectArray(1, javaDefaultClassString, jstr);
	if (args == 0) {
		EWOL_ERROR("C->java : Out of memory" );
		return;
	}
	EWOL_DEBUG("C->java : 555");
	//Call java ...
	JavaVirtualMachinePointer->CallStaticVoidMethod(javaClassActivity, javaClassActivityEntryPoint, args);
	
	EWOL_DEBUG("C->java : 666");
	// manage execption : 
	if (JavaVirtualMachinePointer->ExceptionOccurred()) {
		JavaVirtualMachinePointer->ExceptionDescribe();
		JavaVirtualMachinePointer->ExceptionClear();
	}
	if (status == JNI_EDETACHED) {
		// Finished with the JVM.
		g_JavaVM->DetachCurrentThread();
	}
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
		EWOL_DEBUG("JNI-> load the jvm ..." );
		return JNI_VERSION_1_6;
	}
	// JNI OnUnLoad
	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
	{
		g_JavaVM = NULL;
		EWOL_DEBUG("JNI-> Un-load the jvm ..." );
	}

	
	/* Call to initialize the graphics state */
	void Java___PROJECT_ORG_TYPE_____PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivityParamSetArchiveDir( JNIEnv* env, jobject  thiz, jint mode, jstring myString)
	{
		// direct setting of the date in the string system ...
		jboolean isCopy;
		const char* str = env->GetStringUTFChars(myString, &isCopy);
		eSystem::SetArchiveDir(mode, str);
		if (isCopy == JNI_TRUE) {
			// from here str is reset ...
			env->ReleaseStringUTFChars(myString, str);
			str = NULL;
		}
	}
	
	void Java___PROJECT_ORG_TYPE_____PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivitySetJavaVortualMachineStart( JNIEnv*  env, jclass classBase, jobject obj)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Set JVM Pointer                      **");
		EWOL_DEBUG("*******************************************");
		JavaVirtualMachinePointer = env;
		// get default needed all time elements : 
		if (NULL != JavaVirtualMachinePointer) {
			EWOL_DEBUG("C->java : try load __PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__ class");
			javaClassActivity = JavaVirtualMachinePointer->FindClass("__PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__" );
			if (javaClassActivity == 0) {
				EWOL_ERROR("C->java : Can't find __PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__ class");
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			// get the activity object : 
			javaClassActivityEntryPoint = JavaVirtualMachinePointer->GetStaticMethodID(javaClassActivity, "eventFromCPP", "([Ljava/lang/String;)V" );
			if (javaClassActivityEntryPoint == 0) {
				EWOL_ERROR("C->java : Can't find __PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.eventFromCPP" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaClassActivityEntryPoint__CPP_keyboardShow = JavaVirtualMachinePointer->GetMethodID(javaClassActivity, "CPP_keyboardShow", "()V" );
			if (javaClassActivityEntryPoint__CPP_keyboardShow == 0) {
				EWOL_ERROR("C->java : Can't find __PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.CPP_keyboardShow" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaClassActivityEntryPoint__CPP_keyboardHide = JavaVirtualMachinePointer->GetMethodID(javaClassActivity, "CPP_keyboardHide", "()V" );
			if (javaClassActivityEntryPoint__CPP_keyboardHide == 0) {
				EWOL_ERROR("C->java : Can't find __PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.CPP_keyboardHide" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			javaClassActivityEntryPoint__CPP_OrientationChange = JavaVirtualMachinePointer->GetMethodID(javaClassActivity, "CPP_OrientationChange", "(I)V" );
			if (javaClassActivityEntryPoint__CPP_OrientationChange == 0) {
				EWOL_ERROR("C->java : Can't find __PROJECT_ORG_TYPE__/__PROJECT_VENDOR__/__PROJECT_PACKAGE__/__PROJECT_NAME__.CPP_OrientationChange" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
			
			//javaObjectActivity = JavaVirtualMachinePointer->NewGlobalRef(obj);
			javaObjectActivity = obj;
			
			javaDefaultClassString = JavaVirtualMachinePointer->FindClass("java/lang/String" );
			if (javaDefaultClassString == 0) {
				EWOL_ERROR("C->java : Can't find java/lang/String" );
				// remove access on the virtual machine : 
				JavaVirtualMachinePointer = NULL;
				return;
			}
		}
	}
	void Java___PROJECT_ORG_TYPE_____PROJECT_VENDOR_____PROJECT_PACKAGE_____PROJECT_NAME___ActivitySetJavaVortualMachineStop( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Remove JVM Pointer                   **");
		EWOL_DEBUG("*******************************************");
		JavaVirtualMachinePointer = NULL;
	}
	void Java_org_ewol_interfaceJNI_TouchEvent( JNIEnv*  env )
	{
		EWOL_DEBUG(" ==> Touch Event");
		if (env->ExceptionOccurred()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
	
	void Java_org_ewol_interfaceJNI_ActivityOnCreate( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Create                   **");
		EWOL_DEBUG("*******************************************");
		eSystem::Init();
	}
	void Java_org_ewol_interfaceJNI_ActivityOnStart( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Start                    **");
		EWOL_DEBUG("*******************************************");
		//SendSystemMessage(" testmessages ... ");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnReStart( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Re-Start                 **");
		EWOL_DEBUG("*******************************************");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnResume( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Resume                   **");
		EWOL_DEBUG("*******************************************");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnPause( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Pause                    **");
		EWOL_DEBUG("*******************************************");
		// All the openGl has been destroyed ...
		eSystem::OpenGlContextDestroy();
	}
	void Java_org_ewol_interfaceJNI_ActivityOnStop( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Stop                     **");
		EWOL_DEBUG("*******************************************");
	}
	void Java_org_ewol_interfaceJNI_ActivityOnDestroy( JNIEnv*  env )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("**  Activity On Destroy                  **");
		EWOL_DEBUG("*******************************************");
		eSystem::UnInit();
	}
	
	
	
	/* **********************************************************************************************
	 * ** IO section :
	 * ********************************************************************************************** */
	void Java_org_ewol_interfaceJNI_IOInputEventMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		eSystem::SetInputMotion(pointerID+1, x, m_currentHeight-y);
	}
	
	void Java_org_ewol_interfaceJNI_IOInputEventState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		eSystem::SetInputState(pointerID+1, isUp, x, m_currentHeight-y);
	}
	
	void Java_org_ewol_interfaceJNI_IOMouseEventMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
	{
		eSystem::SetMouseMotion(pointerID+1, x, m_currentHeight-y);
	}
	
	void Java_org_ewol_interfaceJNI_IOMouseEventState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
	{
		eSystem::SetMouseState(pointerID+1, isUp, x, m_currentHeight-y);
	}
	
	void Java_org_ewol_interfaceJNI_IOUnknowEvent( JNIEnv* env, jobject  thiz, jint pointerID)
	{
		EWOL_DEBUG("Unknown IO event : " << pointerID << " ???");
	}
	
	void Java_org_ewol_interfaceJNI_IOKeyboardEventMove( JNIEnv* env, jobject  thiz, jint type, jboolean isdown)
	{
		EWOL_DEBUG("IO keyboard Move event : \"" << type << "\" is down=" << isdown);
	}
	
	void Java_org_ewol_interfaceJNI_IOKeyboardEventKey( JNIEnv* env, jobject  thiz, jint uniChar, jboolean isdown)
	{
		EWOL_DEBUG("IO keyboard Key event : \"" << uniChar << "\" is down=" << isdown);
		eSystem::keyboardKey_ts keyInput;
		keyInput.myChar = uniChar;
		keyInput.isDown = isdown;
		eSystem::SetKeyboard(keyInput);
	}
	
	enum {
		SYSTEM_KEY__VOLUME_UP = 1,
		SYSTEM_KEY__VOLUME_DOWN,
		SYSTEM_KEY__MENU,
		SYSTEM_KEY__CAMERA,
		SYSTEM_KEY__HOME,
		SYSTEM_KEY__POWER,
	};
	// TODO : Set a return true or false if we want to grep this event ...
	void Java_org_ewol_interfaceJNI_IOKeyboardEventKeySystem( JNIEnv* env, jobject  thiz, jint keyVal, jboolean isdown)
	{
		switch (keyVal)
		{
			case SYSTEM_KEY__VOLUME_UP:
				EWOL_DEBUG("IO keyboard Key System \"VOLUME_UP\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__VOLUME_DOWN:
				EWOL_DEBUG("IO keyboard Key System \"VOLUME_DOWN\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__MENU:
				EWOL_DEBUG("IO keyboard Key System \"MENU\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__CAMERA:
				EWOL_DEBUG("IO keyboard Key System \"CAMERA\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__HOME:
				EWOL_DEBUG("IO keyboard Key System \"HOME\" is down=" << keyVal);
				break;
			case SYSTEM_KEY__POWER:
				EWOL_DEBUG("IO keyboard Key System \"POWER\" is down=" << keyVal);
				break;
			default:
				EWOL_DEBUG("IO keyboard Key System event : \"" << keyVal << "\" is down=" << isdown);
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
		m_currentHeight = h;
		eSystem::Resize(w, h);
	}
	
	void Java_org_ewol_interfaceJNI_RenderDraw( JNIEnv*  env )
	{
		eSystem::Draw(true);
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


#undef __class__
#define __class__ "guiInterface"



int64_t guiInterface::GetTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    //EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
    return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_usec);
}


void guiInterface::SetTitle(etk::UString& title)
{
	// can not set the title in Android ...
}


// -------------------------------------------------------------------------
//         ClipBoard AREA :
// -------------------------------------------------------------------------

bool l_clipBoardOwnerStd = false;
void guiInterface::ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	// this is to force the local system to think we have the buffer
	// TODO : Remove this 2 Line when code will be writen
	l_clipBoardOwnerStd = true;
	switch (clipboardID)
	{
		case ewol::clipBoard::CLIPBOARD_SELECTION:
			// NOTE : Windows does not support the middle button the we do it internaly
			// just transmit an event , we have the data in the system
			eSystem::ClipBoardArrive(clipboardID);
			break;
		case ewol::clipBoard::CLIPBOARD_STD:
			if (false == l_clipBoardOwnerStd) {
				// Generate a request TO the OS
				// TODO : Send the message to the OS "We disire to receive the copy buffer ...
			} else {
				// just transmit an event , we have the data in the system
				eSystem::ClipBoardArrive(clipboardID);
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}

void guiInterface::ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	switch (clipboardID)
	{
		case ewol::clipBoard::CLIPBOARD_SELECTION:
			// NOTE : nothing to do : Windows deas ot supported Middle button
			break;
		case ewol::clipBoard::CLIPBOARD_STD:
			// Request the clipBoard :
			if (false == l_clipBoardOwnerStd) {
				// TODO : Inform the OS that we have the current buffer of copy ...
				l_clipBoardOwnerStd = true;
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}


void guiInterface::Stop(void)
{
	// TODO : send a message to the android system to stop ...
}

// java system to send message : 
void SendSystemMessage(const char * dataString);
void SendJava_KeyboardShow(bool showIt);

void guiInterface::KeyboardShow(void)
{
	// send a message at the java :
	SendJava_KeyboardShow(true);
}

void guiInterface::KeyboardHide(void)
{
	// send a message at the java :
	SendJava_KeyboardShow(false);
}

void guiInterface::ChangeSize(etk::Vector2D<int32_t> size)
{
	// The size can not be change on android platform
}

void guiInterface::ChangePos(etk::Vector2D<int32_t> size)
{
	// The position can not be change on Android platform
}

void guiInterface::GetAbsPos(etk::Vector2D<int32_t>& size)
{
	size.x = 0;
	size.y = 0;
}

void guiInterface::ForceOrientation(ewol::orientation_te orientation)
{
	SendJava_OrientationChange((int32_t)orientation);
}
