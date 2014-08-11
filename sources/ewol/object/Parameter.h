/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/ParameterList.h>

#ifndef __EWOL_PARAMETER_H__
#define __EWOL_PARAMETER_H__

#include <string>

namespace ewol {
	namespace object {
		class Parameter {
			private:
				std::string m_name;
			public:
				Parameter(ewol::object::ParameterList& _objectLink, const std::string& _name);
				virtual ~Parameter() { };
				/**
				 * @brief Get the name of the parameter.
				 * @return The name of the parameter
				 */
				virtual std::string getName() {
					return m_name;
				};
				/**
				 * @brief Get the type of the parameter in string mode.
				 * @return The string type of the parameter.
				 */
				virtual std::string getType() = 0;
				/**
				 * @brief Get the string of the current value of the parameter.
				 * @return The string description of the value.
				 */
				virtual std::string getString() = 0;
				/**
				 * @brief Get the string of the default value of the parameter.
				 * @return the string decription of the default value.
				 */
				virtual std::string getDefault() = 0;
				/**
				 * @brief Set a new value of the parameter (with string interface).
				 * @param[in] _newVal New value of the parameters.
				 */
				virtual void setString(const std::string& _newVal) = 0;
				/**
				 * @brief Description of the parameters.
				 * @return Descriptive information of the parameter (for remote UI).
				 */
				virtual std::string getInfo() = 0;
				/**
				 * @brief Check if the value is the default
				 * @return true : the vakue is the default one, false otherwise.
				 */
				virtual bool isDefault() = 0;
				/**
				 * @brief Reset the value to the default value.
				 */
				virtual void setDefault() = 0;
		};
	};
};

#endif
