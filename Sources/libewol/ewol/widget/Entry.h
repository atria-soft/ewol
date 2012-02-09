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
#include <ewol/Widget.h>

extern const char * const ewolEventEntryClick;
extern const char * const ewolEventEntryEnter;

namespace ewol {
	class Entry :public ewol::Widget
	{
		public:
			Entry(void);
			Entry(etk::String newData);
			virtual ~Entry(void);
			void Init(void);
			virtual bool   CalculateMinSize(void);
			void           SetValue(etk::String newData);
			etk::String    GetValue(void);
			void           SetWidth(int32_t width)
			{
				m_userSize = width;
			}
		private:
			etk::String    m_data;
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
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
			virtual bool OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE]);
		protected:
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
	};
};

#endif
