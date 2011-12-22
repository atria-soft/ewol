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
		private:
			etk::String    m_data;
			color_ts       m_textColorFg;  //!< Text color
			color_ts       m_textColorBg;  //!< Background color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			//virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
			virtual bool OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y);
			virtual bool OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE]);
	};
};

#endif
