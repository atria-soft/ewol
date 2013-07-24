/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
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
#include <ewol/debug.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/renderer/audio/audio.h>
#include <ewol/renderer/os/gui.h>
#include <ewol/Dimension.h>
/* include auto generated file */
#include <ewol/renderer/os/org_ewol_EwolConstants.h>

// get a resources from the java environement : 
static JNIEnv*   JavaVirtualMachinePointer = NULL; // the JVM
static jclass    javaClassEwol = 0;            // main activity class (android ...)
static jclass    javaClassEwolCallbackAndActivity = 0;
static jobject   javaObjectEwolCallbackAndActivity = 0;

static jmethodID javaMethodEwolCallbackEventNotifier = 0;  // basic methode to call ...
static jmethodID javaMethodEwolCallbackKeyboardUpdate = 0;  // basic methode to call ...
static jmethodID javaMethodEwolCallbackOrientationUpdate = 0;
static jmethodID javaMethodEwolActivitySetTitle = 0;
// generic classes
static jclass    javaDefaultClassString = 0;       // default string class

static int32_t m_currentHeight = 0;

static JavaVM* g_JavaVM = NULL;

// special key of the android system :
static ewol::SpecialKey guiKeyBoardSpecialKeyMode;


// jni doc : /usr/lib/jvm/java-1.6.0-openjdk/include


#define __jni_safe_init_method_id(mid, cls, name, sign) ({		\
    mid = JavaVirtualMachinePointer->GetMethodID(cls, name, sign);	\
    if(mid == NULL) {							\
      EWOL_ERROR("C->java : Can't find the method " << name);		\
      /* remove access on the virtual machine : */			\
      JavaVirtualMachinePointer = NULL;					\
      return;								\
    }									\
  })

static void java_check_exception(JNIEnv* env) {
  if (env->ExceptionOccurred()) {
    EWOL_ERROR("C->java : EXEPTION ...");
    env->ExceptionDescribe();
    env->ExceptionClear();
  }
}

static bool java_attach_current_thread(int *rstatus) {
  EWOL_DEBUG("C->java : call java");
  if (NULL == g_JavaVM) {
    EWOL_ERROR("C->java : JVM not initialised");
    return false;
  }
  JNIEnv *JavaVirtualMachinePointer_tmp;
  *rstatus = g_JavaVM->GetEnv((void **) &JavaVirtualMachinePointer_tmp, JNI_VERSION_1_6);
  if (*rstatus == JNI_EDETACHED) {
    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "EwolNativeThread";
    lJavaVMAttachArgs.group = NULL; 
    int status = g_JavaVM->AttachCurrentThread(&JavaVirtualMachinePointer_tmp, &lJavaVMAttachArgs);
    java_check_exception(JavaVirtualMachinePointer);
    if (status != JNI_OK) {
      EWOL_ERROR("C->java : AttachCurrentThread failed : " << status);
      return false;
    }
  }
  return true;
}
static void java_detach_current_thread(int status) {
  if(status == JNI_EDETACHED)  g_JavaVM->DetachCurrentThread();
}


void SendJavaKeyboardUpdate(bool showIt)
{
  int status;
  if(!java_attach_current_thread(&status)) return;

  //Call java ...
  JavaVirtualMachinePointer->CallVoidMethod(javaObjectEwolCallbackAndActivity, javaMethodEwolCallbackKeyboardUpdate, showIt ? JNI_TRUE : JNI_FALSE);
  // manage execption : 
  java_check_exception(JavaVirtualMachinePointer);
  java_detach_current_thread(status);
}

// mode 0 : auto; 1 landscape, 2 portrait
void SendJavaOrientationUpdate(int32_t mode)
{
#ifndef __ANDROID_PERMISSION__SET_ORIENTATION__
  EWOL_ERROR("C->java : call set orientation without Allow application to do it ... Break...");
  return;
#else
  int status;
  if(!java_attach_current_thread(&status)) return;
  jint param = mode;
		
  //Call java ...
  JavaVirtualMachinePointer->CallVoidMethod(javaObjectEwolCallbackAndActivity, javaMethodEwolCallbackOrientationUpdate, param);
	
  // manage execption : 
  java_check_exception(JavaVirtualMachinePointer);
  java_detach_current_thread(status);
#endif
}


void SendJavaSetTitle(const char * dataString)
{
  EWOL_DEBUG("C->java : send message to the java : \"" << dataString << "\"");
  int status;
  if(!java_attach_current_thread(&status)) return;
  EWOL_DEBUG("C->java : 222");
  if (NULL == dataString) {
    EWOL_ERROR("C->java : No data to send ...");
    return;
  }

  //Call java ...
  jstring title = JavaVirtualMachinePointer->NewStringUTF(dataString);
  JavaVirtualMachinePointer->CallVoidMethod(javaObjectEwolCallbackAndActivity, javaMethodEwolActivitySetTitle, title);
  JavaVirtualMachinePointer->DeleteLocalRef(title);
  // manage execption : 
  java_check_exception(JavaVirtualMachinePointer);
  java_detach_current_thread(status);
}



void SendSystemMessage(const char * dataString)
{
  EWOL_DEBUG("C->java : send message to the java : \"" << dataString << "\"");
  int status;
  if(!java_attach_current_thread(&status)) return;
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
  JavaVirtualMachinePointer->CallVoidMethod(javaObjectEwolCallbackAndActivity, javaMethodEwolCallbackEventNotifier, args);
	
  EWOL_DEBUG("C->java : 666");
  java_check_exception(JavaVirtualMachinePointer);
  java_detach_current_thread(status);
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
  void Java_org_ewol_Ewol_paramSetArchiveDir(JNIEnv* env, jclass  cls, jint mode, jstring myString)
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
	
  void Java_org_ewol_Ewol_setJavaVirtualMachineStart(JNIEnv*  env, jclass classBase, jobject objCallback)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Set JVM Pointer                      **");
    EWOL_DEBUG("*******************************************");
    JavaVirtualMachinePointer = env;
    // get default needed all time elements : 
    if (NULL != JavaVirtualMachinePointer) {
      EWOL_DEBUG("C->java : try load org/ewol/Ewol class");
      javaClassEwol = JavaVirtualMachinePointer->FindClass("org/ewol/Ewol" );
      if (javaClassEwol == 0) {
	EWOL_ERROR("C->java : Can't find org/ewol/Ewol class");
	// remove access on the virtual machine : 
	JavaVirtualMachinePointer = NULL;
	return;
      }
      /* The object field extends Activity and implement EwolCallback */
      javaClassEwolCallbackAndActivity = JavaVirtualMachinePointer->GetObjectClass(objCallback);
      if(javaClassEwolCallbackAndActivity == NULL) {
	EWOL_ERROR("C->java : Can't find org/ewol/EwolCallback class");
	// remove access on the virtual machine : 
	JavaVirtualMachinePointer = NULL;
	return;
      }

      __jni_safe_init_method_id(
	javaMethodEwolActivitySetTitle, 
	javaClassEwolCallbackAndActivity, 
	"setTitle", "(Ljava/lang/CharSequence;)V");

      __jni_safe_init_method_id(
	javaMethodEwolCallbackEventNotifier, 
	javaClassEwolCallbackAndActivity, 
	"eventNotifier", "([Ljava/lang/String;)V");

      __jni_safe_init_method_id(
	javaMethodEwolCallbackKeyboardUpdate, 
	javaClassEwolCallbackAndActivity, 
	"keyboardUpdate", "(Z)V");

      __jni_safe_init_method_id(
	javaMethodEwolCallbackOrientationUpdate,
	javaClassEwolCallbackAndActivity, 
	"orientationUpdate", "(I)V");

			
      javaObjectEwolCallbackAndActivity = env->NewGlobalRef(objCallback);
      //javaObjectEwolCallbackAndActivity = objCallback;
			
      javaDefaultClassString = JavaVirtualMachinePointer->FindClass("java/lang/String" );
      if (javaDefaultClassString == 0) {
	EWOL_ERROR("C->java : Can't find java/lang/String" );
	// remove access on the virtual machine : 
	JavaVirtualMachinePointer = NULL;
	return;
      }
    }
  }
  void Java_org_ewol_Ewol_setJavaVirtualMachineStop(JNIEnv*  env, jclass cls) {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Remove JVM Pointer                   **");
    EWOL_DEBUG("*******************************************");
    env->DeleteGlobalRef(javaObjectEwolCallbackAndActivity);
    JavaVirtualMachinePointer = NULL;
  }
  void Java_org_ewol_Ewol_touchEvent( JNIEnv*  env, jobject thiz )
  {
    EWOL_DEBUG(" ==> Touch Event");
    java_check_exception(env);
  }
	
  void Java_org_ewol_Ewol_onCreate( JNIEnv*  env, jobject thiz )
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Create                   **");
    EWOL_DEBUG("*******************************************");
    eSystem::Init();
  }

  void Java_org_ewol_Ewol_onStart(JNIEnv* env, jobject thiz)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Start                    **");
    EWOL_DEBUG("*******************************************");
    //SendSystemMessage(" testmessages ... ");
  }
  void Java_org_ewol_Ewol_onReStart(JNIEnv* env, jobject thiz)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Re-Start                 **");
    EWOL_DEBUG("*******************************************");
  }
  void Java_org_ewol_Ewol_onResume(JNIEnv* env, jobject thiz)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Resume                   **");
    EWOL_DEBUG("*******************************************");
  }
  void Java_org_ewol_Ewol_onPause(JNIEnv* env, jobject thiz)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Pause                    **");
    EWOL_DEBUG("*******************************************");
    // All the openGl has been destroyed ...
    eSystem::OpenGlContextDestroy();
  }
  void Java_org_ewol_Ewol_onStop(JNIEnv* env, jobject thiz)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Stop                     **");
    EWOL_DEBUG("*******************************************");
  }
  void Java_org_ewol_Ewol_onDestroy(JNIEnv* env, jobject thiz)
  {
    EWOL_DEBUG("*******************************************");
    EWOL_DEBUG("**  Activity On Destroy                  **");
    EWOL_DEBUG("*******************************************");
    eSystem::UnInit();
  }
	
	
	
  /* **********************************************************************************************
   * ** IO section :
   * ********************************************************************************************** */
  void Java_org_ewol_Ewol_inputEventMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
  {
    eSystem::SetInputMotion(pointerID+1, x, m_currentHeight-y);
  }
	
  void Java_org_ewol_Ewol_inputEventState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
  {
    eSystem::SetInputState(pointerID+1, isUp, x, m_currentHeight-y);
  }
	
  void Java_org_ewol_Ewol_mouseEventMotion( JNIEnv* env, jobject  thiz, jint pointerID, jfloat x, jfloat y )
  {
    eSystem::SetMouseMotion(pointerID+1, x, m_currentHeight-y);
  }
	
  void Java_org_ewol_Ewol_mouseEventState( JNIEnv* env, jobject  thiz, jint pointerID, jboolean isUp, jfloat x, jfloat y )
  {
    eSystem::SetMouseState(pointerID+1, isUp, x, m_currentHeight-y);
  }
	
  void Java_org_ewol_Ewol_unknowEvent( JNIEnv* env, jobject  thiz, jint pointerID)
  {
    EWOL_DEBUG("Unknown IO event : " << pointerID << " ???");
  }
	
  void Java_org_ewol_Ewol_keyboardEventMove( JNIEnv* env, jobject  thiz, jint type, jboolean isdown)
  {
    EWOL_DEBUG("IO keyboard Move event : \"" << type << "\" is down=" << isdown);
  }
	
  void Java_org_ewol_Ewol_keyboardEventKey( JNIEnv* env, jobject  thiz, jint uniChar, jboolean isdown)
  {
    EWOL_DEBUG("IO keyboard Key event : \"" << uniChar << "\" is down=" << isdown);
    eSystem::SetKeyboard(guiKeyBoardSpecialKeyMode, uniChar, isdown);
  }
	
  void Java_org_ewol_Ewol_displayPropertyMetrics( JNIEnv* env, jobject  thiz, jfloat ratioX, jfloat ratioY)
  {
    // set the internal system ratio properties ...
    ewol::dimension::SetPixelRatio(vec2(ratioX,ratioY), ewol::Dimension::Inch);
  }
	
  // TODO : Set a return true or false if we want to grep this event ...
  void Java_org_ewol_Ewol_keyboardEventKeySystem( JNIEnv* env, jobject  thiz, jint keyVal, jboolean isdown)
  {
    switch (keyVal)
    {
      case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_VOLUME_UP:
	EWOL_DEBUG("IO keyboard Key System \"VOLUME_UP\" is down=" << isdown);
	break;
      case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_VOLUME_DOWN:
	EWOL_DEBUG("IO keyboard Key System \"VOLUME_DOWN\" is down=" << isdown);
	break;
      case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_MENU:
	EWOL_DEBUG("IO keyboard Key System \"MENU\" is down=" << isdown);
	break;
      case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_CAMERA:
	EWOL_DEBUG("IO keyboard Key System \"CAMERA\" is down=" << isdown);
	break;
      case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_HOME:
	EWOL_DEBUG("IO keyboard Key System \"HOME\" is down=" << isdown);
	break;
      case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_POWER:
	EWOL_DEBUG("IO keyboard Key System \"POWER\" is down=" << isdown);
	break;
      default:
	EWOL_DEBUG("IO keyboard Key System event : \"" << keyVal << "\" is down=" << isdown);
	break;
    }
  }
	
	
  /* **********************************************************************************************
   * **  Renderer section :
   * ********************************************************************************************** */
  void Java_org_ewol_Ewol_renderInit(JNIEnv* env, jobject thiz)
  {
		
  }
	
  void Java_org_ewol_Ewol_renderResize( JNIEnv* env, jobject thiz, jint w, jint h )
  {
    m_currentHeight = h;
    eSystem::Resize(w, h);
  }
	
  void Java_org_ewol_Ewol_renderDraw(JNIEnv* env, jobject thiz)
  {
    eSystem::Draw(true);
  }

  void Java_org_ewol_Ewol_audioPlayback(JNIEnv* env, void* reserved, jshortArray location, jint frameRate, jint nbChannels)
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


int guiInterface::main(int argc, const char *argv[])
{
  // unneeded fuction, all is controlled by android java ...
  return 0;
}

int64_t guiInterface::GetTime(void)
{
  struct timeval  now;
  gettimeofday(&now, NULL);
  //EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
  return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_usec);
}


void guiInterface::SetTitle(etk::UString& title)
{
  SendJavaSetTitle(title.c_str());
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
    case ewol::clipBoard::clipboardSelection:
      // NOTE : Windows does not support the middle button the we do it internaly
      // just transmit an event , we have the data in the system
      eSystem::ClipBoardArrive(clipboardID);
      break;
    case ewol::clipBoard::clipboardStd:
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
    case ewol::clipBoard::clipboardSelection:
      // NOTE : nothing to do : Windows deas ot supported Middle button
      break;
    case ewol::clipBoard::clipboardStd:
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
void SendJavaKeyboardUpdate(bool showIt);

void guiInterface::KeyboardShow(void)
{
  // send a message at the java :
  SendJavaKeyboardUpdate(true);
}

void guiInterface::KeyboardHide(void)
{
  // send a message at the java :
  SendJavaKeyboardUpdate(false);
}

void guiInterface::ChangeSize(ivec2 size)
{
  // The size can not be change on android platform
}

void guiInterface::ChangePos(ivec2 size)
{
  // The position can not be change on Android platform
}

void guiInterface::GetAbsPos(ivec2& size)
{
  size.setValue(0,0);
}

void guiInterface::ForceOrientation(ewol::orientation_te orientation)
{
  SendJavaOrientationUpdate((int32_t)orientation);
}

void guiInterface::GrabPointerEvents(bool isGrabbed, vec2 forcedPosition)
{
  // nothing to do ...
}

void guiInterface::SetCursor(ewol::cursorDisplay_te newCursor)
{
  // nothing to do ...
}

void guiInterface::SetIcon(etk::UString inputFile)
{
  // nothing to do ...
}


