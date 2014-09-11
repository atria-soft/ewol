/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_SLIDER_H__
#define __EWOL_SLIDER_H__

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/object/Signal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Slider : public ewol::Widget {
			public:
				// Event list of properties
				ewol::object::Signal<int32_t> signalChange;
			protected:
				Slider();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Slider, "Slider");
				virtual ~Slider();
				void setValue(int32_t _val);
				int32_t getValue();
				void setMin(int32_t _val);
				void setMax(int32_t _val);
				void setColor(etk::Color<> _newColor) {
					m_textColorFg = _newColor;
				};
			protected:
				ewol::compositing::Drawing m_draw; //!< drawing tool.
				int32_t m_value;
				int32_t m_min;
				int32_t m_max;
				etk::Color<> m_textColorFg; //!< Text color
				etk::Color<> m_textColorBg; //!< Background color
			public: // Derived function
				virtual void onDraw();
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
		};
	};
};

#endif
