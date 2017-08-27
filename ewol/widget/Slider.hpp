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
		using SliderShared = ememory::SharedPtr<ewol::widget::Slider>;
		using SliderWeak = ememory::WeakPtr<ewol::widget::Slider>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Slider : public ewol::Widget {
			public: // signals
				esignal::Signal<float> signalChange;
			public:
				//eproperty::Value<etk::String> propertyShape; //!< name of the shape used
				eproperty::Value<float> propertyValue; //!< current value of the Slider
				eproperty::Value<float> propertyMinimum; //!< minimum value of the slider
				eproperty::Value<float> propertyMaximum; //!< maximum value of the slider
				eproperty::Value<float> propertyStep; //!< step of every iteration of the slider (increment/precision)
			protected:
				Slider();
			public:
				DECLARE_WIDGET_FACTORY(Slider, "Slider");
				virtual ~Slider();
			public:
				// TODO : Rewoek the color in the theme ...
				void setColor(etk::Color<> _newColor) {
					m_textColorFg = _newColor;
				};
			protected:
				ewol::compositing::Drawing m_draw; //!< drawing tool.
				etk::Color<> m_textColorFg; //!< Text color
				etk::Color<> m_textColorBg; //!< Background color
				void updateValue(float _newValue);
			public: // Derived function
				void onDraw() override;
				void calculateMinMaxSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
			protected:
				virtual void onChangePropertyValue();
				virtual void onChangePropertyMinimum();
				virtual void onChangePropertyMaximum();
				virtual void onChangePropertyStep();
		};
	}
}

