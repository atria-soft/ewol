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

#define NB_BOUBLE_BUFFER        (2)

namespace ewol {
	class Widget;
};

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject.h>
#include <etk/VectorType.h>

namespace ewol {
	extern "C" {
		typedef struct {
			etkFloat_t x;
			etkFloat_t y;
		} coord;
	}
	typedef enum {
		EVENT_INPUT_TYPE_DOWN,
		EVENT_INPUT_TYPE_MOVE,
		EVENT_INPUT_TYPE_SINGLE,
		EVENT_INPUT_TYPE_DOUBLE,
		EVENT_INPUT_TYPE_TRIPLE,
		EVENT_INPUT_TYPE_UP,
		EVENT_INPUT_TYPE_ENTER,
		EVENT_INPUT_TYPE_LEAVE,
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
	
	enum {
		FLAG_EVENT_INPUT_1               = 1 << 0,
		FLAG_EVENT_INPUT_2               = 1 << 1,
		FLAG_EVENT_INPUT_3               = 1 << 2,
		FLAG_EVENT_INPUT_4               = 1 << 3,
		FLAG_EVENT_INPUT_5               = 1 << 4,
		FLAG_EVENT_INPUT_6               = 1 << 5,
		FLAG_EVENT_INPUT_7               = 1 << 6,
		FLAG_EVENT_INPUT_8               = 1 << 7,
		FLAG_EVENT_INPUT_9               = 1 << 8,
		FLAG_EVENT_INPUT_10              = 1 << 9,
		FLAG_EVENT_INPUT_11              = 1 << 10,
		FLAG_EVENT_INPUT_12              = 1 << 11,
		FLAG_EVENT_INPUT_13              = 1 << 12,
		FLAG_EVENT_INPUT_14              = 1 << 13,
		FLAG_EVENT_INPUT_15              = 1 << 14,
		FLAG_EVENT_INPUT_16              = 1 << 15,
		FLAG_EVENT_INPUT_17              = 1 << 16,
		
		FLAG_EVENT_INPUT_MOTION          = 1 << 20,
		FLAG_EVENT_INPUT_ENTER           = 1 << 21,
		FLAG_EVENT_INPUT_LEAVE           = 1 << 22,
		FLAG_EVENT_INPUT_DOWN            = 1 << 23,
		FLAG_EVENT_INPUT_UP              = 1 << 24,
		FLAG_EVENT_INPUT_CLICKED         = 1 << 25,
		FLAG_EVENT_INPUT_CLICKED_DOUBLE  = 1 << 26,
		FLAG_EVENT_INPUT_CLICKED_TRIPLE  = 1 << 27,
		FLAG_EVENT_INPUT_CLICKED_ALL     = FLAG_EVENT_INPUT_CLICKED + FLAG_EVENT_INPUT_CLICKED_DOUBLE + FLAG_EVENT_INPUT_CLICKED_TRIPLE,
	};
	
	#define FLAG_EVENT_INPUT_BT_LEFT        (FLAG_EVENT_INPUT_1)
	#define FLAG_EVENT_INPUT_BT_CENTER      (FLAG_EVENT_INPUT_2)
	#define FLAG_EVENT_INPUT_BT_RIGHT       (FLAG_EVENT_INPUT_3)
	#define FLAG_EVENT_INPUT_BT_SCROOL_UP   (FLAG_EVENT_INPUT_4)
	#define FLAG_EVENT_INPUT_BT_SCROOL_DOWN (FLAG_EVENT_INPUT_5)
	
	
	#define UTF8_MAX_SIZE          (8)
	#define EWOL_EVENT_AREA        (0)
	#define EWOL_EVENT_SHORTCUT    (1)
	extern "C" {
		typedef struct {
			const char * generateEventId; // event generate ID (to be unique it was pointer on the string name)
			coord origin;   // widget specific
			coord size;     // widget specific
			uint64_t flags; // widget specific
		} eventArea_ts;
		typedef struct {
			const char * generateEventId; // event generate ID (to be unique it was pointer on the string name)
			bool shift;
			bool control;
			bool alt;
			bool meta;
			uint32_t UnicodeValue;
		} eventShortCut_ts;
		typedef struct {
			const char * generateEventId;       //!< event generate ID (to be unique it was pointer on the string name)
			int32_t      widgetCall;            //!< unique ID of the widget
			const char * generateEventIdExtern; //!< External generated event ID (to be unique it was pointer on the string name)
		} eventExtern_ts;
	};
	
	class Widget {
		public:
			Widget(void);
			virtual ~Widget(void);
			int32_t GetWidgetId(void);
		private:
			ewol::Widget *  m_parrent;   //!< parrent of the current widget (if NULL ==> this is the main node(root))
		public:
			void SetParrent(ewol::Widget * newParrent) { m_parrent = newParrent; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget Size:
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			// internal element calculated by the system
			coord          m_origin;        //!< internal ... I do not really known how i can use it ...
			coord          m_size;          //!< internal : current size of the widget
			coord          m_minSize;       //!< user define the minimum size of the widget
			// user configuaration
			coord          m_userMinSize;   //!< user define the minimum size of the widget
			bool           m_userExpendX;
			bool           m_userExpendY;
			bool           m_userFillX;
			bool           m_userFillY;
		public:
			void           SetOrigin(etkFloat_t x, etkFloat_t y)            { m_origin.x=x; m_origin.y=y; };
			coord          GetOrigin(void)                                  { return m_origin; };
			virtual bool   CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY); // this generate the current size ...
			 //update the min Size ... and the expend parameters for the sizer
			virtual bool   CalculateMinSize(void)                           {m_minSize.x = m_userMinSize.x; m_minSize.y = m_userMinSize.y; return true; };
			virtual void   SetMinSize(etkFloat_t x=-1, etkFloat_t y=-1)     { m_userMinSize.x = x; m_userMinSize.y = y; };
			coord          GetMinSize(void)                                 { return m_minSize; };
			coord          GetSize(void)                                    { return m_size; };
			void           SetCurrentSise(etkFloat_t x=-1, etkFloat_t y=-1) { m_size.x = x; m_size.y = y; };
			coord          GetCurrentSize(void)                             { return m_size; };
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
			bool    SetFocus(void)
			{
				if (true == m_canFocus) {
					m_hasFocus = true;
					OnGetFocus();
					return true;
				}
				return false;
			}
			bool    RmFocus(void)
			{
				if (true == m_canFocus) {
					m_hasFocus = false;
					OnLostFocus();
					return true;
				}
				return false;
			}
			bool    GetFocus(void) { return m_hasFocus;};
			bool    CanHaveFocus(void) { return m_canFocus;};
			void    SetCanHaveFocus(bool canFocusState)
			{
				m_canFocus = canFocusState;
				if (true == m_hasFocus) {
					(void)RmFocus();
				}
			}
		protected:
			virtual void OnGetFocus(void) {};
			virtual void OnLostFocus(void) {};
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Input Event on the widget : The input is the muse for a computer, or the finger for the tablettes
		// -- Shortcut: (only for computer) ==> must be manage otherwise for tablette pc
		// ----------------------------------------------------------------------------------------------------------------
		private:
			etk::VectorType<eventArea_ts>     m_inputAreaEvent;      //!< generic area event
			etk::VectorType<eventShortCut_ts> m_inputShortCutEvent;  //!< generic short-cut event
			etk::VectorType<eventExtern_ts>   m_externEvent;         //!< Generic list of event generation for output link
			etk::VectorType<const char*>      m_ListEventAvaillable; //!< List of all event availlable for this widget
		public:
			// external acces to set an input event on this widget.
			bool GenEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t X, etkFloat_t Y); // call when input event arrive and call OnEventInput, if no event detected
			bool GenEventInputExternal(const char * generateEventId, etkFloat_t x, etkFloat_t y);
			bool GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue);
		protected:
			void AddEventId(const char * generateEventId) {
				if (NULL != generateEventId) {
					m_ListEventAvaillable.PushBack(generateEventId);
				}
			}
			void EventAreaRemoveAll(void) { m_inputAreaEvent.Clear();m_inputShortCutEvent.Clear(); };
			bool AddEventArea(coord origin, coord size, uint64_t flags, const char * generateEventId);
			bool AddEventShortCut(bool shift, bool control, bool alt, bool pomme, uint32_t unicodeValue, const char * generateEventId);
			bool AddEventShortCut(char * descriptiveString, const char * generateEventId);
		public:
			// to link an extern widget at the internal event of this one it will access by here :
			bool ExternLinkOnEvent(const char * eventName, int32_t widgetId, const char * eventExternId = NULL);
		protected:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t X, etkFloat_t Y) { return false; };
			virtual bool OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y) { return false; };
		public:
			// when an event arrive from an other widget, it will arrive here:
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y) { return false; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Keboard event (when one is present or when a graphical is present
		// ----------------------------------------------------------------------------------------------------------------
		public:
			virtual bool OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE]) { return false; };
			virtual bool OnEventKbMove(eventKbType_te typeEvent, eventKbMoveType_te moveTypeEvent) { return false; };
		
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Drawing : Special case ==> have internal system drawing management to prevent reconstriction of a widget
		// --           this will automaticly regenerate the same view in openGL
		// ----------------------------------------------------------------------------------------------------------------
		private:
			bool m_genericDraw;
			bool m_specificDraw;
			int32_t m_currentDrawId;
			int32_t m_currentCreateId;
			bool    m_needFlipFlop;
			etk::VectorType<ewol::OObject*> m_listOObject[NB_BOUBLE_BUFFER];   //!< generic element to display...
			bool GenericDraw(void);
		public:
			void DoubleBufferFlipFlop(void);
		protected:
			void AddOObject(ewol::OObject* newObject, etk::String name = "", int32_t pos=-1);
			ewol::OObject* GetOObject(etk::String name);
			void RmOObjectElem(etk::String name);
			void ClearOObjectList(void);
			void GenericDrawDisable(void) { m_genericDraw = false; };
			void GenericDrawEnable(void) { m_genericDraw = true; };
			void SpecificDrawDisable(void) { m_specificDraw = false; };
			void SpecificDrawEnable(void) { m_specificDraw = true; };
			virtual bool OnDraw(void) { /*EWOL_ERROR("plop");*/ return true; };
		private:
			bool m_needRegenerateDisplay;
		protected:
			void MarkToReedraw(void) { m_needRegenerateDisplay = true; };
			bool NeedRedraw(void) { bool tmpData=m_needRegenerateDisplay; m_needRegenerateDisplay=false; return tmpData; };
		public:
			virtual void OnRegenerateDisplay(void) { };
			bool GenDraw(void)
			{
				if (true == m_genericDraw) {
					//EWOL_DEBUG("Draw generic...");
					GenericDraw();
				}
				if (true == m_specificDraw) {
					//EWOL_DEBUG("Draw Custum...");
					OnDraw();
				}
				return true;
			};

	}; // end of the class Widget declaration
};// end of nameSpace

#endif
