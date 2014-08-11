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

namespace ewol {
	namespace object {
		class ParamList : public Parameter {
			private:
				int64_t m_value; //!< Element value ==> can be directly used.
				int64_t m_default; //!< Default value.
				std::map<std::string, int64_t> m_list; //!< pointer on the list of all elements.
			public:
				/**
				 * @brief Create a parameter with List of element parameter.
				 * @param[in] blockLink Pointer on the reference block (must be define for upper class).
				 * @param[in] name Static name of the parameter.
				 */
				ParamList(ewol::object::ParameterList& _objectLink,
				          const std::string& _name) :
				  Parameter(_objectLink, _name),
				  m_value(0),
				  m_default(0) {
					
				};
				/**
				 * @brief Destructor.
				 */
				virtual ~ParamList() {
					
				};
				void add(int64_t _value, const std::string& _name, const std::string& _description = "");
				// herited methode
				virtual std::string getType() {
					return "list";
				};
				// herited methode
				virtual std::string getString();
				// herited methode
				virtual std::string getDefault();
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
				/**
				 * @brief Get the value of the current parameter.
				 * @return the Reference value
				 */
				const inline int64_t& get() const {
					return m_value;
				};
				/**
				 * @brief Set the value of the current parameter.
				 * @param[in] _newVal New value of the parameter. (not set if out of range)
				 */
				void set(int64_t _newVal);
			private:
				/**
				 * @brief Get the element description from real Value.
				 * @param[in] _intValue value that might be converted in string.
				 * @return the description string coresponding to this ID.
				 */
				std::string getElement(int64_t _intValue);
			public:
				/**
				 * @brief assignement operator.
				 * @param[in] newVal The new value of the parameter.
				 */
				const ParamList& operator= (int64_t _newVal) {
					set(_newVal);
					return *this;
				}
				operator int64_t() {
					return m_value;
				}
		};
		std::ostream& operator <<(std::ostream& _os, const ewol::object::ParamList& _obj);
	};
};
#endif

