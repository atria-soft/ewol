/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ProgressBar : public ewol::Widget {
			private:
				ewol::compositing::Drawing m_draw; // basic drawing element
			protected:
				ProgressBar();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ProgressBar, "ProgressBar");
				virtual ~ProgressBar();
				void setValue(float _val);
				float getValue() const {
					return m_value;
				};
				void setColor(etk::Color<> _newColor) {
					m_textColorFg = _newColor;
				};
			private:
				eproperty::Value<float> m_value; //!< % used
				eproperty::Value<etk::Color<>> m_textColorFg; //!< forder bar color
				eproperty::Value<etk::Color<>> m_textColorBgOn; //!< bar color enable
				eproperty::Value<etk::Color<>> m_textColorBgOff; //!< bar color disable
			protected: // Derived function
				virtual void onDraw();
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual void calculateMinMaxSize();
		};
	};
};

