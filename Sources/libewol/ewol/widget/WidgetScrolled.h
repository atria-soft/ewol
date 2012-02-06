/**
 *******************************************************************************
 * @file ewol/widget/WidgetScrolled.h
 * @brief ewol Scrooled widget system (header)
 * @author Edouard DUPIN
 * @date 06/02/2012
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

#ifndef __EWOL_SCROLLED_WIDGET_H__
#define __EWOL_SCROLLED_WIDGET_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

namespace ewol {
	class WidgetScrooled :public ewol::Widget
	{
		protected:
			int32_t m_originScrooledX;
			int32_t m_originScrooledY;
		private:
			uint32_t m_pixelScrolling;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
			virtual bool OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
		protected:
			void SetScrollingSize(uint32_t nbPixel) { m_pixelScrolling = nbPixel; };
	};
};

#endif
