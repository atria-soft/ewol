/**
 *******************************************************************************
 * @file ewol/widget/Label.h
 * @brief ewol Label widget system (header)
 * @author Edouard DUPIN
 * @date 22/12/2011
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

#ifndef __EWOL_LABEL_H__
#define __EWOL_LABEL_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

extern const char * const ewolEventLabelPressed;

namespace ewol {
	class Label :public ewol::Widget
	{
		public:
			Label(void);
			Label(etk::String newLabel);
			void Init(void);
			virtual ~Label(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::String newLabel);
		private:
			etk::String    m_label;
			color_ts       m_textColorFg;  //!< Text color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
	};
};

#endif
