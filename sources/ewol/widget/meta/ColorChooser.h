/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_COLOR_CHOOSER_H__
#define __EWOL_COLOR_CHOOSER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ColorBar.h>
#include <ewol/widget/Slider.h>
#include <ewol/object/Signal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ColorChooser : public ewol::widget::Sizer {
			public:
				// Event list of properties
				ewol::object::Signal<etk::Color<>> signalChange;
			protected:
				ColorChooser();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ColorChooser, "ColorChooser");
				virtual ~ColorChooser();
			public:
				void setColor(etk::Color<> _newColor);
				etk::Color<> getColor();
			private:
				std::shared_ptr<ewol::widget::ColorBar> m_widgetColorBar;
				std::shared_ptr<ewol::widget::Slider> m_widgetRed;
				std::shared_ptr<ewol::widget::Slider> m_widgetGreen;
				std::shared_ptr<ewol::widget::Slider> m_widgetBlue;
				std::shared_ptr<ewol::widget::Slider> m_widgetAlpha;
				etk::Color<> m_currentColor;
				void onCallbackColorChangeRed(const int32_t& _newColor);
				void onCallbackColorChangeGreen(const int32_t& _newColor);
				void onCallbackColorChangeBlue(const int32_t& _newColor);
				void onCallbackColorChangeAlpha(const int32_t& _newColor);
				void onCallbackColorChange(const etk::Color<>& _newColor);
		};
	};
};

#endif
