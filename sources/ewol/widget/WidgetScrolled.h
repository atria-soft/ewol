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
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class WidgetScrooled : public ewol::Widget {
		public:
			enum scrollingMode {
				scroolModeNormal, //!< No Zoom , can UP and down, left and right
				scroolModeCenter, //!< Zoom enable, no move left and right
				scroolModeGame, //!< Zoom enable, no move left and right
			};
		private:
			std::vector<ewol::Compositing*> m_listOObject; //!< generic element to display...
			void addOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
			void clearOObjectList(void);
		protected:
			vec2 m_originScrooled;
			vec2 m_maxSize;
			float m_limitScrolling;
		private:
			enum scrollingMode m_scroollingMode; //!< mode of management of the scrooling
			float m_pixelScrolling;
			vec2 m_highSpeedStartPos;
			enum Scroll::highSpeedMode m_highSpeedMode;
			int32_t m_highSpeedButton;
			enum ewol::keyEvent::type m_highSpeedType;
		public:
			WidgetScrooled(void);
			virtual ~WidgetScrooled(void);
		protected: // Derived function
			virtual void onDraw(void);
		public: // Derived function
			virtual const char * const getObjectType(void) {
				return "widget::Scrooled";
			};
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual void systemDraw(const ewol::DrawProperty& _displayProp);
		protected:
			/**
			 * @brief For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
			 * @param[in] _nbPixel number of pixel scrolling
			 */
			void setScrollingSize(float _nbPixel) {
				m_pixelScrolling = _nbPixel;
			};
			/**
			 * @brief Specify the mode of scrolling for this windows
			 * @param[in] _newMode the selected mode for the scrolling...
			 */
			void scroolingMode(enum scrollingMode _newMode);
			/**
			 * @brief set the specific mawimum size of the widget
			 * @param[in] _localSize new Maximum size
			 */
			void setMaxSize(vec2 _localSize) {
				m_maxSize = _localSize;
			};
			/**
			 * @brief Request a specific position for the scrolling of the current windows.
			 * @param[in] _borderWidth size of the border that requested the element might not to be
			 * @param[in] _currentPosition Position that is requested to view
			 * @param[in] _center True if the position might be at the center of the widget
			 */
			void setScrollingPositionDynamic(vec2 _borderWidth, vec2 _currentPosition, bool _center = false);
			/**
			 * @brief set the scrolling limit when arriving at he end of the widget
			 * @param[in] _poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
			 */
			void setLimitScrolling(float _poucentageLimit) {
				m_limitScrolling = etk_avg(0.1, _poucentageLimit,0.9);
			};
	};
	
};

#endif
