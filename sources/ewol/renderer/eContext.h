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
#include <ewol/commandLine.h>

// TODO : Remove this from here ...
class eSystemMessage {
	public:
		enum theadMessage {
			msgNone,
			msgInit,
			msgRecalculateSize,
			msgResize,
			msgHide,
			msgShow,
			
			msgInputMotion,
			msgInputState,
			
			msgKeyboardKey,
			msgKeyboardMove,
			
			msgClipboardArrive
		};
	public :
		// specify the message type
		enum theadMessage TypeMessage;
		// can not set a union ...
		enum ewol::clipBoard::clipboardListe clipboardID;
		// InputId
		enum ewol::keyEvent::type inputType;
		int32_t                 inputId;
		// generic dimentions
		vec2 dimention;
		// keyboard events :
		bool                        repeateKey;  //!< special flag for the repeating key on the PC interface
		bool                        stateIsDown;
		char32_t                   keyboardChar;
		enum ewol::keyEvent::keyboard keyboardMove;
		ewol::SpecialKey            keyboardSpecial;
		
		eSystemMessage(void) :
			TypeMessage(msgNone),
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

namespace ewol {
	enum orientation{
		screenAuto = 0,
		screenLandscape,
		screenPortrait
	};
	
	class eContext {
		private:
			ewol::CommandLine m_commandLine; //!< Start command line information
		public:
			ewol::CommandLine& getCmd(void) {
				return m_commandLine;
			};
		private:
			ewol::ConfigFont m_configFont; //!< global font configuration
		public:
			ewol::ConfigFont& getFontDefault(void) {
				return m_configFont;
			};
		private:
			ewol::WidgetManager m_widgetManager; //!< global widget manager
		public:
			ewol::WidgetManager& getWidgetManager(void) {
				return m_widgetManager;
			};
		private:
			ewol::EObjectManager m_EObjectManager; //!< eObject Manager main instance
		public:
			ewol::EObjectManager& getEObjectManager(void) {
				return m_EObjectManager;
			};
		private:
			ewol::ResourceManager m_resourceManager; //!< global resources Manager
		public:
			ewol::ResourceManager& getResourcesManager(void) {
				return m_resourceManager;
			};
		public:
			eContext(int32_t _argc=0, const char* _argv[]=NULL);
			virtual ~eContext(void);
		protected:
			/**
			 * @brief set the curent interface.
			 * @note this lock the main mutex
			 */
			void lockContext(void);
			/**
			 * @brief set the curent interface at NULL.
			 * @note this un-lock the main mutex
			 */
			void unLockContext(void);
		private:
			int64_t m_previousDisplayTime;  // this is to limit framerate ... in case...
			ewol::eInput m_input;
			etk::MessageFifo<eSystemMessage> m_msgSystem;
			bool m_displayFps;
			ewol::Fps m_FpsSystemEvent;
			ewol::Fps m_FpsSystemContext;
			ewol::Fps m_FpsSystem;
			ewol::Fps m_FpsFlush;
			/**
			 * @brief Processing all the event arrived ... (commoly called in draw function)
			 */
			void processEvents(void);
		public:
			
			virtual void setArchiveDir(int _mode, const char* _str);
			
			virtual void OS_SetInputMotion(int _pointerID, const vec2& _pos);
			virtual void OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos);
			
			virtual void OS_SetMouseMotion(int _pointerID, const vec2& _pos);
			virtual void OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos);
			
			virtual void OS_SetKeyboard(ewol::SpecialKey& _special,
			                            char32_t _myChar,
			                            bool _isDown,
			                            bool _isARepeateKey=false);
			virtual void OS_SetKeyboardMove(ewol::SpecialKey& _special,
			                                enum ewol::keyEvent::keyboard _move,
			                                bool _isDown,
			                                bool _isARepeateKey=false);
			/**
			 * @brief The current context is suspended
			 */
			virtual void OS_Suspend(void);
			/**
			 * @brief The current context is resumed
			 */
			virtual void OS_Resume(void);
			
			void requestUpdateSize(void);
			
			// return true if a flush is needed
			bool OS_Draw(bool _displayEveryTime);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject removed  == > the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 */
			void onObjectRemove(ewol::EObject* _removeObject);
			/**
			 * @brief reset event management for the IO like Input ou Mouse or keyborad
			 */
			void resetIOEvent(void);
			/**
			 * @brief The OS inform that the openGL constext has been destroy  == > use to automaticly reload the texture and other thinks ...
			 */
			void OS_OpenGlContextDestroy(void);
			/**
			 * @brief The OS Inform that the Window has been killed
			 */
			void OS_Stop(void);
			/**
			 * @brief The application request that the Window will be killed
			 */
			virtual void stop(void);
		private:
			ewol::Windows* m_windowsCurrent; //!< curent displayed windows
		public:
			/**
			 * @brief set the current windows to display :
			 * @param _windows Windows that might be displayed
			 */
			void setWindows(ewol::Windows* _windows);
			/**
			 * @brief get the current windows that is displayed
			 * @return the current handle on the windows (can be null)
			 */
			ewol::Windows* getWindows(void) {
				return m_windowsCurrent;
			};
		private:
			vec2 m_windowsSize; //!< current size of the system
		public:
			/**
			 * @brief get the current windows size
			 * @return the current size ...
			 */
			const vec2& getSize(void) {
				return m_windowsSize;
			};
			/**
			 * @brief The OS inform that the current windows has change his size.
			 * @param[in] _size new size of the windows.
			 */
			virtual void OS_Resize(const vec2& _size);
			/**
			 * @brief The application request a change of his curent size.
			 * @param[in] _size new Requested size of the windows.
			 */
			virtual void setSize(const vec2& _size);
			/**
			 * @brief The OS inform that the current windows has change his position.
			 * @param[in] _pos New position of the Windows.
			 */
			void OS_Move(const vec2& _pos);
			/**
			 * @brief The Application request that the current windows will change his position.
			 * @param[in] _pos New position of the Windows requested.
			 */
			virtual void setPos(const vec2& _pos);
			/**
			 * @brief The OS inform that the Windows is now Hidden.
			 */
			void OS_Hide(void);
			/**
			 * @brief The Application request that the Windows will be Hidden.
			 */
			virtual void hide(void);
			/**
			 * @brief The OS inform that the Windows is now visible.
			 */
			void OS_Show(void);
			/**
			 * @brief The Application request that the Windows will be visible.
			 */
			virtual void show(void);
			/**
			 * @brief Redraw all the windows
			 */
			void forceRedrawAll(void);
			
			// TODO : Later ...
			/**
			 * @brief This is to transfert the event from one widget to another one
			 * @param source the widget where the event came from
			 * @param destination the widget where the event mitgh be generated now
			 */
			void inputEventTransfertWidget(ewol::Widget* _source, ewol::Widget* _destination);
			/**
			 * @brief This fonction lock the pointer properties to move in relative instead of absolute
			 * @param[in] widget The widget that lock the pointer events
			 */
			void inputEventGrabPointer(ewol::Widget* _widget);
			/**
			 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
			 */
			void inputEventUnGrabPointer(void);
			
			/**
			 * @brief display the virtal keyboard (for touch system only)
			 */
			virtual void keyboardShow(void);
			/**
			 * @brief Hide the virtal keyboard (for touch system only)
			 */
			virtual void keyboardHide(void);
			
			/**
			 * @brief Inform the Gui that we want to have a copy of the clipboard
			 * @param[in] _clipboardID ID of the clipboard (STD/SELECTION) only apear here
			 */
			virtual void clipBoardGet(enum ewol::clipBoard::clipboardListe _clipboardID) { };
			/**
			 * @brief Inform the Gui that we are the new owner of the clipboard
			 * @param[in] _clipboardID ID of the clipboard (STD/SELECTION) only apear here
			 */
			virtual void clipBoardSet(enum ewol::clipBoard::clipboardListe _clipboardID) { };
			/**
			 * @brief Call by the OS when a clipboard arrive to US (previously requested by a widget)
			 * @param[in] Id of the clipboard
			 */
			void OS_ClipBoardArrive(enum ewol::clipBoard::clipboardListe _clipboardID);
			/**
			 * @brief set the new title of the windows
			 * @param[in] title New desired title
			 */
			virtual void setTitle(const std::string& _title);
			/**
			 * @brief force the screen orientation (availlable on portable elements ...
			 * @param[in] _orientation Selected orientation.
			 */
			virtual void forceOrientation(enum ewol::orientation _orientation) { };
			/**
			 * @brief get all the event from the X system
			 * @param[in] _isGrabbed "true" if all the event will be get, false if we want only ours.
			 * @param[in] _forcedPosition the position where the mouse might be reset at  every events ...
			 */
			virtual void grabPointerEvents(bool _isGrabbed, const vec2& _forcedPosition) { };
			/**
			 * @brief set the cursor display type.
			 * @param[in] _newCursor selected new cursor.
			 */
			virtual void setCursor(enum ewol::cursorDisplay _newCursor) { };
			/**
			 * @brief set the Icon of the program
			 * @param[in] _inputFile new filename icon of the curent program.
			 */
			virtual void setIcon(const std::string& _inputFile) { };
			/**
			 * @brief get the curent time in micro-second
			 * @note : must be implemented in all system OS implementation
			 * @return The curent time of the process
			 */
			static int64_t getTime(void);
		private:
			// TODO : set user argument here ....
			
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
	eContext& getContext(void);
};

//!< must be define in CPP by the application ... this are the main init and unInit of the Application
// return false if an error occured
bool APP_Init(ewol::eContext& _context);
void APP_UnInit(ewol::eContext& _context);


#endif

