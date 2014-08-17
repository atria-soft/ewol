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

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ColorChooser : public ewol::widget::Sizer {
			public:
				// Event list of properties
				static const char * const eventChange;
			protected:
				ColorChooser();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ColorChooser, "ColorChooser");
				virtual ~ColorChooser();
			public: // Derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
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
		};
	};
};

#endif
