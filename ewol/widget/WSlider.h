/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>
#include <esignal/ISignal.h>

namespace ewol {
	namespace widget {
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
				esignal::ISignal<> signalStartSlide;
				esignal::ISignal<> signalStopSlide;
			public: // properties:
				eproperty::Range<float> propertyTransitionSpeed; //!< speed of the transition (default 1  == > 1s)
				eproperty::List<enum sladingMode> propertyTransitionMode; //!< mode to slide the widgets
				eproperty::Value<std::string> propertySelectWidget; //!< current select configuration
			protected:
				WSlider();
				void init() {
					ewol::widget::ContainerN::init();
				};
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
				void subWidgetSelectSet(const std::shared_ptr<ewol::Widget>& _widgetPointer);
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _widgetName Name of the subwidget name
				 */
				void subWidgetSelectSet(const std::string& _widgetName);
			public: // Derived function
				virtual void onChangeSize();
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
		};
	}
	std::ostream& operator <<(std::ostream& _os, const enum ewol::widget::WSlider::sladingMode _obj);
}

