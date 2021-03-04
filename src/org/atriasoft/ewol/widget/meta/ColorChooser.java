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
		using ColorChooser = ememory::Ptr<ewol::widget::ColorChooser>;
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
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(ColorChooser, "ColorChooser");
				 ~ColorChooser();
			private:
				ewol::widget::ColorBar this.widgetColorBar;
				ewol::widget::Slider this.widgetRed;
				ewol::widget::Slider this.widgetGreen;
				ewol::widget::Slider this.widgetBlue;
				ewol::widget::Slider this.widgetAlpha;
				void onCallbackColorChangeRed( float _newColor);
				void onCallbackColorChangeGreen( float _newColor);
				void onCallbackColorChangeBlue( float _newColor);
				void onCallbackColorChangeAlpha( float _newColor);
				void onCallbackColorChange( etk::Color<> _newColor);
			protected:
				 void onChangePropertyValue();
		};
	};
};
