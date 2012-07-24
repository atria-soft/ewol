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
		SCROLL_ENABLE_FINGER,     // Specific for touchpad
		SCROLL_ENABLE_HORIZONTAL, // Specific for mouse
		SCROLL_ENABLE_VERTICAL,   // Specific for mouse
		SCROLL_GREP_END_EVENT,
	}highSpeedMode_te;
	
	typedef enum {
		SCROLL_MODE_NORMAL, //!< No Zoom , can UP and down, left and right
		SCROLL_MODE_CENTER, //!< Zoom enable, no move left and right
	} scrollingMode_te;
	class WidgetScrooled : public ewol::Widget
	{
		private:
			etk::VectorType<ewol::OObject*> m_listOObject[NB_BOUBLE_BUFFER];   //!< generic element to display...
			void    AddOObject(ewol::OObject* newObject, int32_t pos=-1);
			void    ClearOObjectList(void);
		protected:
			Vector2D<float>          m_originScrooled;
			Vector2D<float>          m_maxSize;
			float                    m_zoom; //!< current zoom on the display
			float                    m_limitScrolling;
		private:
			scrollingMode_te   m_scroollingMode; //!< mode of management of the scrooling
			float              m_pixelScrolling;
			Vector2D<float>    m_highSpeedStartPos;
			highSpeedMode_te   m_highSpeedMode;
			int32_t            m_highSpeedButton;
			ewol::inputType_te m_highSpeedType;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolWidgetScrooled"; };
			virtual void OnRegenerateDisplay(void);
			virtual void OnDraw(DrawProperty& displayProp);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, ewol::eventInputType_te typeEvent, Vector2D<float>  pos);
			/**
			 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
			 * @param ---
			 * @return ---
			 */
			virtual void GenDraw(DrawProperty displayProp);
		protected:
			/**
			 * @brief For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
			 * @param[in] nbPixel number of pixel scrolling
			 * @return ---
			 */
			void SetScrollingSize(float nbPixel) { m_pixelScrolling = nbPixel; };
			/**
			 * @brief Specify the mode of scrolling for this windows
			 * @param[in] newMode the selected mode for the scrolling...
			 * @return ---
			 */
			void ScroolingMode(scrollingMode_te newMode) { m_scroollingMode = newMode; };
			/**
			 * @brief Set the specific mawimum size of the widget
			 * @param[in] localSize new Maximum size
			 * @return ---
			 */
			void SetMaxSize(Vector2D<float>  localSize) { m_maxSize = localSize; };
			/**
			 * @brief Request a specific position for the scrolling of the current windows.
			 * @param[in] borderWidth Size of the border that requested the element might not to be
			 * @param[in] currentPosition Position that is requested to view
			 * @param[in] center True if the position might be at the center of the widget
			 * @return ---
			 */
			void SetScrollingPositionDynamic(Vector2D<float>  borderWidth, Vector2D<float>  currentPosition, bool center = false);
			/**
			 * @brief Set the scrolling limit when arriving at he end of the widget
			 * @param[in] poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
			 * @return ---
			 */
			void SetLimitScrolling(float poucentageLimit) { m_limitScrolling = etk_avg(0.1, poucentageLimit,0.9); };
	};
	
};

#endif
