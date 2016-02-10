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
				ewol::Signal<int64_t> signalValue;
				ewol::Signal<double> signalValueDouble;
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
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName) {
					//m_shaper.setString(_shaperName);
				}
			protected:
				ewol::parameter::Value<int64_t> m_value; //!< Current value of the Spin.
			public:
				/**
				 * @brief set the current value of the Spin
				 * @param[in] _val New value to set
				 */
				void setValue(int64_t _val);
				/**
				 * @brief get the current Spin value.
				 * @return The current spin value.
				 */
				int64_t getValue() const;
			protected:
				ewol::parameter::Value<int64_t> m_min; //!< Minimum value
			public:
				/**
				 * @brief set the minimum of the Spin
				 * @param[in] _val New minimum to set
				 */
				void setMinimum(int64_t _val);
				/**
				 * @brief get the current Spin minimum.
				 * @return The current spin minimum.
				 */
				int64_t getMinimum() const;
			protected:
				ewol::parameter::Value<int64_t> m_max; //!< Maximum value
			public:
				/**
				 * @brief set the maxnimum of the Spin
				 * @param[in] _val New maxnimum to set
				 */
				void setMaximum(int64_t _val);
				/**
				 * @brief get the current Spin maxnimum.
				 * @return The current spin maxnimum.
				 */
				int64_t getMaximum() const;
			protected:
				ewol::parameter::Value<int64_t> m_increment; //!< Increment value
			public:
				/**
				 * @brief set the increment value of the Spin
				 * @param[in] _val New increment to set
				 */
				void setIncrement(int64_t _val);
				/**
				 * @brief get the current Spin increment.
				 * @return The current spin increment.
				 */
				int64_t getIncrement() const;
			protected:
				ewol::parameter::Value<int8_t> m_mantis; //!< number of value under '.' value
			public:
				/**
				 * @brief set the mantis value of the Spin
				 * @param[in] _val New mantis to set
				 */
				void setMantis(int8_t _val);
				/**
				 * @brief get the current Spin mantis.
				 * @return The current spin mantis.
				 */
				int8_t getMantis() const;
			protected:
				virtual void checkValue(int64_t _value);
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
				virtual void updateGui();
			protected:
				void onCallbackUp();
				void onCallbackDown();
		};
	};
};
