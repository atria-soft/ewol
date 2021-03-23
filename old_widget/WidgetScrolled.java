/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Scroll.hpp>
#include <ewol/compositing/Shaper.hpp>

#define CALCULATE_SIMULTANEOUS_FINGER (5)

namespace ewol {
	namespace widget {
		class WidgetScrolled;
		using WidgetScrolled = ememory::Ptr<ewol::widget::WidgetScrolled>;
		using WidgetScrolledWeak = ememory::WeakPtr<ewol::widget::WidgetScrolled>;
		/**
		 * Widget to integrate a scrool bar in a widget. This is not a stadalone widget.
		 */
		class WidgetScrolled : public Widget {
			public: // properties:
				eproperty::Value<etk::Uri> propertyShapeVert; //!< Vertical shaper name
				eproperty::Value<etk::Uri> propertyShapeHori; //!< Horizontal shaper name
				// TODO : All property
			public:
				enum scrollingMode {
					scroolModeNormal, //!< No Zoom , can UP and down, left and right
					scroolModeCenter, //!< Zoom enable, no move left and right
					scroolModeGame, //!< Zoom enable, no move left and right
				};
			private:
				ewol::compositing::Shaper this.shaperH; //!< Compositing theme Horizontal.
				ewol::compositing::Shaper this.shaperV; //!< Compositing theme Vertical.
			protected:
				Vector2f this.originScrooled; //!< pixel distance from the origin of the display (Bottum left)
				Vector2f this.maxSize; //!< Maximum size of the Widget ==> to display scrollbar
				Vector2f this.limitScrolling; //!< Mimit scrolling represent the propertion of the minimel scrolling activate (0.2 ==> 20% migt all time be visible)
			private: // Mouse section :
				enum scrollingMode this.scroollingMode; //!< mode of management of the scrooling
				float this.pixelScrolling;
				Vector2f this.highSpeedStartPos;
				enum Scroll::highSpeedMode this.highSpeedMode;
				int this.highSpeedButton;
				KeyType this.highSpeedType;
			private: // finger section:
				boolean this.singleFingerMode; //!< in many case the moving in a subwidget is done with one finger, it is enought ==> the user select...
			public:
				/**
				 * Set the single finger capabilities/
				 * @param _status True if single inger mode, two otherwise/
				 */
				void setSingleFinger(boolean _status);
				/**
				 * Get the single finger capabilities
				 * @return true The single finger mode is active
				 * @return false The To finger mode is active
				 */
				boolean getSingleFinger() {
					return this.singleFingerMode;
				}
				/**
				 * Reset the scoll of the subWidget
				 */
				void resetScrollOrigin() {
					this.originScrooled = Vector2f(0,0);
				}
			private:
				boolean this.fingerPresent[CALCULATE_SIMULTANEOUS_FINGER];
				boolean this.fingerScoolActivated;
				Vector2f this.fingerMoveStartPos[CALCULATE_SIMULTANEOUS_FINGER];
			protected:
				/**
				 * Scroll Widget main ructor to be herited from an other widget (this is not a stand-alone widget)
				 * @param _shaperName Shaper name if the scrolled widget.
				 */
				WidgetScrolled();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(WidgetScrolled, "WidgetScrolled");
				/**
				 * Scroll widget destructor.
				 */
				 ~WidgetScrolled();
			protected:
				void onDraw() ;
			public:
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				void systemDraw( ewol::DrawProperty _displayProp) ;
			protected:
				/**
				 * For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
				 * @param _nbPixel number of pixel scrolling
				 */
				void setScrollingSize(float _nbPixel) {
					this.pixelScrolling = _nbPixel;
				};
				/**
				 * Specify the mode of scrolling for this windows
				 * @param _newMode the selected mode for the scrolling...
				 */
				void scroolingMode(enum scrollingMode _newMode);
				/**
				 * set the specific mawimum size of the widget
				 * @param _localSize new Maximum size
				 */
				void setMaxSize( Vector2f _localSize) {
					this.maxSize = _localSize;
				};
				/**
				 * Request a specific position for the scrolling of the current windows.
				 * @param _borderWidth size of the border that requested the element might not to be
				 * @param _currentPosition Position that is requested to view
				 * @param _center True if the position might be at the center of the widget
				 */
				void setScrollingPositionDynamic(Vector2f _borderWidth,  Vector2f _currentPosition, boolean _center = false);
				/**
				 * set the scrolling limit when arriving at he end of the widget
				 * @param _poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
				 */
				void setLimitScrolling(float _poucentageLimit) {
					_poucentageLimit = etk::avg(0.1f, _poucentageLimit,1.0f);
					this.limitScrolling = Vector2f(_poucentageLimit, _poucentageLimit);
				};
				/**
				 * set the scrolling limit when arriving at he end of the widget
				 * @param _poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end for axis specific...
				 */
				void setLimitScrolling( Vector2f _poucentageLimit) {
					this.limitScrolling = Vector2f(etk::avg(0.1f, _poucentageLimit.x(),1.0f), etk::avg(0.1f, _poucentageLimit.y(),1.0f));
				};
			protected:
				 void onChangePropertyShapeVert();
				 void onChangePropertyShapeHori();
		};
	}
}

