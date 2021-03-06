/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/ColorBar.hpp>
#include <ewol/widget/Slider.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class ColorChooser;
		using ColorChooserShared = ememory::SharedPtr<ewol::widget::ColorChooser>;
		using ColorChooserWeak = ememory::WeakPtr<ewol::widget::ColorChooser>;
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
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(ColorChooser, "ColorChooser");
				virtual ~ColorChooser();
			private:
				ewol::widget::ColorBarShared m_widgetColorBar;
				ewol::widget::SliderShared m_widgetRed;
				ewol::widget::SliderShared m_widgetGreen;
				ewol::widget::SliderShared m_widgetBlue;
				ewol::widget::SliderShared m_widgetAlpha;
				void onCallbackColorChangeRed(const float& _newColor);
				void onCallbackColorChangeGreen(const float& _newColor);
				void onCallbackColorChangeBlue(const float& _newColor);
				void onCallbackColorChangeAlpha(const float& _newColor);
				void onCallbackColorChange(const etk::Color<>& _newColor);
			protected:
				virtual void onChangePropertyValue();
		};
	};
};
