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

namespace widget {
	class ColorChooser : public widget::Sizer
	{
		public:
			ColorChooser(void);
			~ColorChooser(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "widget::ColorChooser"; };
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject* CallerObject, const char * eventId, const etk::UString& data);
			// Derived function
			virtual void OnObjectRemove(ewol::EObject* removeObject);
			
			void SetColor(draw::Color newColor);
			draw::Color GetColor(void);
		private:;
			widget::ColorBar* m_widgetColorBar;
			widget::Slider* m_widgetRed;
			widget::Slider* m_widgetGreen;
			widget::Slider* m_widgetBlue;
			widget::Slider* m_widgetAlpha;
			draw::Color m_currentColor;
	};
	
};

#endif
