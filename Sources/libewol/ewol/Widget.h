/**
 *******************************************************************************
 * @file ewol/Widget.h
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

#include <ewol/EObject.h>

#define NB_BOUBLE_BUFFER        (2)

namespace ewol {
	class Widget;
};

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject.h>
#include <etk/VectorType.h>

namespace ewol {
	typedef enum {
		EVENT_INPUT_TYPE_DOWN,
		EVENT_INPUT_TYPE_MOVE,
		EVENT_INPUT_TYPE_SINGLE,
		EVENT_INPUT_TYPE_DOUBLE,
		EVENT_INPUT_TYPE_TRIPLE,
		EVENT_INPUT_TYPE_UP,
		EVENT_INPUT_TYPE_ENTER,
		EVENT_INPUT_TYPE_LEAVE,
		EVENT_INPUT_TYPE_ABORT, // SPecial event generate when an upper classes get an event ... (TBD)
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
	
	char* GetCharTypeMoveEvent(eventKbMoveType_te type);
	
	typedef struct {
		coord2D_ts   abs;
		coord2D_ts   local;
	} eventPosition_ts;
	
	
	
	
	
	class Widget : public EObject {
		public:
			Widget(void);
			// TODO : Set this in private if possible ...
			virtual ~Widget(void);
			void    MarkToRemove(void);
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget Size:
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			// internal element calculated by the system
			coord2D_ts     m_origin;        //!< internal ... I do not really known how i can use it ...
			coord2D_ts     m_size;          //!< internal : current size of the widget
			coord2D_ts     m_minSize;       //!< user define the minimum size of the widget
			// user configuaration
			coord2D_ts     m_userMinSize;   //!< user define the minimum size of the widget
			bool           m_userExpendX;
			bool           m_userExpendY;
			bool           m_userFillX;
			bool           m_userFillY;
		public:
			void           SetOrigin(etkFloat_t x, etkFloat_t y)            { m_origin.x=x; m_origin.y=y;};
			coord2D_ts     GetOrigin(void)                                  { return m_origin; };
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			//update the min Size ... and the expend parameters for the sizer
			virtual bool   CalculateMinSize(void)                           {m_minSize.x = m_userMinSize.x; m_minSize.y = m_userMinSize.y; MarkToReedraw(); return true; };
			virtual void   SetMinSize(etkFloat_t x=-1, etkFloat_t y=-1)     { m_userMinSize.x = x; m_userMinSize.y = y; };
			coord2D_ts     GetMinSize(void)                                 { return m_minSize; };
			coord2D_ts     GetSize(void)                                    { return m_size; };
			void           SetCurrentSise(etkFloat_t x=-1, etkFloat_t y=-1) { m_size.x = x; m_size.y = y; MarkToReedraw();};
			coord2D_ts     GetCurrentSize(void)                             { return m_size; };
			virtual void   SetExpendX(bool newExpend=false)                 { m_userExpendX = newExpend; };
			virtual bool   CanExpentX(void)                                 { return m_userExpendX; };
			virtual void   SetExpendY(bool newExpend=false)                 { m_userExpendY = newExpend; };
			virtual bool   CanExpentY(void)                                 { return m_userExpendY; };
			virtual void   SetFillX(bool newFill=false)                     { m_userFillX = newFill; };
			bool           CanFillX(void)                                   { return m_userFillX; };
			virtual void   SetFillY(bool newFill=false)                     { m_userFillY = newFill; };
			bool           CanFillY(void)                                   { return m_userFillY; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Focus Area
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool    m_hasFocus;        //!< set the focus on this widget
			bool    m_canFocus;        //!< the focus can be done on this widget
		public:
			bool    GetFocus(void) { return m_hasFocus;};
			bool    CanHaveFocus(void) { return m_canFocus;};
			bool    SetFocus(void);
			bool    RmFocus(void);
			void    SetCanHaveFocus(bool canFocusState);
		protected:
			virtual void OnGetFocus(void) {};
			virtual void OnLostFocus(void) {};
		
		public:
			// external acces to set an input event on this widget.
			bool GenEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos); // call when input event arrive and call OnEventInput, if no event detected
			virtual bool GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue);
		protected:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos) { return false; };
		// ----------------------------------------------------------------------------------------------------------------
		// -- Keboard event (when one is present or when a graphical is present
		// ----------------------------------------------------------------------------------------------------------------
		public:
			virtual bool OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData) { return false; };
			virtual bool OnEventKbMove(eventKbType_te typeEvent, eventKbMoveType_te moveTypeEvent) { return false; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			int8_t       m_currentDrawId;         //!< Id of the element that might be displayed by the Gui thread
			int8_t       m_currentCreateId;       //!< Id of the element might be modify
			bool         m_needFlipFlop;          //!< A flip flop need to be done
			bool         m_needRegenerateDisplay; //!< the display might be done the next regeneration
			        void MarkToReedraw(void) { m_needRegenerateDisplay = true; };
			        bool NeedRedraw(void)    { bool tmpData=m_needRegenerateDisplay; m_needRegenerateDisplay=false; return tmpData; };
		public:
			virtual void OnFlipFlopEvent(void);
		public:
			        bool GenDraw(void);
		protected:
			virtual bool OnDraw(void) { return true; };
		public:
			virtual void OnRegenerateDisplay(void) { /* nothing to do */ };

	}; // end of the class Widget declaration
};// end of nameSpace

#endif
