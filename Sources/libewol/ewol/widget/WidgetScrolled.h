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
	#ifdef __MODE__Touch
		SCROLL_ENABLE,
	#else
		SCROLL_ENABLE_HORIZONTAL,
		SCROLL_ENABLE_VERTICAL,
	#endif
	}highSpeedMode_te;
	
	class WidgetScrooled : virtual public ewol::Widget
	{
		protected:
			coord2D_ts        m_originScrooled;
			coord2D_ts        m_maxSize;
		private:
			etkFloat_t        m_pixelScrolling;
			coord2D_ts        m_highSpeedStartPos;
			highSpeedMode_te  m_highSpeedMode;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, eventPosition_ts pos);
		protected:
			void SetScrollingSize(etkFloat_t nbPixel) { m_pixelScrolling = nbPixel; };
	};
};

#endif
