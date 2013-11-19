/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SLIDER_H__
#define __EWOL_SLIDER_H__

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>

extern const char * const ewolEventSliderChange;

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Slider :public ewol::Widget {
		public:
			static void init(ewol::WidgetManager& _widgetManager);
		public:
			Slider(void);
			virtual ~Slider(void);
			void setValue(int32_t _val);
			int32_t getValue(void);
			void setMin(int32_t _val);
			void setMax(int32_t _val);
			void setColor(etk::Color<> _newColor) {
				m_textColorFg = _newColor;
			};
		protected:
			ewol::Drawing m_draw; //!< drawing tool.
			int32_t m_value;
			int32_t m_min;
			int32_t m_max;
			etk::Color<> m_textColorFg; //!< Text color
			etk::Color<> m_textColorBg; //!< Background color
		public: // Derived function
			virtual void onDraw(void);
			virtual void calculateMinMaxSize(void);
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
	};
	
};

#endif
