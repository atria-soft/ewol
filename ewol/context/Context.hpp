/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <gale/key/key.hpp>
#include <gale/Application.hpp>
#include <gale/context/Context.hpp>
#include <gale/context/clipBoard.hpp>
#include <gale/context/commandLine.hpp>

#include <ewol/debug.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/context/Application.hpp>
#include <ewol/context/ConfigFont.hpp>
#include <ewol/context/InputManager.hpp>
#include <ememory/memory.hpp>

namespace ewol {
	// Here we hereted from the gale application to be agnostic of the OW where we work ...
	class Context : public gale::Application {
		private:
			ememory::SharedPtr<ewol::context::Application> m_application; //!< Application handle
		public:
			ememory::SharedPtr<ewol::context::Application> getApplication() {
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
			void onCreate(gale::Context& _context) override;
			void onStart(gale::Context& _context) override;
			void onResume(gale::Context& _context) override;
			void onRegenerateDisplay(gale::Context& _context) override;
			void onDraw(gale::Context& _context) override;
			void onPause(gale::Context& _context) override;
			void onStop(gale::Context& _context) override;
			void onDestroy(gale::Context& _context) override;
			void onKillDemand(gale::Context& _context) override;
			void onPointer(enum gale::key::type _type,
			               int32_t _pointerID,
			               const vec2& _pos,
			               gale::key::status _state) override;
			void onKeyboard(const gale::key::Special& _special,
			                enum gale::key::keyboard _type,
			                char32_t _value,
			                gale::key::status _state) override;
			void onClipboardEvent(enum gale::context::clipBoard::clipboardListe _clipboardId) override;
		public:
			/**
			 * @brief reset event management for the IO like Input ou Mouse or keyborad
			 */
			void resetIOEvent();
		private:
			ewol::widget::WindowsShared m_windowsCurrent; //!< curent displayed windows
		public:
			/**
			 * @brief set the current windows to display :
			 * @param _windows Windows that might be displayed
			 */
			void setWindows(const ewol::widget::WindowsShared& _windows);
			/**
			 * @brief get the current windows that is displayed
			 * @return the current handle on the windows (can be null)
			 */
			ewol::widget::WindowsShared getWindows();
			
			/**
			 * @brief Redraw all the windows
			 */
			void forceRedrawAll();
			
			/**
			 * @brief This is to transfert the event from one widget to another one
			 * @param source the widget where the event came from
			 * @param destination the widget where the event mitgh be generated now
			 */
			void inputEventTransfertWidget(ewol::WidgetShared _source, ewol::WidgetShared _destination);
			/**
			 * @brief This fonction lock the pointer properties to move in relative instead of absolute
			 * @param[in] widget The widget that lock the pointer events
			 */
			void inputEventGrabPointer(ewol::WidgetShared _widget);
			/**
			 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
			 */
			void inputEventUnGrabPointer();
			void onResize(const ivec2& _size) override;
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
		public:
			/**
			 * @brief Request a display after call a resize
			 */
			void requestUpdateSize();
			void onPeriod(int64_t _time) override;
	};
	/**
	 * @brief From everyware in the program, we can get the context inteface.
	 * @return current reference on the instance.
	 */
	Context& getContext();
};

