/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
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
		using WSliderShared = ememory::SharedPtr<ewol::widget::WSlider>;
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
				eproperty::Value<std::string> propertySelectWidget; //!< current select configuration
			protected:
				WSlider();
			public:
				DECLARE_WIDGET_FACTORY(WSlider, "WSlider");
				virtual ~WSlider();
			private:
				int32_t m_windowsSources; //!< widget source viewed
				int32_t m_windowsDestination; //!< widget destinated viewed
				int32_t m_windowsRequested; //!< widget destination requested when change in modification in progress
				float m_slidingProgress; //!< ratio progression of a sliding
			protected:
				/**
				 * @brief Generate the move on the specific vector ID (This is not a public acces, because the vector can have some null pointer inside ...)
				 * @param[in] _id Id in the vector
				 */
				void subWidgetSelectSetVectorId(int32_t _id);
			public:
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _id Id of the subwidget requested
				 */
				void subWidgetSelectSet(int32_t _id);
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _widgetPointer Pointer on the widget selected (must be added before)
				 */
				void subWidgetSelectSet(const ewol::WidgetShared& _widgetPointer);
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _widgetName Name of the subwidget name
				 */
				void subWidgetSelectSet(const std::string& _widgetName);
			public:
				void onChangeSize() override;
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
				void onRegenerateDisplay() override;
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
			protected:
				esignal::Connection m_PCH; //!< Periodic call handle to remove it when needed
				/**
				 * @brief Periodic call to update grapgic display
				 * @param[in] _event Time generic event
				 */
				void periodicCall(const ewol::event::Time& _event);
			protected:
				virtual void onChangePropertySelectWidget();
				virtual void onChangePropertyTransitionMode();
		};
	}
	std::ostream& operator <<(std::ostream& _os, const enum ewol::widget::WSlider::sladingMode _obj);
}

