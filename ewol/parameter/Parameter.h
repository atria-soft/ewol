/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/parameter/Interface.h>

#pragma once

#include <string>
#include <typeinfo>

namespace ewol {
	namespace parameter {
		class Ref;
		class Parameter {
			private:
				ewol::parameter::Interface& m_interfaceLink;
				std::string m_name;
			public:
				Parameter(ewol::parameter::Interface& _paramInterfaceLink, const std::string& _name);
				virtual ~Parameter() { };
				/**
				 * @brief call main class that parameterChange
				 */
				void notifyChange() const;
				/**
				 * @brief Get the name of the parameter.
				 * @return The name of the parameter
				 */
				virtual std::string getName() const {
					return m_name;
				};
				/**
				 * @brief Get the parameter type of the class in string mode.
				 * @return The string type of the parameter.
				 */
				virtual std::string getParameterType() const = 0;
				/**
				 * @brief Get the type of the parameter in string mode.
				 * @return The string type of the parameter.
				 */
				virtual std::string getType() const = 0;
				/**
				 * @brief Get the string of the current value of the parameter.
				 * @return The string description of the value.
				 */
				virtual std::string getString() const = 0;
				/**
				 * @brief Get the string of the default value of the parameter.
				 * @return the string decription of the default value.
				 */
				virtual std::string getDefault() const = 0;
				/**
				 * @brief Set a new value of the parameter (with string interface).
				 * @param[in] _newVal New value of the parameters.
				 */
				virtual void setString(const std::string& _newVal) = 0;
				/**
				 * @brief Description of the parameters.
				 * @return Descriptive information of the parameter (for remote UI).
				 */
				virtual std::string getInfo() const = 0;
				/**
				 * @brief Check if the value is the default
				 * @return true : the vakue is the default one, false otherwise.
				 */
				virtual bool isDefault() const = 0;
				/**
				 * @brief Reset the value to the default value.
				 */
				virtual void setDefault() = 0;
		};
		class Ref {
			public:
				const Parameter* m_ref;
				Ref(const Parameter* _ref) :
				  m_ref(_ref) {
					// nothing to do ...
				}
		};
		bool operator==(const Ref& _obj, const Parameter& _obj2) noexcept;
		bool operator==(const Parameter& _obj2, const Ref& _obj) noexcept;
	};
};

