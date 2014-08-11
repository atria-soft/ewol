/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#ifndef __EWOL_PARAMETER_LIST_H__
#define __EWOL_PARAMETER_LIST_H__

#include <vector>

namespace ewol {
	namespace object {
		class Parameter;
		class ParameterList {
			friend class ewol::object::Parameter; // to register parameter in the list.
			private:
				std::vector<ewol::object::Parameter*> m_list;  //!< list of availlable Parameters
			public:
				/**
				 * @brief Constructor.
				 */
				ParameterList();
				/**
				 * @brief Destructor.
				 */
				~ParameterList();
				/**
				 * @brief Register a parameter class pointer in the List of parameters
				 * @note This class does not destroy the parameter pointer!!!
				 * @param[in] pointerOnParameter Pointer on the parameter that might be added.
				 */
				void parameterAdd(Parameter* _pointerOnParameter);
				/**
				 * @brief Remove all the parameter reference in this class. 
				 * @note no delete, just clean and inform that a parameter has not been removed.
				 */
				void parameterClean();
				/**
				 * @brief Set a specific value to the parameter reference name.
				 * @param[in] parameter The parameter string name.
				 * @param[in] value The new value of the parameter (string).
				 * @return true Parameter update.
				 * @return false Parameter not update.
				 */
				bool parameterSet(const std::string& _parameter, const std::string& _value);
				/**
				 * @brief Get a specific value of the parameter reference name.
				 * @param[in] parameter The parameter string name.
				 * @return The value of the parameter (string).
				 */
				std::string parameterGet(const std::string& _parameter);
				/**
				 * @brief Display all the parameter value with there name.
				 * @param[in] changeOnly check at true if the user want to display only parameter that are not at default value.
				 */
				void parameterDisplay(bool _changeOnly = false);
		};
	};
};

#endif
