/**
 *******************************************************************************
 * @file ewol/widget/Entry.h
 * @brief ewol entry widget system (header)
 * @author Edouard DUPIN
 * @date 07/11/2011
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

#ifndef __EWOL_ENTRY_H__
#define __EWOL_ENTRY_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventEntryClick;
extern const char * const ewolEventEntryEnter;
extern const char * const ewolEventEntryModify;

namespace ewol {
	class Entry :public ewol::Drawable
	{
		public:
			Entry(void);
			Entry(etk::UString newData);
			virtual ~Entry(void);
			void Init(void);
			virtual bool   CalculateMinSize(void);
			void           SetValue(etk::UString newData);
			etk::UString   GetValue(void);
			void           SetWidth(int32_t width)
			{
				m_userSize = width;
			}
		private:
			etk::UString   m_data;
			color_ts       m_textColorFg;  //!< Text color
			color_ts       m_textColorBg;  //!< Background color
			int32_t        m_userSize;
			int32_t        m_displayStartPosition;
			int32_t        m_displayCursorPos;
			int32_t        m_borderSize;
			int32_t        m_paddingSize;
			void           UpdateTextPosition(void);
			bool           m_displayCursor;
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos);
			virtual bool OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData);
		protected:
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
	};
};

#endif
