/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/ContainerN.hpp>
#include <ewol/widget/Manager.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class WSlider;
		using WSlider = ememory::Ptr<ewol::widget::WSlider>;
		using WSliderWeak = ememory::WeakPtr<ewol::widget::WSlider>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class WSlider :public ewol::widget::ContainerN {
			public:
				enum sladingMode {
					sladingTransitionVert,
					sladingTransitionHori,
					sladingTransition_count,
				};
			public: // signals:
				esignal::Signal<> signalStartSlide;
				esignal::Signal<> signalStopSlide;
			public: // properties:
				eproperty::Range<float> propertyTransitionSpeed; //!< speed of the transition (default 1  == > 1s)
				eproperty::List<enum sladingMode> propertyTransitionMode; //!< mode to slide the widgets
				eproperty::Value<String> propertySelectWidget; //!< current select configuration
			protected:
				WSlider();
			public:
				DECLARE_WIDGET_FACTORY(WSlider, "WSlider");
				 ~WSlider();
			private:
				int this.windowsSources; //!< widget source viewed
				int this.windowsDestination; //!< widget destinated viewed
				int this.windowsRequested; //!< widget destination requested when change in modification in progress
				float this.slidingProgress; //!< ratio progression of a sliding
			protected:
				/**
				 * Generate the move on the specific vector ID (This is not a public acces, because the vector can have some null pointer inside ...)
				 * @param _id Id in the vector
				 */
				void subWidgetSelectSetVectorId(int _id);
			public:
				/** 
				 * Select a new subwidget to display
				 * @param _id Id of the subwidget requested
				 */
				void subWidgetSelectSet(int _id);
				/** 
				 * Select a new subwidget to display
				 * @param _widgetPointer Pointer on the widget selected (must be added before)
				 */
				void subWidgetSelectSet( Widget _widgetPointer);
				/** 
				 * Select a new subwidget to display
				 * @param _widgetName Name of the subwidget name
				 */
				void subWidgetSelectSet( String _widgetName);
			public:
				void onChangeSize() ;
				void systemDraw( ewol::DrawProperty _displayProp) ;
				void onRegenerateDisplay() ;
				Widget getWidgetAtPos( Vector2f _pos) ;
			protected:
				esignal::Connection this.PCH; //!< Periodic call handle to remove it when needed
				/**
				 * Periodic call to update grapgic display
				 * @param _event Time generic event
				 */
				void periodicCall( ewol::event::Time _event);
			protected:
				 void onChangePropertySelectWidget();
				 void onChangePropertyTransitionMode();
		};
	}
	etk::Stream operator +(etk::Stream _os,  enum ewol::widget::WSlider::sladingMode _obj);
}

