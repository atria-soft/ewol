/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SCROLLED_WIDGET_H__
#define __EWOL_SCROLLED_WIDGET_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Scroll.h>
#include <ewol/compositing/Compositing.h>

namespace widget {
	
	typedef enum {
		SCROLL_MODE_NORMAL, //!< No Zoom , can UP and down, left and right
		SCROLL_MODE_CENTER, //!< Zoom enable, no move left and right
		SCROLL_MODE_GAME, //!< Zoom enable, no move left and right
	} scrollingMode_te;
	class WidgetScrooled : public ewol::Widget
	{
		private:
			etk::Vector<ewol::Compositing*> m_listOObject; //!< generic element to display...
			void addOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
			void clearOObjectList(void);
		protected:
			vec2 m_originScrooled;
			vec2 m_maxSize;
			float m_limitScrolling;
		private:
			scrollingMode_te m_scroollingMode; //!< mode of management of the scrooling
			float m_pixelScrolling;
			vec2 m_highSpeedStartPos;
			highSpeedMode_te m_highSpeedMode;
			int32_t m_highSpeedButton;
			ewol::keyEvent::type_te m_highSpeedType;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
		protected: // Derived function
			virtual void onDraw(void);
		public: // Derived function
			virtual const char * const getObjectType(void) { return "EwolWidgetScrooled"; };
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual void systemDraw(const ewol::drawProperty& displayProp);
		protected:
			/**
			 * @brief For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
			 * @param[in] nbPixel number of pixel scrolling
			 */
			void setScrollingSize(float nbPixel) { m_pixelScrolling = nbPixel; };
			/**
			 * @brief Specify the mode of scrolling for this windows
			 * @param[in] newMode the selected mode for the scrolling...
			 */
			void scroolingMode(scrollingMode_te newMode);
			/**
			 * @brief set the specific mawimum size of the widget
			 * @param[in] localSize new Maximum size
			 */
			void setMaxSize(vec2  localSize) { m_maxSize = localSize; };
			/**
			 * @brief Request a specific position for the scrolling of the current windows.
			 * @param[in] borderWidth size of the border that requested the element might not to be
			 * @param[in] currentPosition Position that is requested to view
			 * @param[in] center True if the position might be at the center of the widget
			 */
			void setScrollingPositionDynamic(vec2  borderWidth, vec2  currentPosition, bool center = false);
			/**
			 * @brief set the scrolling limit when arriving at he end of the widget
			 * @param[in] poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
			 */
			void setLimitScrolling(float poucentageLimit) { m_limitScrolling = etk_avg(0.1, poucentageLimit,0.9); };
	};
	
};

#endif
