/**
 *******************************************************************************
 * @file ewol/widget/CheckBox.h
 * @brief ewol check-box widget system (header)
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

#ifndef __EWOL_CHECK_BOX_H__
#define __EWOL_CHECK_BOX_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

extern const char * ewolEventCheckBoxClicked;

namespace ewol {
	class CheckBox :public ewol::Widget
	{
		public:
			CheckBox(void);
			CheckBox(etk::String newLabel);
			void Init(void);
			virtual ~CheckBox(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::String newLabel);
			void           SetValue(bool val);
			bool           GetValue(void);
		private:
			etk::String    m_label;
			bool           m_value;
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
