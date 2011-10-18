/**
 *******************************************************************************
 * @file ewolWidget.h
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

namespace ewol {
	extern "C" {
		typedef struct {
			double x;
			double y;
		} coord;
	
	typedef enum {
		EVENT_INPUT_TYPE_DOWN;
		EVENT_INPUT_TYPE_DOUBLE;
		EVENT_INPUT_TYPE_TRIPLE;
		EVENT_INPUT_TYPE_MOVE;
		EVENT_INPUT_TYPE_UP;
	} eventInputType_te;
	
	typedef enum {
		EVENT_KB_TYPE_DOWN;
		EVENT_KB_TYPE_UP;
	} eventKbType_te;
	
	typedef enum {
		EVENT_KB_MOVE_TYPE_LEFT;
		EVENT_KB_MOVE_TYPE_RIGHT;
		EVENT_KB_MOVE_TYPE_UP;
		EVENT_KB_MOVE_TYPE_DOWN;
		EVENT_KB_MOVE_TYPE_PAGE_UP;
		EVENT_KB_MOVE_TYPE_PAGE_DOWN;
		EVENT_KB_MOVE_TYPE_START;
		EVENT_KB_MOVE_TYPE_END;
	} eventKbMoveType_te;
	
	#define UTF8_MAX_SIZE    (8)
	/*
	extern "C" {
		typedef struct {
			bool shift;
			bool control;
			bool alt;
			bool pomme;
			char UTF8_data[UTF8_MAX_SIZE];
			const char * generateEventId; // event generate ID (to be unique it was pointer on the string name)
			int32_t widgetCall; //!< unique ID of the widget
		} shortCut_ts;
	}
	extern "C" {
		typedef struct {
			coord origin; // widget specific
			coord size;   // widget specific
			uint32_t flags; // widget specific
			const char * generateEventId; // event generate ID (to be unique it was pointer on the string name)
			int32_t widgetCall; //!< unique ID of the widget
		} eventArea_ts;
	}
	*/
	#define EWOL_EVENT_UNION       (0)
	#define EWOL_EVENT_SHORTCUT    (1)
	extern "C" {
		typedef struct {
			const char * generateEventId; // event generate ID (to be unique it was pointer on the string name)
			int32_t widgetCall; //!< unique ID of the widget
			int32_t mode; //!< EWOL_EVENT_UNION or EWOL_EVENT_SHORTCUT
			union {
				struct shortCut{
					bool shift;
					bool control;
					bool alt;
					bool pomme;
					char UTF8_data[UTF8_MAX_SIZE];
				};
				struct Area{
					coord origin; // widget specific
					coord size;   // widget specific
					uint32_t flags; // widget specific
				};
		} event_ts;
	}
	
	class Widget;
	
	class Widget {
		public:
			Widget(void);
			virtual ~Widget(void);
		private:
			int32_t m_uniqueId;        //!< UniqueId to identify the widget unicly
		public;
			int32_t GetUniqueId(void) { return m_uniqueId; };
		
		private:
			ewol::Widget *  m_parrent;   //!< parrent of the current widget (if NULL ==> this is the main node(root))
		public:
			void SetParrent(ewol::Widget * newParrent) { m_parrent = newParrent; };
		
		// ----------------------------------------------------------------------------------------------------------------
		// -- Widget Size:
		// ----------------------------------------------------------------------------------------------------------------
		protected:
			coord m_origin;
			coord m_minSize;
			coord m_size;
			coord m_maxSize;
			bool  m_expendX;
			bool  m_expendY;
			void  SetExpendX(bool newExpend=false) { m_expendX = newExpend; };
			void  SetExpendY(bool newExpend=false) { m_expendY = newExpend; };
			void  SetMinSise(double x=-1, double y=-1) { m_minSize.x = x; m_minSize.y = y; };
			void  SetMaxSise(double x=-1, double y=-1) { m_maxSize.x = x; m_maxSize.y = y; };
			void  SetCurrentSise(double x=-1, double y=-1) { m_size.x = x; m_size.y = y; };
		public
			void  SetOrigin(double x, double y) { m_origin.x=x; m_origin.y=y; };
			virtual bool CalculateSize(double availlableX, double availlableY); // this generate the current size ...
			coord GetMinSize(void) { return m_minSize };
			coord GetMaxSize(void) { return m_maxSize };
			coord GetCurrentSize(void) { return m_size };
		
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
			estd::VectorType<event_ts> m_inputEvent;     //!< generic area and short-cut event
		public:
			// external acces to set an input event on this widget.
			bool GenEventInput(int32_t IdInput, eventInputType_te typeEvent, double X, double Y); // call when input event arrive and call OnEventInput, if no event detected
			bool GenEventShortCut(bool shift, bool control, bool alt, bool pomme, char UTF8_data[UTF8_MAX_SIZE]);
		protected:
			bool AddEventArea(coord origin, coord size, uint32_t flags, const char * generateEventId);
			bool AddEventShortCut(bool shift, bool control, bool alt, bool pomme, char UTF8_data[UTF8_MAX_SIZE], const char * generateEventId);
			bool AddEventShortCut(char * descriptiveString, const char * generateEventId);
		public:
			// to link an extern widget at the internal event of this one it will access by here :
			bool ExternLinkOnEvent(const char * eventName, int32_t widgetId);
		protected:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, double X, double Y) { return false; };
			virtual bool OnEventArea(const char * generateEventId) { return false; };
			// when an event arrive from an other widget, it will arrive here:
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId) { return false; };
		
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
			int32_t MultipleDisplayCurrentId;
			//estd::VectorType<event_ts> m_inputEvent;
			bool m_genericDraw;
			bool GenericDraw(void);
		protected:
			void SetNotGenericDraw(void) { m_genericDraw = false; };
			void SetGenericDraw(void) { m_genericDraw = true; };
			//void FlipFlopNext(void); //!< jump at the next display registered
			virtual bool OnDraw(void) { return true; };
		public:
			bool GenDraw(void)
			{
				if (true == m_genericDraw) {
					return GenericDraw();
				} else {
					return OnDraw();
				}
			}
		
	}
};

#endif
