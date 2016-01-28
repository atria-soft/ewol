/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_PARAMETER_VALUE_H__
#define __EWOL_PARAMETER_VALUE_H__

#include <ewol/parameter/Interface.h>
#include <ewol/parameter/Parameter.h>
#include <etk/math/Vector2D.h>


namespace ewol {
	namespace parameter {
		template<typename MY_TYPE, bool isEventReceiving=false> class Value : public Parameter {
			private:
				MY_TYPE m_value; //!< Current value.
				MY_TYPE m_default; //!< Default value.
			public:
				/**
				 * @brief Create a parameter with a specific type.
				 * @param[in] _objectLink reference on the parameter lister.
				 * @param[in] _name Static name of the parameter.
				 * @param[in] _defaultValue Default value of the parameter.
				 * @param[in] _min Minumum value.
				 * @param[in] _max Maximum value.
				 * @param[in] _description description of the parameter.
				 */
				Value(ewol::parameter::Interface& _paramInterfaceLink,
				      const std::string& _name,
				      const MY_TYPE& _defaultValue,
				      const std::string& _description = "") :
				  Parameter(_paramInterfaceLink, _name),
				  m_value(_defaultValue),
				  m_default(_defaultValue) {
					
				};
				Value(ewol::parameter::Interface& _paramListLink,
				      const std::string& _name,
				      const std::string& _description = "") :
				  Parameter(_paramListLink, _name),
				  m_value(),
				  m_default() {
					
				};
				/**
				 * @brief Destructor.
				 */
				virtual ~Value() { };
				// herited methode
				virtual std::string getParameterType() const {
					return "ewol::parameter::Value";
				}
				// herited methode
				virtual std::string getType() const {
					return typeid(MY_TYPE).name();
				}
				// herited methode
				virtual std::string getString() const {
					return getValueSpecific(m_value);
				}
				// herited methode
				virtual std::string getDefault() const {
					return getValueSpecific(m_default);
				}
				// herited methode
				virtual void setString(const std::string& _newVal) {
					// when you want to set an element in parameter you will implement the function template std::from_string
					etk::from_string(m_value, _newVal);
					// TODO : Do it better ...
					notifyChange();
				}
				// herited methode
				virtual std::string getInfo() const {
					return getType() + " default=" + getDefault();
				}
				// herited methode
				virtual bool isDefault() const {
					return m_value == m_default;
				}
				// herited methode
				virtual void setDefault() {
					set(m_default);
				}
			public:
				/**
				 * @brief Get the value of the current parameter.
				 * @note For performence, this function must be inline
				 * @return the Reference value
				 */
				inline MY_TYPE& get() {
					return m_value;
				};
				const inline MY_TYPE& get() const {
					return m_value;
				};
				/**
				 * @brief Set a new value for this parameter
				 * @param[in] newVal New value to set (set the nearest value if range is set)
				 */
				void set(const MY_TYPE& _newVal) {
					if (_newVal != m_value) {
						m_value = _newVal;
						notifyChange();
					}
				}
			private:
				/**
				 * @brief Get the string of the specify value.
				 * @return convetion of the velue in string.
				 */
				std::string getValueSpecific(const MY_TYPE& _valueRequested) const {
					return etk::to_string(_valueRequested);
				}
			public:
				/**
				 * @brief assignement operator.
				 * @param[in] newVal The new value of the parameter.
				 */
				const Value<MY_TYPE>& operator= (const MY_TYPE& _newVal) {
					set(_newVal);
					return *this;
				};
				operator const MY_TYPE&() const {
					return m_value;
				}
				MY_TYPE& operator *() const noexcept {
					return m_value;
				}
				const MY_TYPE* operator->() const noexcept {
					return &m_value;
				}
				MY_TYPE* operator->() noexcept {
					return &m_value;
				}
		};
		
		template<typename MY_TYPE, bool isEventReceiving=false> std::ostream& operator <<(std::ostream& _os, const ewol::parameter::Value<MY_TYPE, isEventReceiving>& _obj) {
			_os << _obj.get();
			return _os;
		}
	};
};
#endif
