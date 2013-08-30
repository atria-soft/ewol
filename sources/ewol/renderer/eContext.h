/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CONTEXT_H__
#define __EWOL_CONTEXT_H__

#include <etk/types.h>
#include <etk/MessageFifo.h>
#include <ewol/key.h>
#include <ewol/ewol.h>
#include <ewol/clipBoard.h>
#include <ewol/widget/Windows.h>
#include <ewol/renderer/eInput.h>
#include <ewol/renderer/Fps.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/ConfigFont.h>
#include <ewol/renderer/EObjectManager.h>
#include <ewol/resources/ResourceManager.h>


// TODO : Remove this from here ...
typedef enum {
	THREAD_NONE,
	THREAD_INIT,
	THREAD_RECALCULATE_SIZE,
	THREAD_RESIZE,
	THREAD_HIDE,
	THREAD_SHOW,
	
	THREAD_INPUT_MOTION,
	THREAD_INPUT_STATE,
	
	THREAD_KEYBORAD_KEY,
	THREAD_KEYBORAD_MOVE,
	
	THREAD_CLIPBOARD_ARRIVE,
} theadMessage_te;
// TODO : Remove this from here ...
class eSystemMessage {
	public :
		// specify the message type
		theadMessage_te TypeMessage;
		// can not set a union ...
		ewol::clipBoard::clipboardListe_te clipboardID;
		// InputId
		ewol::keyEvent::type_te inputType;
		int32_t                 inputId;
		// generic dimentions
		vec2 dimention;
		// keyboard events :
		bool                        repeateKey;  //!< special flag for the repeating key on the PC interface
		bool                        stateIsDown;
		uniChar_t                   keyboardChar;
		ewol::keyEvent::keyboard_te keyboardMove;
		ewol::SpecialKey            keyboardSpecial;
		
		eSystemMessage(void) :
			TypeMessage(THREAD_NONE),
			clipboardID(ewol::clipBoard::clipboardStd),
			inputType(ewol::keyEvent::typeUnknow),
			inputId(-1),
			dimention(0,0),
			repeateKey(false),
			stateIsDown(false),
			keyboardChar(0),
			keyboardMove(ewol::keyEvent::keyboardUnknow)
		{
			
		}
};

namespace ewol
{
	class eContext
	{
		private:
			ewol::ConfigFont m_configFont; //!< global font configuration
		public:
			ewol::ConfigFont& GetFontDefault(void) { return m_configFont; };
		private:
			ewol::WidgetManager m_widgetManager; //!< global widget manager
		public:
			ewol::WidgetManager& GetWidgetManager(void) { return m_widgetManager; };
		private:
			ewol::EObjectManager m_EObjectManager; //!< eObject Manager main instance
		public:
			ewol::EObjectManager& GetEObjectManager(void) { return m_EObjectManager; };
		private:
			ewol::ResourceManager m_resourceManager; //!< global resources Manager
		public:
			ewol::ResourceManager& GetResourcesManager(void) { return m_resourceManager; };
		public:
			eContext(void);
			virtual ~eContext(void);
		protected:
			/**
			 * @brief Set the curent interface.
			 * @note this lock the main mutex
			 */
			void LockContext(void);
			/**
			 * @brief Set the curent interface at NULL.
			 * @note this un-lock the main mutex
			 */
			void UnLockContext(void);
		private:
			int64_t m_previousDisplayTime;  // this is to limit framerate ... in case...
			ewol::eInput m_input;
			etk::MessageFifo<eSystemMessage> m_msgSystem;
			ewol::Fps m_FpsSystemEvent;
			ewol::Fps m_FpsSystemContext;
			ewol::Fps m_FpsSystem;
			ewol::Fps m_FpsFlush;
			/**
			 * @brief Processing all the event arrived ... (commoly called in draw function)
			 */
			void ProcessEvents(void);
		public:
			
			void SetArchiveDir(int _mode, const char* _str);
			
			
			void OS_SetInputMotion(int _pointerID, const vec2& _pos);
			void OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos);
			
			void OS_SetMouseMotion(int _pointerID, const vec2& _pos);
			void OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos);
			
			void OS_SetKeyboard(ewol::SpecialKey& _special,
			                    uniChar_t _myChar,
			                    bool _isDown,
			                    bool _isARepeateKey=false);
			void OS_SetKeyboardMove(ewol::SpecialKey& _special,
			                        ewol::keyEvent::keyboard_te _move,
			                        bool _isDown,
			                        bool _isARepeateKey=false);
			
			void OS_SetClipBoard(ewol::clipBoard::clipboardListe_te _clipboardID);
			
			void RequestUpdateSize(void);
			
			// return true if a flush is needed
			bool OS_Draw(bool _displayEveryTime);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject removed ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 */
			void OnObjectRemove(ewol::EObject * removeObject) {
				// TODO : I did not remember what I must do ...
			};
			/**
			 * @brief reset event management for the IO like Input ou Mouse or keyborad
			 */
			void ResetIOEvent(void);
			/**
			 * @brief The OS inform that the OpenGL constext has been destroy ==> use to automaticly reload the texture and other thinks ...
			 */
			void OS_OpenGlContextDestroy(void);
			/**
			 * @brief The OS Inform that the Window has been killed
			 */
			void OS_Stop(void);
			/**
			 * @brief The application request that the Window will be killed
			 */
			virtual void Stop(void) { };
		private:
			ewol::Windows* m_windowsCurrent; //!< curent displayed windows
		public:
			/**
			 * @brief set the current windows to display :
			 * @param _windows Windows that might be displayed
			 */
			void SetWindows(ewol::Windows* _windows);
			/**
			 * @brief Get the current windows that is displayed
			 * @return the current handle on the windows (can be null)
			 */
			ewol::Windows* GetWindows(void) { return m_windowsCurrent; };
		private:
			vec2 m_windowsSize; //!< current size of the system
		public:
			/**
			 * @brief Get the current windows size
			 * @return the current size ...
			 */
			const vec2& GetSize(void) { return m_windowsSize; };
			/**
			 * @brief The OS inform that the current windows has change his size.
			 * @param[in] _size new size of the windows.
			 */
			void OS_Resize(const vec2& _size);
			/**
			 * @brief The application request a change of his curent size.
			 * @param[in] _size new Requested size of the windows.
			 */
			virtual void SetSize(const vec2& _size) { };
			/**
			 * @brief The OS inform that the current windows has change his position.
			 * @param[in] _pos New position of the Windows.
			 */
			void OS_Move(const vec2& _pos);
			/**
			 * @brief The Application request that the current windows will change his position.
			 * @param[in] _pos New position of the Windows requested.
			 */
			virtual void SetPos(const vec2& _pos) { };
			/**
			 * @brief The OS inform that the Windows is now Hidden.
			 */
			void OS_Hide(void);
			/**
			 * @brief The Application request that the Windows will be Hidden.
			 */
			virtual void Hide(void) { };
			/**
			 * @brief The OS inform that the Windows is now visible.
			 */
			void OS_Show(void);
			/**
			 * @brief The Application request that the Windows will be visible.
			 */
			virtual void Show(void) { };
			/**
			 * @brief Redraw all the windows
			 */
			void ForceRedrawAll(void);
			
			// TODO : Later ...
			/**
			 * @brief This is to transfert the event from one widget to another one
			 * @param source the widget where the event came from
			 * @param destination the widget where the event mitgh be generated now
			 */
			void InputEventTransfertWidget(ewol::Widget* _source, ewol::Widget* _destination);
			/**
			 * @brief This fonction lock the pointer properties to move in relative instead of absolute
			 * @param[in] widget The widget that lock the pointer events
			 */
			void InputEventGrabPointer(ewol::Widget* _widget);
			/**
			 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
			 */
			void InputEventUnGrabPointer(void);
			
			/**
			 * @brief Display the virtal keyboard (for touch system only)
			 */
			void KeyboardShow(void) {};
			/**
			 * @brief Hide the virtal keyboard (for touch system only)
			 */
			void KeyboardHide(void) {};
			
			/**
			 * @brief Inform the Gui that we want to have a copy of the clipboard
			 * @param[in] _clipboardID ID of the clipboard (STD/SELECTION) only apear here
			 */
			virtual void ClipBoardGet(ewol::clipBoard::clipboardListe_te _clipboardID) { };
			/**
			 * @brief Inform the Gui that we are the new owner of the clipboard
			 * @param[in] _clipboardID ID of the clipboard (STD/SELECTION) only apear here
			 */
			virtual void ClipBoardSet(ewol::clipBoard::clipboardListe_te _clipboardID) { };
			/**
			 * @brief Call by the OS when a clipboard arrive to US (previously requested by a widget)
			 * @param[in] Id of the clipboard
			 */
			void OS_ClipBoardArrive(ewol::clipBoard::clipboardListe_te _clipboardID);
			/**
			 * @brief Set the new title of the windows
			 * @param[in] title New desired title
			 */
			virtual void SetTitle(etk::UString& _title) { };
			/**
			 * @brief Force the screen orientation (availlable on portable elements ...
			 * @param[in] _orientation Selected orientation.
			 */
			virtual void ForceOrientation(ewol::orientation_te _orientation) { };
			/**
			 * @brief Get all the event from the X system
			 * @param[in] _isGrabbed "true" if all the event will be get, false if we want only ours.
			 * @param[in] _forcedPosition the position where the mouse might be reset at  every events ...
			 */
			virtual void GrabPointerEvents(bool _isGrabbed, const vec2& _forcedPosition) { };
			/**
			 * @brief Set the cursor display type.
			 * @param[in] _newCursor selected new cursor.
			 */
			virtual void SetCursor(ewol::cursorDisplay_te _newCursor) { };
			/**
			 * @brief Set the Icon of the program
			 * @param[in] _inputFile new filename icon of the curent program.
			 */
			virtual void SetIcon(const etk::UString& _inputFile) { };
			/**
			 * @brief Get the curent time in micro-second
			 * @note : must be implemented in all system OS implementation
			 * @return The curent time of the process
			 */
			static int64_t GetTime(void);
		private:
			// TODO : Set user argument here ....
			
		public:
			/**
			 * @brief This is the only one things the User might done in his main();
			 * @note : must be implemented in all system OPS implementation
			 * @note To answare you before you ask the question, this is really simple:
			 *       Due to the fect that the current system is multiple-platform, you "main"
			 *       Does not exist in the android platform, then ewol call other start 
			 *       and stop function, to permit to have only one code
			 * @note The main can not be in the ewol, due to the fact thet is an librairy
			 * @param[in] _argc Standard argc
			 * @param[in] _argv Standard argv
			 * @return normal error int for the application error management
			 */
			static int main(int _argc, const char *_argv[]);
	};
	/**
	 * @brief From everyware in the program, we can get the context inteface.
	 * @return current reference on the instance.
	 */
	eContext& GetContext(void);
};

//!< must be define in CPP by the application ... this are the main init and unInit of the Application
// return false if an error occured
bool APP_Init(ewol::eSystem& _system);
void APP_UnInit(ewol::eSystem& _system);


#endif

