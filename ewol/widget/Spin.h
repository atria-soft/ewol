/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <etk/types.h>
#include <ewol/widget/meta/SpinBase.h>

namespace ewol {
	namespace widget {
		/**
		 * @brief a composed Spin is a Spin with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Spin : public ewol::widget::SpinBase {
			public:
				// Event list of properties
				esignal::Signal<int64_t> signalValue;
				esignal::Signal<double> signalValueDouble;
			public:
				eproperty::Value<int64_t> propertyValue; //!< Current value of the Spin.
				eproperty::Value<int64_t> propertyMin; //!< Minimum value
				eproperty::Value<int64_t> propertyMax; //!< Maximum value
				eproperty::Value<int64_t> propertyIncrement; //!< Increment value
				eproperty::Value<int8_t> propertyMantis; //!< number of value under '.' value
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _mode mode to display the spin
				 * @param[in] _shaperName Shaper file properties
				 */
				Spin();
				void init(enum ewol::widget::spinPosition _mode=ewol::widget::spinPosition_RightRight,
				          const std::string& _shaperName="{ewol}THEME:GUI:Spin.json");
			public:
				DECLARE_WIDGET_FACTORY(Spin, "Spin");
				/**
				 * @brief Destructor
				 */
				virtual ~Spin();
			protected:
				virtual void checkValue(int64_t _value);
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
				virtual void updateGui();
			protected:
				void onCallbackUp();
				void onCallbackDown();
		};
	};
};
