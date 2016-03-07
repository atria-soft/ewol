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
		class ProgressBar;
		using ProgressBarShared = ewol::SharedPtr<ewol::widget::ProgressBar>;
		using ProgressBarWeak = ewol::WeakPtr<ewol::widget::ProgressBar>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ProgressBar : public ewol::Widget {
			public: // properties
				eproperty::Range<float> propertyValue; //!< % used
				eproperty::Value<etk::Color<>> propertyTextColorFg; //!< forder bar color
				eproperty::Value<etk::Color<>> propertyTextColorBgOn; //!< bar color enable
				eproperty::Value<etk::Color<>> propertyTextColorBgOff; //!< bar color disable
			protected:
				ProgressBar();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ProgressBar, "ProgressBar");
				virtual ~ProgressBar();
			private:
				ewol::compositing::Drawing m_draw; // basic drawing element
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual void calculateMinMaxSize();
			protected:
				virtual void onChangePropertyValue();
				virtual void onChangePropertyTextColorFg();
				virtual void onChangePropertyTextColorBgOn();
				virtual void onChangePropertyTextColorBgOff();
		};
	};
};

