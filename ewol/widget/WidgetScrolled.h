/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_SCROLLED_WIDGET_H__
#define __EWOL_SCROLLED_WIDGET_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Scroll.h>
#include <ewol/compositing/Shaper.h>

#define CALCULATE_SIMULTANEOUS_FINGER (5)

namespace ewol {
	namespace widget {
		/**
		 * @brief Widget to integrate a scrool bar in a widget. This is not a stadalone widget.
		 */
		class WidgetScrolled : public ewol::Widget {
			public:
				enum scrollingMode {
					scroolModeNormal, //!< No Zoom , can UP and down, left and right
					scroolModeCenter, //!< Zoom enable, no move left and right
					scroolModeGame, //!< Zoom enable, no move left and right
				};
			private:
				ewol::compositing::Shaper m_shaperH; //!< Compositing theme Horizontal.
				ewol::compositing::Shaper m_shaperV; //!< Compositing theme Vertical.
			protected:
				vec2 m_originScrooled; //!< pixel distance from the origin of the display (Bottum left)
				vec2 m_maxSize; //!< Maximum size of the Widget ==> to display scrollbar
				float m_limitScrolling; //!< Mimit scrolling represent the propertion of the minimel scrolling activate (0.2 ==> 20% migt all time be visible)
			private: // Mouse section :
				enum scrollingMode m_scroollingMode; //!< mode of management of the scrooling
				float m_pixelScrolling;
				vec2 m_highSpeedStartPos;
				enum Scroll::highSpeedMode m_highSpeedMode;
				int32_t m_highSpeedButton;
				enum gale::key::type m_highSpeedType;
			private: // finger section:
				bool m_singleFingerMode; //!< in many case the moving in a subwidget is done with one finger, it is enought ==> the user select...
			public:
				/**
				 * @brief Set the single finger capabilities/
				 * @param[in] _status True if single inger mode, two otherwise/
				 */
				void setSingleFinger(bool _status);
				/**
				 * @brief Get the single finger capabilities
				 * @return true The single finger mode is active
				 * @return false The To finger mode is active
				 */
				bool getSingleFinger() {
					return m_singleFingerMode;
				}
			private:
				bool m_fingerPresent[CALCULATE_SIMULTANEOUS_FINGER];
				bool m_fingerScoolActivated;
				vec2 m_fingerMoveStartPos[CALCULATE_SIMULTANEOUS_FINGER];
			protected:
				/**
				 * @brief Scroll Widget main constructor to be herited from an other widget (this is not a stand-alone widget)
				 * @param[in] _shaperName Shaper name if the scrolled widget.
				 */
				WidgetScrolled();
				void init(const std::string& _shaperName="THEME:GUI:WidgetScrolled.json");
			public:
				DECLARE_WIDGET_FACTORY(WidgetScrolled, "WidgetScrolled");
				/**
				 * @brief Scroll widget destructor.
				 */
				virtual ~WidgetScrolled();
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
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
				void setMaxSize(const vec2& _localSize) {
					m_maxSize = _localSize;
				};
				/**
				 * @brief Request a specific position for the scrolling of the current windows.
				 * @param[in] _borderWidth size of the border that requested the element might not to be
				 * @param[in] _currentPosition Position that is requested to view
				 * @param[in] _center True if the position might be at the center of the widget
				 */
				void setScrollingPositionDynamic(vec2 _borderWidth, const vec2& _currentPosition, bool _center = false);
				/**
				 * @brief set the scrolling limit when arriving at he end of the widget
				 * @param[in] _poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
				 */
				void setLimitScrolling(float _poucentageLimit) {
					m_limitScrolling = std::avg(0.1f, _poucentageLimit,0.9f);
				};
		};
	};
};

#endif
