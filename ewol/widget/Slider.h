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
#include <esignal/Signal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Slider : public ewol::Widget {
			public:
				// Event list of properties
				esignal::Signal<float> signalChange;
			protected:
				Slider();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Slider, "Slider");
				virtual ~Slider();
			protected:
				eproperty::Value<float> m_value; //!< current value of the Slider
			public:
				/**
				 * @brief Set the value of the slider.
				 * @param[in] _val New value to apply.
				 */
				void setValue(float _val) {
					m_value.set(_val);
				}
				/**
				 * @brief Get the value of the slider.
				 * @return The current value of the slider.
				 */
				float getValue() {
					return m_value;
				}
			protected:
				eproperty::Value<float> m_min; //!< minimum value of the slider
			public:
				/**
				 * @brief Set the minumum value of the slider.
				 * @param[in] _val New minimum value to apply.
				 */
				void setMin(float _val) {
					m_min.set(_val);
				}
				/**
				 * @brief Get the minimum value of the slider.
				 * @return The minimum value of the slider.
				 */
				float getMin() {
					return m_min;
				}
			protected:
				eproperty::Value<float> m_max; //!< maximum value of the slider
			public:
				/**
				 * @brief Set the maximum value of the slider.
				 * @param[in] _val New maximum value to apply.
				 */
				void setMax(float _val) {
					m_max.set(_val);
				}
				/**
				 * @brief Get the maximum value of the slider.
				 * @return The maximum value of the slider.
				 */
				float getMax() {
					return m_max;
				}
			protected:
				eproperty::Value<float> m_step;
			public:
				/**
				 * @brief Set the step value of the slider.
				 * @param[in] _val New step value to apply.
				 */
				void setStep(float _val) {
					m_step.set(_val);
				}
				/**
				 * @brief Get the step value of the slider.
				 * @return The wtep value of the slider.
				 */
				float getStep() {
					return m_step;
				}
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
				virtual void onDraw();
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
		};
	}
}

