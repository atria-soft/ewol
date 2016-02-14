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
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ColorBar.h>
#include <ewol/widget/Slider.h>
#include <esignal/Signal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ColorChooser : public ewol::widget::Sizer {
			public: // signals
				esignal::Signal<etk::Color<>> signalChange;
			public:
				eproperty::Value<etk::Color<>> propertyValue;
			protected:
				ColorChooser();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ColorChooser, "ColorChooser");
				virtual ~ColorChooser();
			private:
				std::shared_ptr<ewol::widget::ColorBar> m_widgetColorBar;
				std::shared_ptr<ewol::widget::Slider> m_widgetRed;
				std::shared_ptr<ewol::widget::Slider> m_widgetGreen;
				std::shared_ptr<ewol::widget::Slider> m_widgetBlue;
				std::shared_ptr<ewol::widget::Slider> m_widgetAlpha;
				void onCallbackColorChangeRed(const float& _newColor);
				void onCallbackColorChangeGreen(const float& _newColor);
				void onCallbackColorChangeBlue(const float& _newColor);
				void onCallbackColorChangeAlpha(const float& _newColor);
				void onCallbackColorChange(const etk::Color<>& _newColor);
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
		};
	};
};
