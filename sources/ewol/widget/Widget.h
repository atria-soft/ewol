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

namespace ewol {
	
	class DrawProperty{
		public :
			ivec2 m_windowsSize;
			ivec2 m_origin;
			ivec2 m_size;
	};
	
	
	class EventShortCut {
		public:
			bool                        broadcastEvent;    // if it is true, then the message is sent to all the system
			const char *                generateEventId;   // Local generated event
			etk::UString                eventData;         // data link with the event
			ewol::SpecialKey            specialKey;        // special board key
			uniChar_t                   unicodeValue;      // 0 if not used
			ewol::keyEvent::keyboard_te keyboardMoveValue; // ewol::EVENT_KB_MOVE_TYPE_NONE if not used
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
			virtual const char * const GetObjectType(void) { return "EwolWidget"; };
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget Size:
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool   m_hide; //!< hide a widget on the display
		protected:
			// internal element calculated by the system
			float  m_zoom;          //!< generic widget zoom
			vec2   m_origin;        //!< internal ... I do not really known how i can use it ...
			vec2   m_size;          //!< internal : current size of the widget
			vec2   m_minSize;       //!< user define the minimum size of the widget
			// user configuaration
			vec2   m_userMinSize;   //!< user define the minimum size of the widget
			vec2   m_userMaxSize;   //!< user define the maximum size of the widget
			bvec2  m_userExpend;
			bvec2  m_userFill;
		public:
			/**
			 * @brief Set the zoom property of the widget
			 * @param[in] newVal newZoom value
			 */
			void SetZoom(float newVal);
			/**
			 * @brief Get the zoom property of the widget
			 * @return the current zoom value
			 */
			float GetZoom(void);
			/**
			 * @brief Set origin at the widget (must be an parrent widget that set this parameter).
			 * This represent the absolute origin in the program windows
			 * @param[in] x Position ot hte horizantal origin
			 * @param[in] y Position ot hte vertical origin
			 */
			void SetOrigin(float x, float y);
			/**
			 * @brief Get the origin (obsolute position in the windows)
			 * @return coordonate of the origin requested
			 */
			vec2 GetOrigin(void);
			/**
			 * @brief Convert the absolute position in the local Position (Relative)
			 * @param[in] pos Absolute position that you request convertion
			 * @return the relative position
			 */
			virtual vec2 RelativePosition(vec2 pos);
			/**
			 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
			 *        By default this save the widget availlable size in the widget size
			 * @param[in] availlableX Availlable horisantal pixel size
			 * @param[in] availlableY Availlable vertical pixel size
			 */
			// TODO : Remove bool ==> deprecated ...
			// TODO : Rename in SetSize()
			virtual bool CalculateSize(float availlableX, float availlableY);
			//update the min Size ... and the expend parameters for the sizer
			/**
			 * @brief Calculate the minimum size of the widget that is needed to display or the user requested)
			 */
			// TODO : Remove bool ==> deprecated ...
			virtual bool CalculateMinSize(void);
			/**
			 * @brief User set the minimum size he want to set the display
			 * @param[in] x Set minimum horizontal size (-1 : no requested)
			 * @param[in] y Set minimum vertical size (-1 : no requested)
			 */
			virtual void SetMinSize(float x=-1, float y=-1);
			/**
			 * @brief Get the current calculated min size
			 * @return the size requested
			 */
			vec2 GetMinSize(void);
			/**
			 * @brief User set the maximum size he want to set the display
			 * @param[in] size The new maximum size requested (vec2(-1,-1) to unset)
			 */
			virtual void SetMaxSize(vec2 size);
			/**
			 * @brief Get the current maximum size
			 * @return the size requested
			 */
			vec2 GetMaxSize(void);
			/**
			 * @brief Get the widget size
			 * @return Requested size
			 */
			vec2 GetSize(void);
			/**
			 * @brief Set the horizontal expend capacity
			 * @param[in] newExpend new Expend state
			 */
			virtual void SetExpendX(bool newExpend=false);
			/**
			 * @brief Get the horizontal expend capabilities
			 * @return boolean repensent the capacity to expend
			 */
			virtual bool CanExpentX(void);
			/**
			 * @brief Set the vertical expend capacity
			 * @param[in] newExpend new Expend state
			 */
			virtual void SetExpendY(bool newExpend=false);
			/**
			 * @brief Get the vertical expend capabilities
			 * @return boolean repensent the capacity to expend
			 */
			virtual bool CanExpentY(void);
			/**
			 * @brief Set the horizontal filling capacity
			 * @param[in] newFill new fill state
			 */
			virtual void SetFillX(bool newFill=false);
			/**
			 * @brief Get the horizontal filling capabilities
			 * @return boolean repensent the capacity to horizontal filling
			 */
			bool CanFillX(void);
			/**
			 * @brief Set the vertical filling capacity
			 * @param[in] newFill new fill state
			 */
			virtual void SetFillY(bool newFill=false);
			/**
			 * @brief Get the vertical filling capabilities
			 * @return boolean repensent the capacity to vertical filling
			 */
			bool CanFillY(void);
			/**
			 * @brief Set the widget hidden
			 */
			void Hide(void);
			/**
			 * @brief Set the widget visible
			 */
			void Show(void);
			/**
			 * @brief Get the visibility of the widget
			 * @return true: if the widget is hiden, false: it is visible
			 */
			bool IsHide(void) { return m_hide; };
			
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Focus Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool    m_hasFocus;        //!< set the focus on this widget
			bool    m_canFocus;        //!< the focus can be done on this widget
		public:
			/**
			 * @brief Get the focus state of the widget
			 * @return Focus state
			 */
			bool GetFocus(void) { return m_hasFocus;};
			/**
			 * @brief Get the capability to have focus
			 * @return State capability to have focus
			 */
			bool CanHaveFocus(void) { return m_canFocus;};
			/**
			 * @brief Set focus on this widget
			 * @return return true if the widget keep the focus
			 */
			bool SetFocus(void);
			/**
			 * @brief Remove the focus on this widget
			 * @return return true if the widget have release his focus (if he has it)
			 */
			bool RmFocus(void);
			/**
			 * @brief Set the capability to have the focus
			 * @param[in] canFocusState new focus capability
			 */
			void SetCanHaveFocus(bool canFocusState);
			/**
			 * @brief Keep the focus on this widget ==> this remove the previous focus on all other widget
			 */
			void KeepFocus(void);
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
			int32_t GetMouseLimit(void) { return m_limitMouseEvent; };
			/**
			 * @brief Get the number of mouse event supported
			 * @param[in] numberState The number of event that the mouse supported [0..3]
			 */
			void SetMouseLimit(int32_t numberState) { m_limitMouseEvent = numberState; };
		
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
			bool GetKeyboardRepeate(void) { return m_allowRepeateKeyboardEvent; };
			/**
			 * @brief Set the keyboard repeating event supporting.
			 * @param[in] state The repeating status (true: enable, false disable).
			 */
			void SetKeyboardRepeate(bool state) { m_allowRepeateKeyboardEvent = state; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Periodic call Area
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			/**
			 * @brief Request that the current widegt have a periodic call
			 * @param statusToSet true if the periodic call is needed
			 */
			void PeriodicCallSet(bool statusToSet);
		public:
			/**
			 * @brief Periodic call of this widget
			 * @param localTime curent system time
			 */
			virtual void PeriodicCall(int64_t localTime) { };
			
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(vec2 pos) { if (false==IsHide()) { return this; } return NULL; };
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos) { return false; };
			/**
			 * @brief Event on the keybord (if no shortcut has been detected before).
			 * @param[in] type of the event (ewol::EVENT_KB_TYPE_DOWN or ewol::EVENT_KB_TYPE_UP)
			 * @param[in] unicodeValue key pressed by the user
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData) { return false; };
			/**
			 * @brief Event on the keyboard that is not a printable key (if no shortcut has been detected before).
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventKbMove(ewol::keyEvent::status_te typeEvent, ewol::keyEvent::keyboard_te moveTypeEvent) { return false; };
			/**
			 * @brief Event on a past event ==> this event is asynchronous due to all system does not support direct getting datas
			 * @note : need to have focus ...
			 * @param[in] mode Mode of data requested
			 */
			virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID) { };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Shortcut : management of the shortcut
		// ----------------------------------------------------------------------------------------------------------------
		private:
			etk::Vector<EventShortCut*>    m_localShortcut;
		protected:
			/**
			 * @brief Add a specific shortcut with his description
			 * @param[in] descriptiveString Description string of the shortcut
			 * @param[in] generateEventId Event generic of the element
			 * @param[in] data Associate data wit the event
			 */
			void ShortCutAdd(const char * descriptiveString, const char * generateEventId, etk::UString data="", bool broadcast=false);
			/**
			 * @brief Remove all curent shortCut
			 */
			void ShortCutClean(void);
		public:
			/**
			 * @brief Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref OnEventKb)
			 * @param[in] special all the special kay pressed at this time
			 * @param[in] unicodeValue key pressed by the user not used if the kbMove!=ewol::EVENT_KB_MOVE_TYPE_NONE
			 * @param[in] kbMove special key of the keyboard
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 * @note To prevent some error when you get an event get it if it is down and Up ... ==> like this it could not generate some ununderstanding error
			 */
			virtual bool OnEventShortCut(ewol::SpecialKey& special, uniChar_t unicodeValue, ewol::keyEvent::keyboard_te kbMove, bool isDown);
		// ----------------------------------------------------------------------------------------------------------------
		// -- Drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			bool         m_needRegenerateDisplay; //!< the display might be done the next regeneration
			/**
			 * @brief The widget mark itself that it need to regenerate the nest time.
			 */
			void MarkToRedraw(void);
			/**
			 * @brief Get the need of the redrawing of the widget and reset it to false
			 * @return true if we need to redraw
			 * @return false if we have no need to redraw
			 */
			bool NeedRedraw(void) { bool tmpData=m_needRegenerateDisplay; m_needRegenerateDisplay=false; return tmpData; };
		public:
			/**
			 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
			 * @param[in] displayProp properties of the current display
			 */
			virtual void GenDraw(DrawProperty displayProp);
		protected:
			/**
			 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
			 * @param[in] displayProp properties of the current display
			 */
			virtual void OnDraw(DrawProperty& displayProp) { };
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
			void GrabCursor(void);
			/**
			 * @brief Un-Grab the cursor (default mode cursor offset)
			 */
			void UnGrabCursor(void);
			/**
			 * @brief Get the grabbing status of the cursor.
			 * @return true if the cursor is curently grabbed
			 */
			bool GetGrabStatus(void);
		// DisplayCursorType
		private:
			ewol::cursorDisplay_te m_cursorDisplay;
		public:
			/**
			 * @brief Set the cursor display type.
			 * @param[in] newCursor selected new cursor.
			 */
			void SetCursor(ewol::cursorDisplay_te newCursor);
			/**
			 * @brief Get the currrent cursor.
			 * @return the type of the cursor.
			 */
			ewol::cursorDisplay_te GetCursor(void);
		

	}; // end of the class Widget declaration

};// end of namespace


#endif
