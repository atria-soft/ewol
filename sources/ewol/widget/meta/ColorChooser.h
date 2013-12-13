/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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

extern const char * const ewolEventColorChooserChange;

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ColorChooser : public ewol::widget::Sizer {
			public:
				ColorChooser(void);
				~ColorChooser(void);
			public: // Derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
				virtual void onObjectRemove(ewol::Object* _removeObject);
			public:
				void setColor(etk::Color<> _newColor);
				etk::Color<> getColor(void);
			private:
				ewol::widget::ColorBar* m_widgetColorBar;
				ewol::widget::Slider* m_widgetRed;
				ewol::widget::Slider* m_widgetGreen;
				ewol::widget::Slider* m_widgetBlue;
				ewol::widget::Slider* m_widgetAlpha;
				etk::Color<> m_currentColor;
		};
	};
};

#endif
