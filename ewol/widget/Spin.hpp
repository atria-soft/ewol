/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <etk/types.hpp>
#include <ewol/widget/meta/SpinBase.hpp>

namespace ewol {
	namespace widget {
		class Spin;
		using SpinShared = ememory::SharedPtr<ewol::widget::Spin>;
		using SpinWeak = ememory::WeakPtr<ewol::widget::Spin>;
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
			public:
				DECLARE_WIDGET_FACTORY(Spin, "Spin");
				/**
				 * @brief Destructor
				 */
				virtual ~Spin();
			protected:
				virtual void checkValue(int64_t _value);
				virtual void updateGui();
			protected:
				void onCallbackUp();
				void onCallbackDown();
			protected:
				esignal::Connection m_connectionEntry;
				esignal::Connection m_connectionButtonUp;
				esignal::Connection m_connectionButtonDown;
			protected:
				virtual void onChangePropertyValue();
				virtual void onChangePropertyMin();
				virtual void onChangePropertyMax();
				virtual void onChangePropertyIncrement();
				virtual void onChangePropertyMantis();
		};
	};
};
