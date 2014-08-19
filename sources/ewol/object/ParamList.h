/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_PARAM_LIST_H__
#define __EWOL_PARAM_LIST_H__

#include <ewol/object/ParameterList.h>
#include <ewol/object/Parameter.h>
#include <map>
#include <typeinfo>

namespace ewol {
	namespace object {
		template<typename MY_TYPE, bool isEventReceiving=false> class ParamList : public Parameter {
			private:
				MY_TYPE m_value; //!< Element value ==> can be directly used.
				MY_TYPE m_default; //!< Default value.
				std::map<std::string, MY_TYPE> m_list; //!< pointer on the list of all elements.
			public:
				/**
				 * @brief Create a parameter with List of element parameter.
				 * @param[in] _objectLink reference on the parameter lister.
				 * @param[in] _name Static name of the parameter.
				 * @param[in] _description description of the parameter.
				 */
				ParamList(ewol::object::ParameterList& _objectLink,
				          const std::string& _name,
				          const MY_TYPE& _defaultValue,
				          const std::string& _description="") :
				  Parameter(_objectLink, _name),
				  m_value(_defaultValue),
				  m_default(_defaultValue) {
					
				};
				/**
				 * @brief Destructor.
				 */
				virtual ~ParamList() {
					
				};
				void add(const MY_TYPE& _value, const std::string& _name, const std::string& _description = "") {
					auto it = m_list.find(_name);
					if (it != m_list.end()) {
						it->second = _value;
						return;
					}
					m_list.insert(std::make_pair(_name, _value));
				}
				// herited methode
				virtual std::string getType() const {
					return typeid(MY_TYPE).name();
				}
				// herited methode
				virtual std::string getString() const {
					return getElement(m_value);
				}
				// herited methode
				virtual std::string getDefault() const {
					return getElement(m_default);
				}
				// herited methode
				virtual void setString(const std::string& _newVal) {
					auto it = m_list.find(_newVal);
					if (it != m_list.end()) {
						if (it->second != m_value) {
							m_value = it->second;
							notifyChange();
						}
						return;
					}
					EWOL_WARNING("paramList value='" << _newVal << "' is not un the list ... ==> no change");
					for (auto &it : m_list) {
						EWOL_VERBOSE("        element : " << it.first);
					}
				}
				// herited methode
				virtual std::string getInfo() const {
					std::string list = "List default=" + getElement(m_default) + " in : [";
					for (auto &it : m_list) {
						list += it.first + "/";
					}
					return list + "]";
				}
				// herited methode
				virtual bool isDefault() const {
					return m_value == m_default;
				}
				// herited methode
				virtual void setDefault() {
					set(m_default);
				}
				void setDefaultValue(const MY_TYPE& _value) {
					m_default = _value;
				}
				/**
				 * @brief Get the value of the current parameter.
				 * @return the Reference value
				 */
				inline MY_TYPE& get() {
					return m_value;
				};
				const inline MY_TYPE& get() const {
					return m_value;
				};
				/**
				 * @brief Set the value of the current parameter.
				 * @param[in] _newVal New value of the parameter. (not set if out of range)
				 */
				void set(MY_TYPE _newVal) {
					if (_newVal == m_value) {
						return;
					}
					for (auto &it : m_list) {
						if (it.second == _newVal) {
							m_value = it.second;
							notifyChange();
							return;
						}
					}
					EWOL_WARNING("paramList value=??? is not un the list ... ==> no change");
				}
			private:
				/**
				 * @brief Get the element description from real Value.
				 * @param[in] _intValue value that might be converted in string.
				 * @return the description string coresponding to this ID.
				 */
				std::string getElement(MY_TYPE _intValue) const {
					for (auto &it : m_list) {
						if (it.second == _intValue) {
							return it.first;
						}
					}
					return "???";
				}
			public:
				/**
				 * @brief assignement operator.
				 * @param[in] newVal The new value of the parameter.
				 */
				const ParamList& operator= (MY_TYPE _newVal) {
					set(_newVal);
					return *this;
				}
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
		template<typename MY_TYPE> std::ostream& operator <<(std::ostream& _os, const ewol::object::ParamList<MY_TYPE>& _obj) {
			_os << _obj.get();
			return _os;
		}
	};
};
#endif

