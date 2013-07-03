/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_H__
#define __EWOL_WIDGET_H__

#include <ewol/eObject/EObject.h>
#include <ewol/Dimension.h>

namespace ewol {
	class Widget;
};
#include <etk/types.h>
#include <etk/Vector.h>
#include <etk/math/Vector2D.h>
#include <ewol/debug.h>
#include <ewol/clipBoard.h>
#include <ewol/key.h>
#include <ewol/cursor.h>
#include <ewol/renderer/EventInput.h>
#include <ewol/renderer/EventEntry.h>
#include <ewol/renderer/EventTime.h>

#define ULTIMATE_MAX_SIZE  (99999999)

namespace ewol {
	
	class DrawProperty{
		/*
		                                                          /--> m_windowsSize
		      *--------------------------------------------------*
		      |                                                  |
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
			void Limit(const vec2& _origin, const vec2& _size);
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::DrawProperty& _obj);
	
	typedef enum {
		gravityCenter=0x00,
		gravityTopLeft=0x05,
		gravityTop=0x01,
		gravityTopRight=0x03,
		gravityRight=0x02,
		gravityButtomRight=0x06,
		gravityButtom=0x04,
		gravityButtomLeft=0x0C,
		gravityLeft=0x08,
	}gravity_te;
	etk::CCout& operator <<(etk::CCout& _os, const ewol::gravity_te _obj);
	etk::UString GravityToString(const ewol::gravity_te _obj);
	ewol::gravity_te StringToGravity(const etk::UString& _obj);
	
	class EventShortCut {
		public:
			bool broadcastEvent; //!< if it is true, then the message is sent to all the system
			const char* generateEventId; //!< Local generated event
			etk::UString eventData; //!< data link with the event
			ewol::SpecialKey specialKey; //!< special board key
			uniChar_t unicodeValue; //!< 0 if not used
			ewol::keyEvent::keyboard_te keyboardMoveValue; //!< ewol::EVENT_KB_MOVE_TYPE_NONE if not used
			EventShortCut(void) {
				broadcastEvent = false;
				generateEventId = NULL;
				eventData = "";
				unicodeValue = 0;
				keyboardMoveValue = ewol::keyEvent::keyboardUnknow;
			};
			~EventShortCut(void) { };
	};
	class Widget : public EObject {
		public:
			// Config list of properties
			static const char* const configFill;
			static const char* const configExpand;
			static const char* const configHide;
			static const char* const configFocus;
			static const char* const configMinSize;
			static const char* const configMaxSize;
			static const char* const configGravity;
		public:
			/**
			 * @brief Constructor of the widget classes
			 * @return (no execption generated (not managed in embended platform))
			 */
			Widget(void);
			/**
			 * @brief Destructor of the widget classes
			 */
			virtual ~Widget(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "Ewol::Widget"; };
		// ----------------------------------------------------------------------------------------------------------------
		// -- Hierarchy management:
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			ewol::Widget* m_up; //!< uppper widget in the tree of widget
		public:
			/**
			 * @brief Set the upper widget of this widget.
			 * @param[in] _upper Father widget (only keep the last and write error if a previous was set) ==> disable with NULL.
			 */
			void SetUpperWidget(ewol::Widget* _upper);
			/**
			 * @brief Remove the upper widget of this widget.
			 */
			void RemoveUpperWidget(void) { SetUpperWidget(NULL); };
			/**
			 * @brief Get the upper widget (father).
			 * @ return the requested widget (if NULL , 2 case : root widget or error implementation).
			 */
			ewol::Widget* GetUpperWidget(void) { return m_up; };
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget Size:
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			vec2 m_size; //!< internal : current size of the widget
			vec2 m_minSize; //!< internal : minimum size of the widget
			vec2 m_maxSize; //!< internal : maximum size of the widget
		public:
			/**
			 * @brief Convert the absolute position in the local Position (Relative)
			 * @param[in] _pos Absolute position that you request convertion
			 * @return the relative position
			 */
			virtual vec2 RelativePosition(const vec2& _pos);
			/**
			 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
			 *        By default this save the widget available size in the widget size
			 * @param[in] _available Available x&y pixel size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void CalculateSize(const vec2& _available);
			/**
			 * @brief Get the widget size
			 * @return Requested size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual vec2 GetSize(void);
			/**
			 * @brief Calculate the minimum and maximum size (need to estimate expend properties of the widget)
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void CalculateMinMaxSize(void);
			/**
			 * @brief Get the widget minimum size calculated
			 * @return Requested size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual vec2 GetCalculateMinSize(void);
			/**
			 * @brief Get the widget maximum size calculated
			 * @return Requested size
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual vec2 GetCalculateMaxSize(void);
		protected:
			vec2 m_offset; //!< Offset of the display in the viewport
		public:
			/**
			 * @brief Set the zoom property of the widget.
			 * @param[in] _newVal offset value.
			 */
			virtual void SetOffset(const vec2& _newVal);
			/**
			 * @brief Get the offset property of the widget.
			 * @return The current offset value.
			 */
			virtual const vec2& GetOffset(void) { return m_offset; };
		protected:
			// internal element calculated by the system
			float m_zoom; //!< generic widget zoom
		public:
			/**
			 * @brief Set the zoom property of the widget
			 * @param[in] _newVal newZoom value
			 */
			virtual void SetZoom(float _newVal);
			/**
			 * @brief Get the zoom property of the widget
			 * @return the current zoom value
			 */
			virtual float GetZoom(void);
		protected:
			vec2 m_origin; //!< internal ... I do not really known how i can use it ...
		public:
			/**
			 * @brief Set origin at the widget (must be an parrent widget that set this parameter).
			 * This represent the absolute origin in the program windows
			 * @param[in] _pos Position of the origin
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void SetOrigin(const vec2& _pos);
			/**
			 * @brief Get the origin (obsolute position in the windows)
			 * @return coordonate of the origin requested
			 */
			virtual vec2 GetOrigin(void);
		protected:
			ewol::Dimension m_userMinSize; //!< user define the minimum size of the widget
		public:
			/**
			 * @brief User set the minimum size he want to set the display
			 * @param[in] _size Set minimum size (none : 0)
			 */
			void SetMinSize(const ewol::Dimension& _size);
			/**
			 * @brief User set No minimum size.
			 */
			void SetNoMinSize(void);
			/**
			 * @brief Get the current calculated min size
			 * @return the size requested
			 */
			const ewol::Dimension& GetMinSize(void) { return m_userMinSize; };
			/**
			 * @brief Check if the current min size is compatible with the user minimum size
			 *        If it is not the user minimum size will overWrite the minimum size set.
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void CheckMinSize(void);
		protected:
			ewol::Dimension m_userMaxSize; //!< user define the maximum size of the widget
		public:
			/**
			 * @brief User set the maximum size he want to set the display
			 * @param[in] _size The new maximum size requested (vec2(0,0) to unset)
			 */
			void SetMaxSize(const ewol::Dimension& _size);
			/**
			 * @brief User set No maximum size.
			 */
			void SetNoMaxSize(void);
			/**
			 * @brief Get the current maximum size
			 * @return the size requested
			 */
			const ewol::Dimension& GetMaxSize(void) { return m_userMaxSize; };
			/**
			 * @brief Check if the current max size is compatible with the user maximum size
			 *        If it is not the user maximum size will overWrite the maximum size set.
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void CheckMaxSize(void);
		protected:
			bvec2 m_userExpand;
		public:
			/**
			 * @brief Set the expend capabilities (x&y)
			 * @param[in] _newExpend 2D boolean repensent the capacity to expend
			 */
			virtual void SetExpand(const bvec2& _newExpand);
			/**
			 * @brief Get the expend capabilities (x&y) (set by the user)
			 * @return 2D boolean repensent the capacity to expend
			 */
			virtual bvec2 GetExpand(void) { return m_userExpand; };
			/**
			 * @brief Get the expend capabilities (x&y)
			 * @return 2D boolean repensent the capacity to expend
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual bvec2 CanExpand(void);
		protected:
			bvec2 m_userFill;
		public:
			/**
			 * @brief Set the x&y filling capacity
			 * @param[in] _newFill new x&y fill state
			 */
			virtual void SetFill(const bvec2& _newFill);
			/**
			 * @brief Set the x&y filling capacity set by the user
			 * @return bvec2 repensent the capacity to x&y filling (set by the user)
			 */
			virtual const bvec2& GetFill(void) { return m_userFill; };
			/**
			 * @brief Get the filling capabilities x&y
			 * @return bvec2 repensent the capacity to x&y filling
			 * @note : INTERNAL EWOL SYSTEM
			 */
			const bvec2& CanFill(void);
		protected:
			bool m_hide; //!< hide a widget on the display
		public:
			/**
			 * @brief Set the widget hidden
			 */
			virtual void Hide(void);
			/**
			 * @brief Set the widget visible
			 */
			virtual void Show(void);
			/**
			 * @brief Get the visibility of the widget
			 * @return true: if the widget is hiden, false: it is visible
			 */
			virtual bool IsHide(void) { return m_hide; };
		
		protected:
			gravity_te m_gravity; //!< Gravity of the widget
		public:
			/**
			 * @brief Set the widget gravity
			 * @param[in] _gravity New gravity of the widget
			 */
			virtual void SetGravity(gravity_te _gravity);
			/**
			 * @brief Get the widget gravity
			 * @return the gravity type
			 */
			virtual gravity_te GetGravity(void) { return m_gravity; };
		// ----------------------------------------------------------------------------------------------------------------
		// -- Focus Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool m_hasFocus; //!< set the focus on this widget
			bool m_canFocus; //!< the focus can be done on this widget
		public:
			/**
			 * @brief Get the focus state of the widget
			 * @return Focus state
			 */
			virtual bool GetFocus(void) { return m_hasFocus;};
			/**
			 * @brief Get the capability to have focus
			 * @return State capability to have focus
			 */
			virtual bool CanHaveFocus(void) { return m_canFocus;};
			/**
			 * @brief Set focus on this widget
			 * @return return true if the widget keep the focus
			 */
			virtual bool SetFocus(void);
			/**
			 * @brief Remove the focus on this widget
			 * @return return true if the widget have release his focus (if he has it)
			 */
			virtual bool RmFocus(void);
			/**
			 * @brief Set the capability to have the focus
			 * @param[in] _canFocusState new focus capability
			 */
			virtual void SetCanHaveFocus(bool _canFocusState);
			/**
			 * @brief Keep the focus on this widget ==> this remove the previous focus on all other widget
			 */
			virtual void KeepFocus(void);
		protected:
			/**
			 * @brief Event of the focus has been grep by the current widget
			 */
			virtual void OnGetFocus(void) {};
			/**
			 * @brief Event of the focus has been lost by the current widget
			 */
			virtual void OnLostFocus(void) {};
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Mouse event properties Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			int32_t m_limitMouseEvent; //!< this is to limit the number of mouse event that the widget can supported
		public:
			/**
			 * @brief Get the number of mouse event supported
			 * @return return the number of event that the mouse supported [0..3]
			 */
			virtual int32_t GetMouseLimit(void) { return m_limitMouseEvent; };
			/**
			 * @brief Get the number of mouse event supported
			 * @param[in] _numberState The number of event that the mouse supported [0..3]
			 */
			virtual void SetMouseLimit(int32_t _numberState) { m_limitMouseEvent = _numberState; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- keyboard event properties Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool m_allowRepeateKeyboardEvent; //!< This remove the repeating keybord event due to the constant pressing key.
		public:
			/**
			 * @brief Get the keyboard repeating event supporting.
			 * @return true : the event can be repeated.
			 * @return false : the event must not be repeated.
			 */
			virtual bool GetKeyboardRepeate(void) { return m_allowRepeateKeyboardEvent; };
			/**
			 * @brief Set the keyboard repeating event supporting.
			 * @param[in] _state The repeating status (true: enable, false disable).
			 */
			virtual void SetKeyboardRepeate(bool _state) { m_allowRepeateKeyboardEvent = _state; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Periodic call Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			int64_t m_periodicCallDeltaTime; //!< -1 : Disable / 0 : every time / else in US
			int64_t m_periodicCallTime; //!< Last call time
		protected:
			/**
			 * @brief Disable the periodic call.
			 */
			void PeriodicCallDisable(void);
			/**
			 * @brief Disable the periodic call.
			 * @param[in] _callInSecond periodic call in second (float)
			 */
			void PeriodicCallEnable(float _callInSecond=0);
		public:
			/**
			 * @brief {SYSTEM} Get a reference of the periodic call delta time
			 * @return the perodic time delta call -1 : Disable / 0 : every time / else in US
			 */
			int64_t SystemGetCallDeltaTime(void) const { return m_periodicCallDeltaTime; };
			/**
			 * @brief {SYSTEM} Get a reference of the periodic call time
			 * @return Last call from the periodic call
			 */
			int64_t SystemGetLastCallTime(void) const { return m_periodicCallTime; };
			/**
			 * @brief {SYSTEM} Get a reference of the periodic call time
			 * @return Last call from the periodic call
			 */
			void SystemSetLastCallTime(int64_t _time) { m_periodicCallTime=_time; };
			/**
			 * @brief Periodic call of this widget
			 * @param _event Current time property
			 */
			virtual void PeriodicCall(const ewol::EventTime& _event) { };
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] _pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual ewol::Widget* GetWidgetAtPos(const vec2& _pos) { if (false==IsHide()) { return this; } return NULL; };
			/**
			 * @brief Get the widget if it have this name or one of the subwidget with the same name
			 * @param[in] _widgetName name of the widget
			 * @return the requested pointer on the node (or NULL pointer)
			 */
			virtual ewol::Widget* GetWidgetNamed(const etk::UString& _widgetName);
		
		// event section:
		public:
			/**
			 * @brief {SYSTEM} system event input (only meta widget might overwrite this function).
			 * @param[in] _event Event properties
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool SystemEventInput(ewol::EventInputSystem& _event);
		protected:
			/**
			 * @brief Event on an input of this Widget (finger, mouse, stilet)
			 * @param[in] _event Event properties
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(const ewol::EventInput& _event) { return false; };
		public:
			/**
			 * @brief {SYSTEM} Entry event (only meta widget might overwrite this function).
			 * @param[in] _event Event properties
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool SystemEventEntry(ewol::EventEntrySystem& _event);
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
			virtual bool OnEventEntry(const ewol::EventEntry& _event) { return false; };
		public:
			/**
			 * @brief Event on a past event ==> this event is asynchronous due to all system does not support direct getting datas
			 * @note : need to have focus ...
			 * @param[in] mode Mode of data requested
			 */
			virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID) { };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Shortcut : management of the shortcut
		// ----------------------------------------------------------------------------------------------------------------
		private:
			etk::Vector<EventShortCut*> m_localShortcut; //!< list of all shortcut in the widget
		protected:
			/**
			 * @brief Add a specific shortcut with his description
			 * @param[in] _descriptiveString Description string of the shortcut
			 * @param[in] _generateEventId Event generic of the element
			 * @param[in] _data Associate data wit the event
			 */
			virtual void ShortCutAdd(const char * _descriptiveString, const char * _generateEventId, etk::UString _data="", bool _broadcast=false);
			/**
			 * @brief Remove all curent shortCut
			 */
			virtual void ShortCutClean(void);
		public:
			/**
			 * @brief Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref OnEventKb)
			 * @param[in] _special all the special kay pressed at this time
			 * @param[in] _unicodeValue key pressed by the user not used if the kbMove!=ewol::EVENT_KB_MOVE_TYPE_NONE
			 * @param[in] _kbMove special key of the keyboard
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 * @note To prevent some error when you get an event get it if it is down and Up ... ==> like this it could not generate some ununderstanding error
			 */
			virtual bool OnEventShortCut(ewol::SpecialKey& _special, uniChar_t _unicodeValue, ewol::keyEvent::keyboard_te _kbMove, bool _isDown);
		// ----------------------------------------------------------------------------------------------------------------
		// -- Drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			bool m_needRegenerateDisplay; //!< the display might be done the next regeneration
			/**
			 * @brief The widget mark itself that it need to regenerate the nest time.
			 */
			virtual void MarkToRedraw(void);
			/**
			 * @brief Get the need of the redrawing of the widget and reset it to false
			 * @return true if we need to redraw
			 * @return false if we have no need to redraw
			 */
			virtual bool NeedRedraw(void) { bool tmpData=m_needRegenerateDisplay; m_needRegenerateDisplay=false; return tmpData; };
		public:
			/**
			 * @brief {SYSTEM} extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
			 * @param[in] _displayProp properties of the current display
			 * @note : INTERNAL EWOL SYSTEM
			 */
			virtual void SystemDraw(const DrawProperty& _displayProp);
		protected:
			/**
			 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
			 */
			virtual void OnDraw(void) { };
		public:
			/**
			 * @brief Event generated when a redraw is needed
			 */
			virtual void OnRegenerateDisplay(void) { };
		// grab cursor mode
		private:
			bool m_grabCursor;
		public:
			/**
			 * @brief Grab the cursor : This get all the mouvement of the mouse in PC mode, and generate an ofset instead of a position.
			 * @note : the generation of the offset is due to the fact the cursor position is forced at the center of the widget.
			 * @note This done nothing in "Finger" or "Stylet" mode.
			 */
			virtual void GrabCursor(void);
			/**
			 * @brief Un-Grab the cursor (default mode cursor offset)
			 */
			virtual void UnGrabCursor(void);
			/**
			 * @brief Get the grabbing status of the cursor.
			 * @return true if the cursor is curently grabbed
			 */
			virtual bool GetGrabStatus(void);
		private:
			ewol::cursorDisplay_te m_cursorDisplay;
		public:
			/**
			 * @brief Set the cursor display type.
			 * @param[in] _newCursor selected new cursor.
			 */
			virtual void SetCursor(ewol::cursorDisplay_te _newCursor);
			/**
			 * @brief Get the currrent cursor.
			 * @return the type of the cursor.
			 */
			virtual ewol::cursorDisplay_te GetCursor(void);
		public: // Derived function
			virtual void OnObjectRemove(ewol::EObject* _removeObject);
			virtual bool LoadXML(exml::Element* _node);
		protected: // Derived function
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
	}; // end of the class Widget declaration

};// end of namespace


#endif
