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
	
	typedef enum {
		SCROLL_MODE_NORMAL, //!< No Zoom , can UP and down, left and right
		SCROLL_MODE_CENTER, //!< Zoom enable, no move left and right
	} scrollingMode_te;
	class WidgetScrooled : public ewol::Widget
	{
		protected:
			coord2D_ts        m_originScrooled;
			coord2D_ts        m_maxSize;
			etkFloat_t        m_zoom; //!< current zoom on the display
		private:
			scrollingMode_te  m_scroollingMode; //!< mode of management of the scrooling
			etkFloat_t        m_pixelScrolling;
			coord2D_ts        m_highSpeedStartPos;
			highSpeedMode_te  m_highSpeedMode;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			virtual void OnRegenerateDisplay(void);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, coord2D_ts pos);
			/**
			 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
			 * @param ---
			 * @return ---
			 */
			virtual void GenDraw(void);
		protected:
			void SetScrollingSize(etkFloat_t nbPixel) { m_pixelScrolling = nbPixel; };
			void ScroolingMode(scrollingMode_te newMode) { m_scroollingMode = newMode; };
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_SCROOLED;
	
};

#define EWOL_CAST_WIDGET_SCROOLED(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_SCROOLED,ewol::WidgetScrooled,curentPointer)

#endif
