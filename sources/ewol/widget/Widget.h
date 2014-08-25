/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_WIDGET_H__
#define __EWOL_WIDGET_H__

#include <vector>
#include <etk/types.h>
#include <etk/math/Vector2D.h>

#include <ewol/debug.h>
#include <ewol/object/Object.h>
#include <ewol/Dimension.h>

namespace ewol {
	class Widget;
	namespace widget {
		class Manager;
		class Windows;
	};
};
#include <ewol/context/clipBoard.h>
#include <ewol/key/key.h>
#include <ewol/context/cursor.h>
#include <ewol/event/Input.h>
#include <ewol/event/Entry.h>
#include <ewol/event/Time.h>
#include <ewol/translate.h>
#include <ewol/object/Signal.h>

#define ULTIMATE_MAX_SIZE  (99999999)

#define DECLARE_WIDGET_FACTORY(className, name) \
	DECLARE_FACTORY(className); \
	static void createManagerWidget(ewol::widget::Manager& _widgetManager) { \
		_widgetManager.addWidgetCreator(name,[]() -> std::shared_ptr<ewol::Widget> { return className::create(); }); \
	}

namespace ewol {
	/**
	 * @not-in-doc
	 */
	class DrawProperty{
		/*
		                                                          /--> m_windowsSize
		      *--------------------------------------------------*
		      |                                           g       |
		      |                                                  |
		      |                                    m_size        |
		      |                                   /              |
		      |              o-------------------o               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              o-------------------o               |
		      |             /                                    |
		      |     m_origin                                     |
		      |                                                  |
		      *--------------------------------------------------*
		     /
		   (0,0)
		 */
		public :
			ivec2 m_windowsSize; //!< Windows compleate size
			ivec2 m_origin; //!< Windows clipping upper widget (can not be <0)
			ivec2 m_size; //!< Windows clipping upper widget (can not be <0 and >m_windowsSize)
			void limit(const vec2& _origin, const vec2& _size);
	};
	std::ostream& operator <<(std::ostream& _os, const ewol::DrawProperty& _obj);
	/**
	 * @brief Gravity of the widget property
	 * @not-in-doc
	 */
	enum gravity {
		gravityCenter=0x00, //!< gravity is in certer
		gravityTopLeft=0x05,
		gravityTop=0x01,
		gravityTopRight=0x03,
		gravityRight=0x02,
		gravityButtomRight=0x06,
		gravityButtom=0x04,
		gravityButtomLeft=0x0C,
		gravityLeft=0x08,
	};
	std::ostream& operator <<(std::ostream& _os, const enum ewol::gravity _obj);
	std::string gravityToString(const enum ewol::gravity _obj);
	enum ewol::gravity stringToGravity(const std::string& _obj);
	/**
	 * @not-in-doc
	 */
	class EventShortCut {
		public:
			std::string message; //!< data link with the event
			ewol::key::Special specialKey; //!< special board key
			char32_t unicodeValue; //!< 0 if not used
			enum ewol::key::keyboard keyboardMoveValue; //!< ewol::EVENT_KB_MOVE_TYPE_NONE if not used
			EventShortCut() {
				message = "";
				unicodeValue = 0;
				keyboardMoveValue = ewol::key::keyboardUnknow;
			};
			virtual ~EventShortCut() { };
	};
	/**
	 * @brief Widget class is the main widget interface, it hase some generic properties: 
	 * :** known his parent
	 * :** Can be display at a special position with a special scale
	 * :** Can get focus
	 * :** Receive Event (keyboard / mouse / ...)
	 * 
	 */
	class Widget : public ewol::Object {
		protected:
			/**
			 * @brief Constructor of the widget classes
			 * @return (no execption generated (not managed in embended platform))
			 */
			Widget();
			
			void init();
			void init(const std::string& _name);
		public:
			/**
			 * @brief Destructor of the widget classes
			 */
			virtual ~Widget();
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget size:
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			vec2 m_size; //!< internal : current size of the widget
			vec2 m_minSize; //!< internal : minimum size of the widget
			vec2 m_maxSize; //!< internal : maximum size of the widget
		public:
			/**
			 * @brief convert the absolute position in the local Position (Relative)
			 * @param[in] _pos Absolute position that you request convertion
			 * @return the relative position
			 */
			virtual vec2 relativePosition(const vec2& _pos);
			/**
			 * @brief Parent set the possible diplay size of the current widget whith his own possibilities
			 *        By default this save the widget available size in the widget size
			 * @param[in] _available Available x&y pixel size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void calculateSize(const vec2& _available);
			/**
			 * @brief get the widget size
			 * @return Requested size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual vec2 getSize();
			/**
			 * @brief calculate the minimum and maximum size (need to estimate expend properties of the widget)
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void calculateMinMaxSize();
			/**
			 * @brief get the widget minimum size calculated
			 * @return Requested size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual vec2 getCalculateMinSize();
			/**
			 * @brief get the widget maximum size calculated
			 * @return Requested size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual vec2 getCalculateMaxSize();
		protected:
			vec2 m_offset; //!< Offset of the display in the viewport
		public:
			/**
			 * @brief set the zoom property of the widget.
			 * @param[in] _newVal offset value.
			 */
			virtual void setOffset(const vec2& _newVal);
			/**
			 * @brief get the offset property of the widget.
			 * @return The current offset value.
			 */
			virtual const vec2& getOffset() {
				return m_offset;
			};
		protected:
			// internal element calculated by the system
			float m_zoom; //!< generic widget zoom
		public:
			/**
			 * @brief set the zoom property of the widget
			 * @param[in] _newVal newZoom value
			 */
			virtual void setZoom(float _newVal);
			/**
			 * @brief get the zoom property of the widget
			 * @return the current zoom value
			 */
			virtual float getZoom();
			/**
			 * @brief Change Zoom property.
			 * @param[in] _range Range of the zoom change.
			 */
			virtual void changeZoom(float _range) {};
		protected:
			vec2 m_origin; //!< internal ... I do not really known how i can use it ...
		public:
			/**
			 * @brief set origin at the widget (must be an parrent widget that set this parameter).
			 * This represent the absolute origin in the program windows
			 * @param[in] _pos Position of the origin
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void setOrigin(const vec2& _pos);
			/**
			 * @brief get the origin (obsolute position in the windows)
			 * @return coordonate of the origin requested
			 */
			virtual vec2 getOrigin();
		protected:
			ewol::object::Param<ewol::Dimension> m_userMinSize; //!< user define the minimum size of the widget
		public:
			/**
			 * @brief User set the minimum size he want to set the display
			 * @param[in] _size set minimum size (none : 0)
			 */
			void setMinSize(const ewol::Dimension& _size) {
				m_userMinSize.set(_size);
			}
			/**
			 * @brief User set No minimum size.
			 */
			void setNoMinSize();
			/**
			 * @brief get the current calculated min size
			 * @return the size requested
			 */
			const ewol::Dimension& getMinSize() {
				return m_userMinSize.get();
			};
			/**
			 * @brief Check if the current min size is compatible with the user minimum size
			 *        If it is not the user minimum size will overWrite the minimum size set.
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void checkMinSize();
		protected:
			ewol::object::Param<ewol::Dimension> m_userMaxSize; //!< user define the maximum size of the widget
		public:
			/**
			 * @brief User set the maximum size he want to set the display
			 * @param[in] _size The new maximum size requested (vec2(0,0) to unset)
			 */
			void setMaxSize(const ewol::Dimension& _size) {
				m_userMaxSize.set(_size);
			}
			/**
			 * @brief User set No maximum size.
			 */
			void setNoMaxSize();
			/**
			 * @brief get the current maximum size
			 * @return the size requested
			 */
			const ewol::Dimension& getMaxSize() {
				return m_userMaxSize.get();
			};
			/**
			 * @brief Check if the current max size is compatible with the user maximum size
			 *        If it is not the user maximum size will overWrite the maximum size set.
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void checkMaxSize();
		protected:
			ewol::object::Param<bvec2> m_userExpand;
		public:
			/**
			 * @brief set the expend capabilities (x&y)
			 * @param[in] _newExpend 2D boolean repensent the capacity to expend
			 */
			virtual void setExpand(const bvec2& _newExpand) {
				m_userExpand.set(_newExpand);
			}
			/**
			 * @brief get the expend capabilities (x&y) (set by the user)
			 * @return 2D boolean repensent the capacity to expend
			 */
			virtual bvec2 getExpand() {
				return m_userExpand;
			};
			/**
			 * @brief get the expend capabilities (x&y)
			 * @return 2D boolean repensent the capacity to expend
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual bvec2 canExpand();
		protected:
			ewol::object::Param<bvec2> m_userFill;
		public:
			/**
			 * @brief set the x&y filling capacity
			 * @param[in] _newFill new x&y fill state
			 */
			virtual void setFill(const bvec2& _newFill) {
				m_userFill.set(_newFill);
			}
			/**
			 * @brief set the x&y filling capacity set by the user
			 * @return bvec2 repensent the capacity to x&y filling (set by the user)
			 */
			virtual const bvec2& getFill() {
				return m_userFill;
			};
			/**
			 * @brief get the filling capabilities x&y
			 * @return bvec2 repensent the capacity to x&y filling
			 * @note : INTERNAL EWOL SYSTEM
			 */
			const bvec2& canFill();
		protected:
			ewol::object::Param<bool> m_hide; //!< hide a widget on the display
		public:
			/**
			 * @brief set the widget hidden
			 */
			virtual void hide() {
				m_hide.set(true);
			}
			/**
			 * @brief set the widget visible
			 */
			virtual void show() {
				m_hide.set(false);
			}
			/**
			 * @brief get the visibility of the widget
			 * @return true: if the widget is hiden, false: it is visible
			 */
			virtual bool isHide() {
				return m_hide;
			};
		
		protected:
			ewol::object::ParamList<enum ewol::gravity> m_gravity; //!< Gravity of the widget
		public:
			/**
			 * @brief set the widget gravity
			 * @param[in] _gravity New gravity of the widget
			 */
			virtual void setGravity(enum ewol::gravity _gravity) {
				m_gravity.set(_gravity);
			}
			/**
			 * @brief get the widget gravity
			 * @return the gravity type
			 */
			virtual enum ewol::gravity getGravity() {
				return m_gravity;
			};
		// ----------------------------------------------------------------------------------------------------------------
		// -- focus Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool m_hasFocus; //!< set the focus on this widget
			ewol::object::Param<bool> m_canFocus; //!< the focus can be done on this widget
		public:
			/**
			 * @brief get the focus state of the widget
			 * @return focus state
			 */
			virtual bool getFocus() {
				return m_hasFocus;
			};
			/**
			 * @brief get the capability to have focus
			 * @return State capability to have focus
			 */
			virtual bool canHaveFocus() {
				return m_canFocus;
			};
			/**
			 * @brief set focus on this widget
			 * @return return true if the widget keep the focus
			 */
			virtual bool setFocus();
			/**
			 * @brief remove the focus on this widget
			 * @return return true if the widget have release his focus (if he has it)
			 */
			virtual bool rmFocus();
			/**
			 * @brief set the capability to have the focus
			 * @param[in] _canFocusState new focus capability
			 */
			virtual void setCanHaveFocus(bool _canFocusState) {
				m_canFocus.set(_canFocusState);
			}
			/**
			 * @brief keep the focus on this widget  == > this remove the previous focus on all other widget
			 */
			virtual void keepFocus();
		protected:
			/**
			 * @brief Event of the focus has been grep by the current widget
			 */
			virtual void onGetFocus() {};
			/**
			 * @brief Event of the focus has been lost by the current widget
			 */
			virtual void onLostFocus() {};
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Mouse event properties Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			int32_t m_limitMouseEvent; //!< this is to limit the number of mouse event that the widget can supported
		public:
			/**
			 * @brief get the number of mouse event supported
			 * @return return the number of event that the mouse supported [0..3]
			 */
			virtual int32_t getMouseLimit() {
				return m_limitMouseEvent;
			};
			/**
			 * @brief get the number of mouse event supported
			 * @param[in] _numberState The number of event that the mouse supported [0..3]
			 */
			virtual void setMouseLimit(int32_t _numberState) {
				m_limitMouseEvent = _numberState;
			};
		// ----------------------------------------------------------------------------------------------------------------
		// -- keyboard event properties Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool m_allowRepeateKeyboardEvent; //!< This remove the repeating keybord event due to the constant pressing key.
		public:
			/**
			 * @brief get the keyboard repeating event supporting.
			 * @return true : the event can be repeated.
			 * @return false : the event must not be repeated.
			 */
			virtual bool getKeyboardRepeate() {
				return m_allowRepeateKeyboardEvent;
			};
		protected:
			/**
			 * @brief set the keyboard repeating event supporting.
			 * @param[in] _state The repeating status (true: enable, false disable).
			 */
			virtual void setKeyboardRepeate(bool _state) {
				m_allowRepeateKeyboardEvent = _state;
			};
			/**
			 * @brief display the virtual keyboard (if needed)
			 */
			virtual void showKeyboard();
			/**
			 * @brief Hide the virtual keyboard (if needed)
			 */
			virtual void hideKeyboard();
		// ----------------------------------------------------------------------------------------------------------------
		// -- periodic call Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			int64_t m_periodicCallDeltaTime; //!< -1 : disable / 0 : every time / else in US
			int64_t m_periodicCallTime; //!< Last call time
		protected:
			/**
			 * @brief disable the periodic call.
			 */
			void periodicCallDisable();
			/**
			 * @brief disable the periodic call.
			 * @param[in] _callInSecond periodic call in second (float)
			 */
			void periodicCallEnable(float _callInSecond=0);
		public:
			/**
			 * @brief {SYSTEM} get a reference of the periodic call delta time
			 * @return the perodic time delta call -1 : disable / 0 : every time / else in US
			 */
			int64_t systemGetCallDeltaTime() const {
				return m_periodicCallDeltaTime;
			};
			/**
			 * @brief {SYSTEM} get a reference of the periodic call time
			 * @return Last call from the periodic call
			 */
			int64_t systemGetLastCallTime() const {
				return m_periodicCallTime;
			};
			/**
			 * @brief {SYSTEM} get a reference of the periodic call time
			 * @return Last call from the periodic call
			 */
			void systemSetLastCallTime(int64_t _time) {
				m_periodicCallTime=_time;
			};
			/**
			 * @brief periodic call of this widget
			 * @param _event Current time property
			 */
			virtual void periodicCall(const ewol::event::Time& _event) {
				
			};
		public:
			/**
			 * @brief get the widget at the specific windows absolute position
			 * @param[in] _pos gAbsolute position of the requested widget knowledge
			 * @return nullptr No widget found
			 * @return pointer on the widget found
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos) {
				if (false == isHide()) {
					return std::dynamic_pointer_cast<ewol::Widget>(shared_from_this());
				}
				return nullptr;
			};
		
		// event section:
		public:
			/**
			 * @brief {SYSTEM} system event input (only meta widget might overwrite this function).
			 * @param[in] _event Event properties
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool systemEventInput(ewol::event::InputSystem& _event);
		protected:
			/**
			 * @brief Event on an input of this Widget (finger, mouse, stilet)
			 * @param[in] _event Event properties
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool onEventInput(const ewol::event::Input& _event) {
				return false;
			};
		public:
			/**
			 * @brief {SYSTEM} Entry event (only meta widget might overwrite this function).
			 * @param[in] _event Event properties
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool systemEventEntry(ewol::event::EntrySystem& _event);
		protected:
			/**
			 * @brief Entry event.
			 *        represent the physical event :
			 *            - Keyboard (key event and move event)
			 *            - Accelerometer
			 *            - Joystick
			 * @param[in] _event Event properties
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool onEventEntry(const ewol::event::Entry& _event) {
				return false;
			};
		public:
			/**
			 * @brief Event on a past event  == > this event is asynchronous due to all system does not support direct getting datas
			 * @note : need to have focus ...
			 * @param[in] mode Mode of data requested
			 */
			virtual void onEventClipboard(enum ewol::context::clipBoard::clipboardListe _clipboardID) { };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Shortcut : management of the shortcut
		// ----------------------------------------------------------------------------------------------------------------
		public:
			ewol::object::Signal<std::string> signalShortcut; //!< signal handle of the message
		private:
			std::vector<EventShortCut*> m_localShortcut; //!< list of all shortcut in the widget
		protected:
			/**
			 * @brief add a specific shortcut with his description
			 * @param[in] _descriptiveString Description string of the shortcut
			 * @param[in] _message massage to generate (or shortcut name)
			 */
			virtual void shortCutAdd(const std::string& _descriptiveString,
			                         const std::string& _message="");
			/**
			 * @brief remove all curent shortCut
			 */
			virtual void shortCutClean();
		public:
			/**
			 * @brief Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref onEventKb).
			 * @param[in] _special All the special kay pressed at this time.
			 * @param[in] _unicodeValue Key pressed by the user not used if the kbMove!=ewol::EVENT_KB_MOVE_TYPE_NONE.
			 * @param[in] _kbMove Special key of the keyboard.
			 * @return true if the event has been used.
			 * @return false if the event has not been used.
			 * @note To prevent some error when you get an event get it if it is down and Up ...  == > like this it could not generate some ununderstanding error.
			 */
			virtual bool onEventShortCut(ewol::key::Special& _special,
			                             char32_t _unicodeValue,
			                             enum ewol::key::keyboard _kbMove,
			                             bool _isDown);
		// ----------------------------------------------------------------------------------------------------------------
		// -- drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			bool m_needRegenerateDisplay; //!< the display might be done the next regeneration
			/**
			 * @brief The widget mark itself that it need to regenerate the nest time.
			 */
			virtual void markToRedraw();
			/**
			 * @brief get the need of the redrawing of the widget and reset it to false
			 * @return true if we need to redraw
			 * @return false if we have no need to redraw
			 */
			virtual bool needRedraw() {
				bool tmpData = m_needRegenerateDisplay;
				m_needRegenerateDisplay = false;
				return tmpData;
			};
		public:
			/**
			 * @brief {SYSTEM} extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated openGl widget
			 * @param[in] _displayProp properties of the current display
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void systemDraw(const DrawProperty& _displayProp);
		protected:
			/**
			 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
			 */
			virtual void onDraw() { };
		public:
			/**
			 * @brief Event generated when a redraw is needed
			 */
			virtual void onRegenerateDisplay() { };
		// grab cursor mode
		private:
			bool m_grabCursor;
		public:
			/**
			 * @brief Grab the cursor : This get all the mouvement of the mouse in PC mode, and generate an ofset instead of a position.
			 * @note : the generation of the offset is due to the fact the cursor position is forced at the center of the widget.
			 * @note This done nothing in "Finger" or "Stylet" mode.
			 */
			virtual void grabCursor();
			/**
			 * @brief Un-Grab the cursor (default mode cursor offset)
			 */
			virtual void unGrabCursor();
			/**
			 * @brief get the grabbing status of the cursor.
			 * @return true if the cursor is curently grabbed
			 */
			virtual bool getGrabStatus();
		private:
			enum ewol::context::cursorDisplay m_cursorDisplay;
		public:
			/**
			 * @brief set the cursor display type.
			 * @param[in] _newCursor selected new cursor.
			 */
			virtual void setCursor(enum ewol::context::cursorDisplay _newCursor);
			/**
			 * @brief get the currrent cursor.
			 * @return the type of the cursor.
			 */
			virtual enum ewol::context::cursorDisplay getCursor();
		public: // Derived function
			virtual bool loadXML(exml::Element* _node);
		protected: // Derived function
			virtual void onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer);
		public:
			/**
			 * @brief need to be call When the size of the current widget have change  == > this force the system to recalculate all the widget positions
			 */
			void requestUpdateSize();
			/**
			 * @brief get the current Widget Manager
			 */
			ewol::widget::Manager& getWidgetManager();
			/**
			 * @brief get the curent Windows
			 */
			std::shared_ptr<ewol::widget::Windows> getWindows();
		/*
		 * Annimation section :
		 */
		public:
			// event generated :
			ewol::object::Signal<void> signalAnnimationStart; //!< event when start annimation
			ewol::object::Signal<float> signalAnnimationRatio; //!< event when % of annimation change (integer)
			ewol::object::Signal<void> signalAnnimationStop;  //!< event when stop annimation
		protected:
			enum annimationMode {
				annimationModeEnableAdd,
				annimationModeEnableRemove,
				annimationModeDisable
			};
			enum annimationMode m_annimationMode; //!< true when the annimation is started
			float m_annimationratio; //!< Ratio of the annimation [0..1]
		protected:
			ewol::object::ParamList<int32_t> m_annimationTypeStart; //!< type of start annimation
			ewol::object::ParamRange<float> m_annimationTimeStart; //!< time to produce start annimation
			ewol::object::ParamList<int32_t> m_annimationTypeStop; //!< type of start annimation
			ewol::object::ParamRange<float> m_annimationTimeStop; //!< time to produce start annimation
		protected:
			/**
			 * @brief Add a annimation type capabilities of this widget.
			 * @param[in] _mode Configuring mode.
			 * @param[in] _type Type of the annimation.
			 */
			void addAnnimationType(enum ewol::Widget::annimationMode _mode, const char* _type);
		public:
			/**
			 * @brief set a annimation type.
			 * @param[in] _mode Configuring mode.
			 * @param[in] _type type of the annimation
			 */
			void setAnnimationType(enum ewol::Widget::annimationMode _mode, const std::string& _type);
			/**
			 * @brief set a annimation time to produce.
			 * @param[in] _mode Configuring mode.
			 * @param[in] _time Time in second of the annimation display
			 */
			void setAnnimationTime(enum ewol::Widget::annimationMode _mode, float _time);
			/**
			 * @brief Start the annimation.
			 * @param[in] _mode Configuring mode.
			 * @return true if an annimation will be started, false ==> no annimation and no event
			 */
			bool startAnnimation(enum ewol::Widget::annimationMode _mode);
			/**
			 * @brief Stop/Break the annimation.
			 * @return true if an annimation will be stoped, false ==> no curent annimation and no event wil be generated
			 */
			bool stopAnnimation();
		protected:
			/**
			 * @brief Event when start the annimation.
			 * @param[in] _mode Configuring mode.
			 * @return true need to add periodic call.
			 */
			virtual bool onStartAnnimation(enum ewol::Widget::annimationMode _mode) {
				return false;
			};
			/**
			 * @brief Event when Stop the annimation.
			 */
			virtual void onStopAnnimation() { };
			
	};
};

#include <ewol/widget/Manager.h>

#endif
