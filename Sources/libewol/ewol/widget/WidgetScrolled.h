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
	typedef enum {
		SCROLL_DISABLE,
		SCROLL_INIT,
		SCROLL_ENABLE_HORIZONTAL,
		SCROLL_ENABLE_VERTICAL,
	}highSpeedMode_te;
	
	class WidgetScrooled :public ewol::Widget
	{
		protected:
			coord    m_originScrooled;
			coord    m_maxSize;
		private:
			etkFloat_t        m_pixelScrolling;
			coord             m_highSpeedStartPos;
			highSpeedMode_te  m_highSpeedMode;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
		protected:
			void SetScrollingSize(etkFloat_t nbPixel) { m_pixelScrolling = nbPixel; };
	};
};

#endif