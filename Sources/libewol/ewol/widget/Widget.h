/**
 *******************************************************************************
 * @file ewol/widget/Widget.h
 * @brief basic ewol Widget (header)
 * @author Edouard DUPIN
 * @date 18/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_WIDGET_H__
#define __EWOL_WIDGET_H__

#include <ewol/eObject/EObject.h>

namespace ewol {
	class Widget;
};
#include <etk/Types.h>
#include <etk/Vector.h>
#include <ewol/Debug.h>
#include <ewol/oObject/OObject.h>
#include <ewol/ClipBoard.h>

namespace ewol {
	typedef enum {
		EVENT_INPUT_TYPE_DOWN,
		EVENT_INPUT_TYPE_MOVE,
		EVENT_INPUT_TYPE_SINGLE,
		EVENT_INPUT_TYPE_DOUBLE,
		EVENT_INPUT_TYPE_TRIPLE,
		EVENT_INPUT_TYPE_QUAD,
		EVENT_INPUT_TYPE_QUINTE,
		EVENT_INPUT_TYPE_UP,
		EVENT_INPUT_TYPE_ENTER,
		EVENT_INPUT_TYPE_LEAVE,
		EVENT_INPUT_TYPE_ABORT, // Appeare when an event is tranfert betwwen widgets (the widget which receive this has lost the events)
		EVENT_INPUT_TYPE_TRANSFERT, // Appeare when an event is tranfert betwwen widgets (the widget which receive this has receive the transfert of the event)
	} eventInputType_te;
	
	typedef enum {
		EVENT_KB_TYPE_DOWN,
		EVENT_KB_TYPE_UP,
	} eventKbType_te;
	
	typedef enum {
		EVENT_KB_MOVE_TYPE_LEFT,
		EVENT_KB_MOVE_TYPE_RIGHT,
		EVENT_KB_MOVE_TYPE_UP,
		EVENT_KB_MOVE_TYPE_DOWN,
		EVENT_KB_MOVE_TYPE_PAGE_UP,
		EVENT_KB_MOVE_TYPE_PAGE_DOWN,
		EVENT_KB_MOVE_TYPE_START,
		EVENT_KB_MOVE_TYPE_END,
		EVENT_KB_MOVE_TYPE_CENTER,
		EVENT_KB_MOVE_TYPE_ARRET_DEFIL,
		EVENT_KB_MOVE_TYPE_WAIT,
		EVENT_KB_MOVE_TYPE_INSERT,
		EVENT_KB_MOVE_TYPE_F1,
		EVENT_KB_MOVE_TYPE_F2,
		EVENT_KB_MOVE_TYPE_F3,
		EVENT_KB_MOVE_TYPE_F4,
		EVENT_KB_MOVE_TYPE_F5,
		EVENT_KB_MOVE_TYPE_F6,
		EVENT_KB_MOVE_TYPE_F7,
		EVENT_KB_MOVE_TYPE_F8,
		EVENT_KB_MOVE_TYPE_F9,
		EVENT_KB_MOVE_TYPE_F10,
		EVENT_KB_MOVE_TYPE_F11,
		EVENT_KB_MOVE_TYPE_F12,
		EVENT_KB_MOVE_TYPE_CAPLOCK,
		EVENT_KB_MOVE_TYPE_SHIFT_LEFT,
		EVENT_KB_MOVE_TYPE_SHIFT_RIGHT,
		EVENT_KB_MOVE_TYPE_CTRL_LEFT,
		EVENT_KB_MOVE_TYPE_CTRL_RIGHT,
		EVENT_KB_MOVE_TYPE_META_LEFT,
		EVENT_KB_MOVE_TYPE_META_RIGHT,
		EVENT_KB_MOVE_TYPE_ALT,
		EVENT_KB_MOVE_TYPE_ALT_GR,
		EVENT_KB_MOVE_TYPE_CONTEXT_MENU,
		EVENT_KB_MOVE_TYPE_VER_NUM,
	} eventKbMoveType_te;
	
	typedef enum {
		INPUT_TYPE_UNKNOW,
		INPUT_TYPE_MOUSE,
		INPUT_TYPE_FINGER,
	} inputType_te;
	
	char* GetCharTypeMoveEvent(eventKbMoveType_te type);
	
	class DrawProperty{
		public :
			Vector2D<int32_t> m_windowsSize;
			Vector2D<int32_t> m_origin;
			Vector2D<int32_t> m_size;
	};
	
	
	class Widget : public EObject {
		public:
			/**
			 * @brief Constructor of the widget classes
			 * @param ---
			 * @return (no execption generated (not managed in embended platform))
			 */
			Widget(void);
			/**
			 * @brief Destructor of the widget classes
			 * @param ---
			 * @return ---
			 */
			virtual ~Widget(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolWidget"; };
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget Size:
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool                 m_hide; //!< hide a widget on the display
		protected:
			// internal element calculated by the system
			float                m_zoom;          //!< generic widget zoom
			Vector2D<float>      m_origin;        //!< internal ... I do not really known how i can use it ...
			Vector2D<float>      m_size;          //!< internal : current size of the widget
			Vector2D<float>      m_minSize;       //!< user define the minimum size of the widget
			// user configuaration
			Vector2D<float>      m_userMinSize;   //!< user define the minimum size of the widget
			bool           m_userExpendX;
			bool           m_userExpendY;
			bool           m_userFillX;
			bool           m_userFillY;
		public:
			/**
			 * @brief Set the zoom property of the widget
			 * @param[in] newVal newZoom value
			 * @return ---
			 */
			void SetZoom(float newVal) { m_zoom = newVal; MarkToRedraw(); };
			/**
			 * @brief Get the zoom property of the widget
			 * @param ---
			 * @return the current zoom value
			 */
			float GetZoom(void) { return m_zoom; };
			/**
			 * @brief Set origin at the widget (must be an parrent widget that set this parameter).
			 * This represent the absolute origin in the program windows
			 * @param[in] x Position ot hte horizantal origin
			 * @param[in] y Position ot hte vertical origin
			 * @return ---
			 */
			void SetOrigin(float x, float y) { m_origin.x=x; m_origin.y=y;};
			/**
			 * @brief Get the origin (obsolute position in the windows)
			 * @param ---
			 * @return coordonate of the origin requested
			 */
			Vector2D<float>  GetOrigin(void) { return m_origin; };
			/**
			 * @brief Convert the absolute position in the local Position (Relative)
			 * @param[in] pos Absolute position that you request convertion
			 * @return the relative position
			 */
			virtual Vector2D<float>  RelativePosition(Vector2D<float>  pos) { pos.x -= m_origin.x; pos.y -= m_origin.y; return pos; };
			/**
			 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
			 *        By default this save the widget availlable size in the widget size
			 * @param[in] availlableX Availlable horisantal pixel size
			 * @param[in] availlableY Availlable vertical pixel size
			 * @return ---
			 */
			// TODO : Remove bool ==> deprecated ...
			// TODO : Rename in SetSize()
			virtual bool CalculateSize(float availlableX, float availlableY);
			//update the min Size ... and the expend parameters for the sizer
			/**
			 * @brief Calculate the minimum size of the widget that is needed to display or the user requested)
			 * @param ---
			 * @return ---
			 */
			// TODO : Remove bool ==> deprecated ...
			virtual bool CalculateMinSize(void) {m_minSize.x = m_userMinSize.x; m_minSize.y = m_userMinSize.y; MarkToRedraw(); return true; };
			/**
			 * @brief User set the minimum size he want to set the display
			 * @param[in] x Set minimum horizontal size (-1 : no requested)
			 * @param[in] y Set minimum vertical size (-1 : no requested)
			 * @return ---
			 */
			virtual void SetMinSize(float x=-1, float y=-1) { m_userMinSize.x = x; m_userMinSize.y = y; };
			/**
			 * @brief Get the current calculated min size
			 * @param ---
			 * @return re size requested
			 */
			Vector2D<float>  GetMinSize(void) { if (false==IsHide()) { return m_minSize; } return Vector2D<float>(0,0); };
			/**
			 * @brief Get the widget size
			 * @param ---
			 * @return Requested size
			 */
			Vector2D<float>  GetSize(void) { if (false==IsHide()) { return m_size; } return Vector2D<float>(0,0); };
			/**
			 * @brief Set the horizontal expend capacity
			 * @param[in] newExpend new Expend state
			 * @return ---
			 */
			virtual void SetExpendX(bool newExpend=false) { m_userExpendX = newExpend; };
			/**
			 * @brief Get the horizontal expend capabilities
			 * @param ---
			 * @return boolean repensent the capacity to expend
			 */
			virtual bool CanExpentX(void) {  if (false==IsHide()) { return m_userExpendX; } return false; };
			/**
			 * @brief Set the vertical expend capacity
			 * @param[in] newExpend new Expend state
			 * @return ---
			 */
			virtual void SetExpendY(bool newExpend=false) { m_userExpendY = newExpend; };
			/**
			 * @brief Get the vertical expend capabilities
			 * @param ---
			 * @return boolean repensent the capacity to expend
			 */
			virtual bool CanExpentY(void) {  if (false==IsHide()) { return m_userExpendY; } return false; };
			/**
			 * @brief Set the horizontal filling capacity
			 * @param[in] newFill new fill state
			 * @return ---
			 */
			virtual void SetFillX(bool newFill=false) { m_userFillX = newFill; };
			/**
			 * @brief Get the horizontal filling capabilities
			 * @param ---
			 * @return boolean repensent the capacity to horizontal filling
			 */
			bool CanFillX(void) { return m_userFillX; };
			/**
			 * @brief Set the vertical filling capacity
			 * @param[in] newFill new fill state
			 * @return ---
			 */
			virtual void SetFillY(bool newFill=false) { m_userFillY = newFill; };
			/**
			 * @brief Get the vertical filling capabilities
			 * @param ---
			 * @return boolean repensent the capacity to vertical filling
			 */
			bool CanFillY(void) { return m_userFillY; };
			/**
			 * @brief Set the widget hidden
			 * @param ---
			 * @return ---
			 */
			void Hide(void);
			/**
			 * @brief Set the widget visible
			 * @param ---
			 * @return ---
			 */
			void Show(void);
			/**
			 * @brief Get the visibility of the widget
			 * @param ---
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
			 * @param ---
			 * @return Focus state
			 */
			bool GetFocus(void) { return m_hasFocus;};
			/**
			 * @brief Get the capability to have focus
			 * @param ---
			 * @return State capability to have focus
			 */
			bool CanHaveFocus(void) { return m_canFocus;};
			/**
			 * @brief Set focus on this widget
			 * @param ---
			 * @return return true if the widget keep the focus
			 */
			bool SetFocus(void);
			/**
			 * @brief Remove the focus on this widget
			 * @param ---
			 * @return return true if the widget have release his focus (if he has it)
			 */
			bool RmFocus(void);
			/**
			 * @brief Set the capability to have the focus
			 * @param[in] canFocusState new focus capability
			 * @return ---
			 */
			void SetCanHaveFocus(bool canFocusState);
			/**
			 * @brief Keep the focus on this widget ==> this remove the previous focus on all other widget
			 * @param ---
			 * @return ---
			 */
			void KeepFocus(void);
		protected:
			/**
			 * @brief Event of the focus has been grep by the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void OnGetFocus(void) {};
			/**
			 * @brief Event of the focus has been lost by the current widget
			 * @param ---
			 * @return ---
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
			 * @param ---
			 * @return return the number of event that the mouse supported [0..3]
			 */
			int32_t GetMouseLimit(void) { return m_limitMouseEvent; };
			/**
			 * @brief Get the number of mouse event supported
			 * @param[in] numberState The number of event that the mouse supported [0..3]
			 * @return ---
			 */
			void SetMouseLimit(int32_t numberState) { m_limitMouseEvent = numberState; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Periodic call Area
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			/**
			 * @brief Request that the current widegt have a periodic call
			 * @param statusToSet true if the periodic call is needed
			 * @return ---
			 */
			void PeriodicCallSet(bool statusToSet);
		public:
			/**
			 * @brief Periodic call of this widget
			 * @param localTime curent system time
			 * @return ---
			 */
			virtual void PeriodicCall(int64_t localTime) { };
			
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(Vector2D<float>  pos) { if (false==IsHide()) { return this; } return NULL; };
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float>  pos) { return false; };
			/**
			 * @brief Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref OnEventKb)
			 * @param[in] shift The key Shift (left or/and right) is pressed (if true)
			 * @param[in] control The key control (left or/and right) is pressed (if true)
			 * @param[in] alt The key Alt is pressed (if true)
			 * @param[in] meta The key Meta (windows key/Apple key) (left or/and right) is pressed (if true)
			 * @param[in] unicodeValue key pressed by the user
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventShortCut(bool shift, bool control, bool alt, bool meta, uniChar_t unicodeValue) { return false; };
			/**
			 * @brief Event on the keybord (if no shortcut has been detected before).
			 * @param[in] type of the event (ewol::EVENT_KB_TYPE_DOWN or ewol::EVENT_KB_TYPE_UP)
			 * @param[in] unicodeValue key pressed by the user
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData) { return false; };
			/**
			 * @brief Event on the keyboard that is not a printable key (if no shortcut has been detected before).
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventKbMove(eventKbType_te typeEvent, eventKbMoveType_te moveTypeEvent) { return false; };
			/**
			 * @brief Event on a past event ==> this event is asynchronous due to all system does not support direct getting datas
			 * @note : need to have focus ...
			 * @param[in] mode Mode of data requested
			 * @return ---
			 */
			virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID) { };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			bool         m_needRegenerateDisplay; //!< the display might be done the next regeneration
			/**
			 * @brief The widget mark itself that it need to regenerate the nest time.
			 * @param ---
			 * @return ---
			 */
			void MarkToRedraw(void);
			/**
			 * @brief Get the need of the redrawing of the widget and reset it to false
			 * @param ---
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
			 * @return ---
			 */
			virtual void GenDraw(DrawProperty displayProp);
		protected:
			/**
			 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
			 * @param[in] displayProp properties of the current display
			 * @return ---
			 */
			virtual void OnDraw(DrawProperty& displayProp) { };
		public:
			/**
			 * @brief Event generated when a redraw is needed
			 * @param ---
			 * @return ---
			 */
			virtual void OnRegenerateDisplay(void) { };

	}; // end of the class Widget declaration

};// end of namespace


#endif
