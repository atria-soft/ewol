/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class Slider;
		using Slider = ememory::Ptr<ewol::widget::Slider>;
		using SliderWeak = ememory::WeakPtr<ewol::widget::Slider>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Slider : public Widget {
			public: // signals
				esignal::Signal<float> signalChange;
			public:
				//eproperty::Value<String> propertyShape; //!< name of the shape used
				eproperty::Value<float> propertyValue; //!< current value of the Slider
				eproperty::Value<float> propertyMinimum; //!< minimum value of the slider
				eproperty::Value<float> propertyMaximum; //!< maximum value of the slider
				eproperty::Value<float> propertyStep; //!< step of every iteration of the slider (increment/precision)
			protected:
				Slider();
			public:
				DECLARE_WIDGET_FACTORY(Slider, "Slider");
				 ~Slider();
			public:
				// TODO : Rewoek the color in the theme ...
				void setColor(etk::Color<> _newColor) {
					this.textColorFg = _newColor;
				};
			protected:
				ewol::compositing::Drawing this.draw; //!< drawing tool.
				etk::Color<> this.textColorFg; //!< Text color
				etk::Color<> this.textColorBg; //!< Background color
				void updateValue(float _newValue);
			public: // Derived function
				void onDraw() ;
				void calculateMinMaxSize() ;
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
			protected:
				 void onChangePropertyValue();
				 void onChangePropertyMinimum();
				 void onChangePropertyMaximum();
				 void onChangePropertyStep();
		};
	}
}

