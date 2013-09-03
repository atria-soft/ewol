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
#include <ewol/renderer/eContext.h>
#include <ewol/renderer/audio/audio.h>
#include <ewol/Dimension.h>
/* include auto generated file */
#include <ewol/renderer/Android/org_ewol_EwolConstants.h>



int64_t ewol::GetTime(void)
{
	struct timeval	now;
	gettimeofday(&now, NULL);
	//EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
	return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_usec);
}


// jni doc : /usr/lib/jvm/java-1.6.0-openjdk/include

static JavaVM* g_JavaVM=NULL; // global acces on the unique JVM !!!

void java_check_exception(JNIEnv* _env) {
	if (_env->ExceptionOccurred()) {
		EWOL_ERROR("C->java : EXEPTION ...");
		_env->ExceptionDescribe();
		_env->ExceptionClear();
	}
}

class AndroidContext : public ewol::eContext
{
	public:
		typedef enum {
			appl_unknow,
			appl_application,
			appl_wallpaper
		} application_te;
	private:
		application_te m_javaApplicationType;
		// get a resources from the java environement : 
		JNIEnv* m_JavaVirtualMachinePointer; //!< the JVM
		jclass m_javaClassEwol; //!< main activity class (android ...)
		jclass m_javaClassEwolCallbackAndActivity;
		jobject m_javaObjectEwolCallbackAndActivity;
		jmethodID m_javaMethodEwolCallbackEventNotifier; //!< basic methode to call ...
		jmethodID m_javaMethodEwolCallbackKeyboardUpdate; //!< basic methode to call ...
		jmethodID m_javaMethodEwolCallbackOrientationUpdate;
		jmethodID m_javaMethodEwolActivitySetTitle;
		jclass m_javaDefaultClassString; //!< default string class
		int32_t m_currentHeight;
		ewol::SpecialKey m_guiKeyBoardSpecialKeyMode;//!< special key of the android system :
		bool m_clipBoardOwnerStd;
	private:
		bool SafeInitMethodID(jmethodID& _mid, jclass& _cls, char* _name, char* _sign)
		{
			_mid = m_JavaVirtualMachinePointer->GetMethodID(_cls, _name, _sign);
			if(_mid == NULL) {
				EWOL_ERROR("C->java : Can't find the method " << _name);
				/* remove access on the virtual machine : */
				m_JavaVirtualMachinePointer = NULL;
				return false;
			}
			return true;
		}
	public:
		AndroidContext(JNIEnv* _env, jclass _classBase, jobject _objCallback, application_te _typeAPPL) :
			m_javaApplicationType(_typeAPPL),
			m_JavaVirtualMachinePointer(NULL),
			m_javaClassEwol(0),
			m_javaClassEwolCallbackAndActivity(0),
			m_javaObjectEwolCallbackAndActivity(0),
			m_javaMethodEwolCallbackEventNotifier(0),
			m_javaMethodEwolCallbackKeyboardUpdate(0),
			m_javaMethodEwolCallbackOrientationUpdate(0),
			m_javaMethodEwolActivitySetTitle(0),
			m_javaDefaultClassString(0),
			m_currentHeight(0),
			m_clipBoardOwnerStd(false)
		{
			EWOL_DEBUG("*******************************************");
			if (m_javaApplicationType == appl_application) {
				EWOL_DEBUG("** Set JVM Pointer (application)         **");
			} else {
				EWOL_DEBUG("** Set JVM Pointer (LiveWallpaper)       **");
			}
			EWOL_DEBUG("*******************************************");
			m_JavaVirtualMachinePointer = _env;
			// get default needed all time elements : 
			if (NULL != m_JavaVirtualMachinePointer) {
				EWOL_DEBUG("C->java : try load org/ewol/Ewol class");
				m_javaClassEwol = m_JavaVirtualMachinePointer->FindClass("org/ewol/Ewol" );
				if (m_javaClassEwol == 0) {
					EWOL_ERROR("C->java : Can't find org/ewol/Ewol class");
					// remove access on the virtual machine : 
					m_JavaVirtualMachinePointer = NULL;
					return;
				}
				/* The object field extends Activity and implement EwolCallback */
				m_javaClassEwolCallbackAndActivity = m_JavaVirtualMachinePointer->GetObjectClass(_objCallback);
				if(m_javaClassEwolCallbackAndActivity == NULL) {
					EWOL_ERROR("C->java : Can't find org/ewol/EwolCallback class");
					// remove access on the virtual machine : 
					m_JavaVirtualMachinePointer = NULL;
					return;
				}
				bool ret= false;
				if (m_javaApplicationType == appl_application) {
					ret=SafeInitMethodID(m_javaMethodEwolActivitySetTitle,
					                     m_javaClassEwolCallbackAndActivity,
					                     "setTitle",
					                     "(Ljava/lang/CharSequence;)V");
					if (ret==false) {
						return;
					}
				}
				
				ret=SafeInitMethodID(m_javaMethodEwolCallbackEventNotifier,
				                     m_javaClassEwolCallbackAndActivity,
				                     "eventNotifier",
				                     "([Ljava/lang/String;)V");
				if (ret==false) {
					return;
				}
				
				ret=SafeInitMethodID(m_javaMethodEwolCallbackKeyboardUpdate,
				                     m_javaClassEwolCallbackAndActivity,
				                     "keyboardUpdate",
				                     "(Z)V");
				if (ret==false) {
					return;
				}
				
				ret=SafeInitMethodID(m_javaMethodEwolCallbackOrientationUpdate,
				                     m_javaClassEwolCallbackAndActivity,
				                     "orientationUpdate",
				                     "(I)V");
				if (ret==false) {
					return;
				}
				
				m_javaObjectEwolCallbackAndActivity = _env->NewGlobalRef(_objCallback);
				//javaObjectEwolCallbackAndActivity = objCallback;
				
				m_javaDefaultClassString = m_JavaVirtualMachinePointer->FindClass("java/lang/String" );
				if (m_javaDefaultClassString == 0) {
					EWOL_ERROR("C->java : Can't find java/lang/String" );
					// remove access on the virtual machine : 
					m_JavaVirtualMachinePointer = NULL;
					return;
				}
			}
		}
		
		~AndroidContext(void)
		{
			
		}
		
		void UnInit(JNIEnv* _env)
		{
			_env->DeleteGlobalRef(m_javaObjectEwolCallbackAndActivity);
			m_javaObjectEwolCallbackAndActivity = NULL;
		}
		
		
		int32_t Run(void)
		{
			// might never be called !!!
			return -1;
		}
		
		void Stop(void)
		{
			// TODO : send a message to the android system to stop ...
		}
		
		void ClipBoardGet(ewol::clipBoard::clipboardListe_te _clipboardID)
		{
			// this is to force the local system to think we have the buffer
			// TODO : Remove this 2 Line when code will be writen
			m_clipBoardOwnerStd = true;
			switch (_clipboardID)
			{
				case ewol::clipBoard::clipboardSelection:
					// NOTE : Windows does not support the middle button the we do it internaly
					// just transmit an event , we have the data in the system
					OS_ClipBoardArrive(_clipboardID);
					break;
				case ewol::clipBoard::clipboardStd:
					if (false == m_clipBoardOwnerStd) {
						// Generate a request TO the OS
						// TODO : Send the message to the OS "We disire to receive the copy buffer ...
					} else {
						// just transmit an event , we have the data in the system
						OS_ClipBoardArrive(_clipboardID);
					}
					break;
				default:
					EWOL_ERROR("Request an unknow ClipBoard ...");
					break;
			}
		}
		
		void ClipBoardSet(ewol::clipBoard::clipboardListe_te _clipboardID)
		{
			switch (_clipboardID)
			{
				case ewol::clipBoard::clipboardSelection:
					// NOTE : nothing to do : Windows deas ot supported Middle button
					break;
				case ewol::clipBoard::clipboardStd:
					// Request the clipBoard :
					if (false == m_clipBoardOwnerStd) {
						// TODO : Inform the OS that we have the current buffer of copy ...
						m_clipBoardOwnerStd = true;
					}
					break;
				default:
					EWOL_ERROR("Request an unknow ClipBoard ...");
					break;
			}
		}

	private:
		
		
		bool java_attach_current_thread(int *_rstatus) {
			EWOL_DEBUG("C->java : call java");
			if (NULL == g_JavaVM) {
				EWOL_ERROR("C->java : JVM not initialised");
				return false;
			}
			JNIEnv *JavaVirtualMachinePointer_tmp;
			*_rstatus = g_JavaVM->GetEnv((void **) &JavaVirtualMachinePointer_tmp, JNI_VERSION_1_6);
			if (*_rstatus == JNI_EDETACHED) {
				JavaVMAttachArgs lJavaVMAttachArgs;
				lJavaVMAttachArgs.version = JNI_VERSION_1_6;
				lJavaVMAttachArgs.name = "EwolNativeThread";
				lJavaVMAttachArgs.group = NULL; 
				int status = g_JavaVM->AttachCurrentThread(&JavaVirtualMachinePointer_tmp, &lJavaVMAttachArgs);
				java_check_exception(m_JavaVirtualMachinePointer);
				if (status != JNI_OK) {
					EWOL_ERROR("C->java : AttachCurrentThread failed : " << status);
					return false;
				}
			}
			return true;
		}
		
		void java_detach_current_thread(int _status) {
			if(_status == JNI_EDETACHED) {
				g_JavaVM->DetachCurrentThread();
			}
		}
		
		
		void SendJavaKeyboardUpdate(jboolean _showIt)
		{
			int status;
			if(!java_attach_current_thread(&status)) {
				return;
			}
			
			//Call java ...
			m_JavaVirtualMachinePointer->CallVoidMethod(m_javaObjectEwolCallbackAndActivity, m_javaMethodEwolCallbackKeyboardUpdate, _showIt);
			// manage execption : 
			java_check_exception(m_JavaVirtualMachinePointer);
			java_detach_current_thread(status);
		}
		void KeyboardShow(void) { SendJavaKeyboardUpdate(JNI_TRUE); };
		void KeyboardHide(void) { SendJavaKeyboardUpdate(JNI_FALSE); };
		
		// mode 0 : auto; 1 landscape, 2 portrait
		void ForceOrientation(ewol::orientation_te _orientation)
		{
		#ifndef __ANDROID_PERMISSION__SET_ORIENTATION__
			EWOL_ERROR("C->java : call set orientation without Allow application to do it ... Break...");
			return;
		#else
			int status;
			if(!java_attach_current_thread(&status)) {
				return;
			}
			jint param = (jint)_orientation;
			
			//Call java ...
			m_JavaVirtualMachinePointer->CallVoidMethod(m_javaObjectEwolCallbackAndActivity, m_javaMethodEwolCallbackOrientationUpdate, param);
			
			// manage execption : 
			java_check_exception(m_JavaVirtualMachinePointer);
			java_detach_current_thread(status);
		#endif
		}
		
		void SetTitle(etk::UString& _title)
		{
			EWOL_DEBUG("C->java : send message to the java : \"" << _title << "\"");
			if (m_javaApplicationType == appl_application) {
				int status;
				if(!java_attach_current_thread(&status)) {
					return;
				}
				etk::Char tmpChar = _title.c_str();
				//Call java ...
				jstring title = m_JavaVirtualMachinePointer->NewStringUTF(tmpChar);
				m_JavaVirtualMachinePointer->CallVoidMethod(m_javaObjectEwolCallbackAndActivity, m_javaMethodEwolActivitySetTitle, title);
				m_JavaVirtualMachinePointer->DeleteLocalRef(title);
				// manage execption : 
				java_check_exception(m_JavaVirtualMachinePointer);
				java_detach_current_thread(status);
			} else {
				EWOL_ERROR("C->java : can not set title on appliation that is not real application");
			}
		}
		
		
		
		void SendSystemMessage(const char* _dataString)
		{
			EWOL_DEBUG("C->java : send message to the java : \"" << _dataString << "\"");
			int status;
			if(!java_attach_current_thread(&status)) {
				return;
			}
			EWOL_DEBUG("C->java : 222");
			if (NULL == _dataString) {
				EWOL_ERROR("C->java : No data to send ...");
				return;
			}
			EWOL_DEBUG("C->java : 333");
			// create the string to the java
			jstring jstr = m_JavaVirtualMachinePointer->NewStringUTF(_dataString);
			if (jstr == 0) {
				EWOL_ERROR("C->java : Out of memory" );
				return;
			}
			EWOL_DEBUG("C->java : 444");
			// create argument list
			jobjectArray args = m_JavaVirtualMachinePointer->NewObjectArray(1, m_javaDefaultClassString, jstr);
			if (args == 0) {
				EWOL_ERROR("C->java : Out of memory" );
				return;
			}
			EWOL_DEBUG("C->java : 555");
			//Call java ...
			m_JavaVirtualMachinePointer->CallVoidMethod(m_javaObjectEwolCallbackAndActivity, m_javaMethodEwolCallbackEventNotifier, args);
			
			EWOL_DEBUG("C->java : 666");
			java_check_exception(m_JavaVirtualMachinePointer);
			java_detach_current_thread(status);
		}
	public:
		void OS_SetInputMotion(int _pointerID, const vec2& _pos)
		{
			ewol::eContext::OS_SetInputMotion(_pointerID, vec2(_pos.x(),m_currentHeight-_pos.y()) );
		}
		
		void OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos)
		{
			ewol::eContext::OS_SetInputState(_pointerID, _isDown, vec2(_pos.x(),m_currentHeight-_pos.y()) );
		}
		
		void OS_SetMouseMotion(int _pointerID, const vec2& _pos)
		{
			ewol::eContext::OS_SetMouseMotion(_pointerID, vec2(_pos.x(),m_currentHeight-_pos.y()) );
		}
		
		void OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos)
		{
			ewol::eContext::OS_SetMouseState(_pointerID, _isDown, vec2(_pos.x(),m_currentHeight-_pos.y()) );
		}
		void ANDROID_SetKeyboard(uniChar_t _myChar, bool _isDown, bool _isARepeateKey=false)
		{
			OS_SetKeyboard(m_guiKeyBoardSpecialKeyMode, _myChar, _isDown, _isARepeateKey);
		}
		void OS_Resize(const vec2& _size)
		{
			m_currentHeight = _size.y();
			ewol::eContext::OS_Resize(_size);
		}
};

static etk::Vector<AndroidContext*> s_listInstance;

extern "C"
{
	// JNI OnLoad
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* _jvm, void* _reserved)
	{
		// get the java virtual machine handle ...
		g_JavaVM = _jvm;
		EWOL_DEBUG("JNI-> load the jvm ..." );
		return JNI_VERSION_1_6;
	}
	// JNI OnUnLoad
	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* _vm, void *_reserved)
	{
		g_JavaVM = NULL;
		EWOL_DEBUG("JNI-> Un-load the jvm ..." );
	}
	
	/* Call to initialize the graphics state */
	void Java_org_ewol_Ewol_paramSetArchiveDir(JNIEnv* _env, jclass _cls, jint _mode, jstring _myString)
	{
		esize_t _id=0;
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		// direct setting of the date in the string system ...
		jboolean isCopy;
		const char* str = _env->GetStringUTFChars(_myString, &isCopy);
		s_listInstance[_id]->SetArchiveDir(_mode, str);
		if (isCopy == JNI_TRUE) {
			// from here str is reset ...
			_env->ReleaseStringUTFChars(_myString, str);
			str = NULL;
		}
	}
	
	// TODO : Return the local ID ...
	void Java_org_ewol_Ewol_setJavaVirtualMachineStart(JNIEnv* _env, jclass _classBase, jobject _objCallback)
	{
		AndroidContext* tmpContext = new AndroidContext(_env, _classBase, _objCallback, AndroidContext::appl_application);
		if (NULL == tmpContext) {
			EWOL_ERROR("Can not allocate the main context instance");
			// TODO : return -1;
		}
		// TODO : Change this ==> this generate the new ID ...
		if (s_listInstance.Size()>0) {
			// note : For now : memory leek and errors ...
			s_listInstance[0] = tmpContext;
		} else {
			// for future case : all time this ...
			s_listInstance.PushBack(tmpContext);
			// esize_t newID = s_listInstance.Size();
			// return newID;
		}
	}
	
	// TODO : Return the local ID ...
	void Java_org_ewol_Ewol_setJavaVirtualMachineStartWallpaperEngine(JNIEnv* _env, jclass _classBase, jobject _objCallback)
	{
		AndroidContext* tmpContext = new AndroidContext(_env, _classBase, _objCallback, AndroidContext::appl_wallpaper);
		if (NULL == tmpContext) {
			EWOL_ERROR("Can not allocate the main context instance");
			// TODO : return -1;
		}
		// TODO : Change this ==> this generate the new ID ...
		if (s_listInstance.Size()>0) {
			// note : For now : memory leek and errors ...
			s_listInstance[0] = tmpContext;
		} else {
			// for future case : all time this ...
			s_listInstance.PushBack(tmpContext);
			// esize_t newID = s_listInstance.Size();
			// return newID;
		}
	}
	
	
	void Java_org_ewol_Ewol_setJavaVirtualMachineStop(JNIEnv* _env, jclass _cls)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Remove JVM Pointer                    **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->UnInit(_env);
		delete(s_listInstance[_id]);
		s_listInstance[_id]=NULL;
	}
	void Java_org_ewol_Ewol_touchEvent(JNIEnv* _env, jobject _thiz )
	{
		EWOL_DEBUG(" ==> Touch Event");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		java_check_exception(_env);
	}
	
	void Java_org_ewol_Ewol_onCreate(JNIEnv* _env, jobject _thiz )
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Create                    **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		//s_listInstance[_id]->Init();
	}
	
	void Java_org_ewol_Ewol_onStart(JNIEnv* _env, jobject _thiz)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Start                     **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		//SendSystemMessage(" testmessages ... ");
	}
	void Java_org_ewol_Ewol_onReStart(JNIEnv* _env, jobject _thiz)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Re-Start                  **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
	}
	void Java_org_ewol_Ewol_onResume(JNIEnv* _env, jobject _thiz)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Resume                    **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
	}
	void Java_org_ewol_Ewol_onPause(JNIEnv* _env, jobject _thiz)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Pause                     **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		// All the openGl has been destroyed ...
		s_listInstance[_id]->GetResourcesManager().ContextHasBeenDestroyed();
	}
	void Java_org_ewol_Ewol_onStop(JNIEnv* _env, jobject _thiz)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Stop                      **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
	}
	void Java_org_ewol_Ewol_onDestroy(JNIEnv* _env, jobject _thiz)
	{
		EWOL_DEBUG("*******************************************");
		EWOL_DEBUG("** Activity On Destroy                   **");
		EWOL_DEBUG("*******************************************");
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		//s_listInstance[_id]->UnInit();
	}
	
	
	
	/* **********************************************************************************************
	 * ** IO section :
	 * ********************************************************************************************** */
	void Java_org_ewol_Ewol_inputEventMotion(JNIEnv* _env, jobject _thiz, jint _pointerID, jfloat _x, jfloat _y )
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->OS_SetInputMotion(_pointerID+1, vec2(_x,_y));
	}
	
	void Java_org_ewol_Ewol_inputEventState(JNIEnv* _env, jobject _thiz, jint _pointerID, jboolean _isUp, jfloat _x, jfloat _y)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->OS_SetInputState(_pointerID+1, _isUp, vec2(_x,_y));
	}
	
	void Java_org_ewol_Ewol_mouseEventMotion(JNIEnv* _env, jobject _thiz, jint _pointerID, jfloat _x, jfloat _y)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->OS_SetMouseMotion(_pointerID+1, vec2(_x,_y));
	}
	
	void Java_org_ewol_Ewol_mouseEventState(JNIEnv* _env, jobject _thiz, jint _pointerID, jboolean _isUp, jfloat _x, jfloat _y)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->OS_SetMouseState(_pointerID+1, _isUp, vec2(_x,_y));
	}
	
	void Java_org_ewol_Ewol_unknowEvent(JNIEnv* _env, jobject _thiz, jint _pointerID)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		EWOL_DEBUG("Unknown IO event : " << _pointerID << " ???");
	}
	
	void Java_org_ewol_Ewol_keyboardEventMove(JNIEnv* _env, jobject _thiz, jint _type, jboolean _isdown)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		EWOL_DEBUG("IO keyboard Move event : \"" << _type << "\" is down=" << _isdown);
	}
	
	void Java_org_ewol_Ewol_keyboardEventKey(JNIEnv* _env, jobject _thiz, jint _uniChar, jboolean _isdown)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		EWOL_DEBUG("IO keyboard Key event : \"" << _uniChar << "\" is down=" << _isdown);
		s_listInstance[_id]->ANDROID_SetKeyboard(_uniChar, _isdown);
	}
	
	void Java_org_ewol_Ewol_displayPropertyMetrics(JNIEnv* _env, jobject _thiz, jfloat _ratioX, jfloat _ratioY)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		// set the internal system ratio properties ...
		ewol::dimension::SetPixelRatio(vec2(_ratioX,_ratioY), ewol::Dimension::Inch);
	}
	
	// TODO : Set a return true or false if we want to grep this event ...
	void Java_org_ewol_Ewol_keyboardEventKeySystem(JNIEnv* _env, jobject _thiz, jint _keyVal, jboolean _isdown)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		switch (_keyVal) {
			case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_VOLUME_UP:
				EWOL_DEBUG("IO keyboard Key System \"VOLUME_UP\" is down=" << _isdown);
				break;
			case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_VOLUME_DOWN:
				EWOL_DEBUG("IO keyboard Key System \"VOLUME_DOWN\" is down=" << _isdown);
				break;
			case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_MENU:
				EWOL_DEBUG("IO keyboard Key System \"MENU\" is down=" << _isdown);
				break;
			case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_CAMERA:
				EWOL_DEBUG("IO keyboard Key System \"CAMERA\" is down=" << _isdown);
				break;
			case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_HOME:
				EWOL_DEBUG("IO keyboard Key System \"HOME\" is down=" << _isdown);
				break;
			case org_ewol_EwolConstants_EWOL_SYSTEM_KEY_POWER:
				EWOL_DEBUG("IO keyboard Key System \"POWER\" is down=" << _isdown);
				break;
			default:
				EWOL_DEBUG("IO keyboard Key System event : \"" << _keyVal << "\" is down=" << _isdown);
				break;
		}
	}
	
	
	/* **********************************************************************************************
	 * **	Renderer section :
	 * ********************************************************************************************** */
	void Java_org_ewol_Ewol_renderInit(JNIEnv* _env, jobject _thiz)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		
	}
	
	void Java_org_ewol_Ewol_renderResize( JNIEnv* _env, jobject _thiz, jint _w, jint _h )
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->OS_Resize(vec2(_w, _h));
	}
	
	// TODO : Return tur or foalse to not redraw when the under draw has not be done (processing gain of time)
	void Java_org_ewol_Ewol_renderDraw(JNIEnv* _env, jobject _thiz)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		s_listInstance[_id]->OS_Draw(true);
	}
	
	void Java_org_ewol_Ewol_audioPlayback(JNIEnv* _env, void* _reserved, jshortArray _location, jint _frameRate, jint _nbChannels)
	{
		esize_t _id = 0; // uint
		if(    _id>=s_listInstance.Size()
		    || NULL==s_listInstance[_id] ) {
			EWOL_ERROR("Call C With an incorrect instance");
			// TODO : Generate error in java to stop the current instance
			return;
		}
		// Get the short* pointer from the Java array
		jboolean isCopy;
		jshort* dst = _env->GetShortArrayElements(_location, &isCopy);
		if (NULL != dst) {
			ewol::audio::GetData(dst, _frameRate, _nbChannels);
		}
		//APPL_DEBUG("IO Audio event request: Frames=" << frameRate << " channels=" << nbChannels);
		// TODO : Understand why it did not work corectly ...
		//if (isCopy == JNI_TRUE) {
		// Release the short* pointer
		_env->ReleaseShortArrayElements(_location, dst, 0);
		//}
	}
};


int ewol::Run(int _argc, const char *_argv[])
{
	// Never call but needed ...
}
