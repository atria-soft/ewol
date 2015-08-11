/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_CONTEXT_H__
#define __EWOL_CONTEXT_H__

#include <gale/key/key.h>
#include <gale/Application.h>
#include <gale/context/Context.h>
#include <gale/context/clipBoard.h>
#include <gale/context/commandLine.h>

#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/object/Manager.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/Windows.h>
#include <ewol/context/Application.h>
#include <ewol/context/ConfigFont.h>
#include <ewol/context/InputManager.h>
#include <memory>

namespace ewol {
	// Here we hereted from the gale application to be agnostic of the OW where we work ...
	class Context : public gale::Application {
		private:
			std::shared_ptr<ewol::context::Application> m_application; //!< Application handle
		public:
			std::shared_ptr<ewol::context::Application> getApplication() {
				return m_application;
			}
		public:
			gale::context::CommandLine& getCmd() {
				return gale::getContext().getCmd();
			};
		private:
			ewol::context::ConfigFont m_configFont; //!< global font configuration
		public:
			ewol::context::ConfigFont& getFontDefault() {
				return m_configFont;
			};
		private:
			ewol::object::Manager m_objectManager; //!< Object Manager main instance
		public:
			ewol::object::Manager& getEObjectManager() {
				return m_objectManager;
			};
		private:
			ewol::widget::Manager m_widgetManager; //!< global widget manager
		public:
			ewol::widget::Manager& getWidgetManager() {
				return m_widgetManager;
			};
		public:
			gale::resource::Manager& getResourcesManager() {
				return gale::getContext().getResourcesManager();
			};
		public:
			Context(ewol::context::Application* _application);
			virtual ~Context();
		private:
			ewol::context::InputManager m_input;
		public: // herited function:
			virtual void onCreate(gale::Context& _context);
			virtual void onStart(gale::Context& _context);
			virtual void onResume(gale::Context& _context);
			virtual void onRegenerateDisplay(gale::Context& _context);
			virtual void onDraw(gale::Context& _context);
			virtual void onPause(gale::Context& _context);
			virtual void onStop(gale::Context& _context);
			virtual void onDestroy(gale::Context& _context);
			virtual void onPointer(enum gale::key::type _type,
			                       int32_t _pointerID,
			                       const vec2& _pos,
			                       gale::key::status _state);
			virtual void onKeyboard(gale::key::Special& _special,
			                        enum gale::key::keyboard _type,
			                        char32_t _value,
			                        gale::key::status _state);
			virtual void onClipboardEvent(enum gale::context::clipBoard::clipboardListe _clipboardId);
			
		public:
			/**
			 * @brief reset event management for the IO like Input ou Mouse or keyborad
			 */
			void resetIOEvent();
			/**
			 * @brief The application request that the Window will be killed
			 */
			virtual void stop();
		private:
			std::shared_ptr<ewol::widget::Windows> m_windowsCurrent; //!< curent displayed windows
		public:
			/**
			 * @brief set the current windows to display :
			 * @param _windows Windows that might be displayed
			 */
			void setWindows(const std::shared_ptr<ewol::widget::Windows>& _windows);
			/**
			 * @brief get the current windows that is displayed
			 * @return the current handle on the windows (can be null)
			 */
			std::shared_ptr<ewol::widget::Windows> getWindows();
			
			/**
			 * @brief Redraw all the windows
			 */
			void forceRedrawAll();
			
			// TODO : Later ...
			/**
			 * @brief This is to transfert the event from one widget to another one
			 * @param source the widget where the event came from
			 * @param destination the widget where the event mitgh be generated now
			 */
			void inputEventTransfertWidget(std::shared_ptr<ewol::Widget> _source, std::shared_ptr<ewol::Widget> _destination);
			/**
			 * @brief This fonction lock the pointer properties to move in relative instead of absolute
			 * @param[in] widget The widget that lock the pointer events
			 */
			void inputEventGrabPointer(std::shared_ptr<ewol::Widget> _widget);
			/**
			 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
			 */
			void inputEventUnGrabPointer();
			// herited function:
			virtual void onResize(const ivec2& _size);
			#if 0
			/**
			 * @brief Inform the Gui that we want to have a copy of the clipboard
			 * @param[in] _clipboardID ID of the clipboard (STD/SELECTION) only apear here
			 */
			virtual void clipBoardGet(enum ewol::context::clipBoard::clipboardListe _clipboardID);
			/**
			 * @brief Inform the Gui that we are the new owner of the clipboard
			 * @param[in] _clipboardID ID of the clipboard (STD/SELECTION) only apear here
			 */
			virtual void clipBoardSet(enum ewol::context::clipBoard::clipboardListe _clipboardID);
			/**
			 * @brief Call by the OS when a clipboard arrive to US (previously requested by a widget)
			 * @param[in] Id of the clipboard
			 */
			void OS_ClipBoardArrive(enum ewol::context::clipBoard::clipboardListe _clipboardID);
			#endif
			
			#if 0
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
			 * @brief get the curent time in micro-second
			 * @note : must be implemented in all system OS implementation
			 * @return The curent time of the process
			 */
			static int64_t getTime();
			#endif
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
		private:
			size_t m_initStepId;
			size_t m_initTotalStep;
		public:
			/**
			 * @brief Special for init (main) set the start image when loading data
			 * @param[in] _fileName Name of the image to load
			 */
			void setInitImage(const std::string& _fileName);
		protected:
			# if 0
			/**
			 * @brief HARDWARE keyboard event from the system
			 * @param[in] _key event type
			 * @param[in] _status Up or down status
			 * @return Keep the event or not
			 */
			virtual bool systemKeyboradEvent(enum gale:key::keyboardSystem _key, bool _down);
			#endif
	};
	/**
	 * @brief From everyware in the program, we can get the context inteface.
	 * @return current reference on the instance.
	 */
	Context& getContext();
};

#endif

