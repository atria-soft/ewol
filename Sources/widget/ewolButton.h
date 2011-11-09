/**
 *******************************************************************************
 * @file ewolButton.h
 * @brief ewol Button widget system (header)
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

#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etkTypes.h>
#include <ewolDebug.h>
#include <ewolWidget.h>

namespace ewol {
	class Button :public ewol::Widget
	{
		public:
			Button(void);
			Button(etk::String newLabel);
			virtual ~Button(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::String newLabel);
			// TODO : 
			//void           SetSize(int32_t size);
			//void           SetFont(etk::String fontName);
			//void           ResetDefaultParameters(void);
			void           SetValue(bool val);
			bool           GetValue(void);
		private:
			etk::String    m_label;
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			//virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, double x, double y);
			virtual bool OnEventArea(const char * generateEventId, double x, double y);
	};
};

#endif