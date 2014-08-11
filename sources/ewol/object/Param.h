/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_PARAM_TYPE_H__
#define __EWOL_PARAM_TYPE_H__

#include <ewol/object/ParameterList.h>
#include <ewol/object/Parameter.h>
#include <etk/math/Vector2D.h>


namespace ewol {
	namespace object {
		template<typename MY_TYPE> class Param : public Parameter {
			private:
				MY_TYPE m_value; //!< Current value.
				MY_TYPE m_min; //!< Minimum value.
				MY_TYPE m_max; //!< Maximum value.
				MY_TYPE m_default; //!< Default value.
			public:
				/**
				 * @brief Create a parameter with a specific type.
				 * @param[in] blockLink Pointer on the reference block (must be define for upper class.
				 * @param[in] name Static name of the parameter.
				 * @param[in] defaultValue Default value of the parameter.
				 * @param[in] min Minumum value.
				 * @param[in] max Maximum value
				 */
				template<typename TYPE, typename = typename
				         std::enable_if<    std::is_same<std::string, TYPE>::value
				                         || std::is_same<char[1], TYPE>::value
				                         || std::is_same<bvec2, TYPE>::value
				                         || std::is_same<bvec3, TYPE>::value
				                         || std::is_same<vec2, TYPE>::value
				                         || std::is_same<vec3, TYPE>::value
				                         || std::is_same<ivec2, TYPE>::value
				                         || std::is_same<ivec3, TYPE>::value
				                         || std::is_same<uivec2, TYPE>::value
				                         || std::is_same<uivec3, TYPE>::value>::type>
				Param(ewol::object::ParameterList& _objectLink,
				      const std::string& _name,
				      const TYPE& _defaultValue) :
				  Parameter(_objectLink, _name),
				  m_value(_defaultValue),
				  m_default(_defaultValue) {
					
				};
				template<typename TYPE, typename = typename std::enable_if<!std::is_same<std::string, TYPE>::value>::type>
				Param(ewol::object::ParameterList& _objectLink,
				      const std::string& _name,
				      const TYPE& _defaultValue,
				      const TYPE& _min,
				      const TYPE& _max) :
				  Parameter(_objectLink, _name),
				  m_value(_defaultValue),
				  m_min(_min),
				  m_max(_max),
				  m_default(_defaultValue) {
					
				};
				/**
				 * @brief Destructor.
				 */
				virtual ~Param() { };
				// herited methode
				virtual std::string getType();
				// herited methode
				virtual std::string getString() {
					return getValueSpecific(m_value);
				};
				// herited methode
				virtual std::string getDefault() {
					return getValueSpecific(m_default);
				};
				// herited methode
				virtual void setString(const std::string& _newVal);
				// herited methode
				virtual std::string getInfo();
				// herited methode
				virtual bool isDefault() {
					return m_value == m_default;
				}
				// herited methode
				virtual void setDefault() {
					m_value = m_default;
				}
			public:
				/**
				 * @brief Get the value of the current parameter.
				 * @note For performence, this function must be inline
				 * @return the Reference value
				 */
				const inline MY_TYPE& get() const {
					return m_value;
				};
				/**
				 * @brief Set a new value for this parameter
				 * @param[in] newVal New value to set (set the nearest value if range is set)
				 */
				void set(const MY_TYPE& _newVal);
			private:
				/**
				 * @brief Get the string of the specify value.
				 * @return convetion of the velue in string.
				 */
				std::string getValueSpecific(const MY_TYPE& _valueRequested);
			public:
				/**
				 * @brief assignement operator.
				 * @param[in] newVal The new value of the parameter.
				 */
				const Param<MY_TYPE>& operator= (const MY_TYPE& _newVal) {
					set(_newVal);
					return *this;
				};
				operator MY_TYPE() {
					return m_value;
				}
		};
		template<> std::string Param<bool>::getType() {
			return "bool";
		}
		template<> std::string Param<std::string>::getType() {
			return "string";
		}
		template<>
		std::string Param<bool>::getInfo() {
			return getType() + " default=" + (m_default?"true":"false");
		}
		template<>
		std::string Param<std::string>::getInfo() {
			return getType() + " default=" + m_default;
		}
		template<typename MY_TYPE>
		std::string Param<MY_TYPE>::getInfo() {
			return getType() + " default= (heterogen type)";
			/*
			if (m_min == m_max) {
				return getType() + " default=" + std::to_string(m_default);
			}
			return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
			*/
		}
		
		template<>
		void Param<std::string>::set(const std::string& _newVal) {
			m_value = _newVal;
		};
		template<typename MY_TYPE>
		void Param<MY_TYPE>::set(const MY_TYPE& _newVal) {
			if (m_min == m_max) {
				m_value = _newVal;
			} else {
				m_value = std::avg(m_min, _newVal, m_max);
			}
		};
		
		template<typename MY_TYPE> std::ostream& operator <<(std::ostream& _os, const ewol::object::Param<MY_TYPE>& _obj) {
			_os << _obj.get();
			return _os;
		}
	};
};
#endif
