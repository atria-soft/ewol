/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#ifndef __EWOL_PARAMETER_INTERFACE_H__
#define __EWOL_PARAMETER_INTERFACE_H__

#include <vector>
#include <map>

namespace ewol {
	namespace parameter {
		class Parameter;
		class Ref;
		class Interface {
			friend class ewol::parameter::Parameter; // to register parameter in the list.
			private:
				std::vector<ewol::parameter::Parameter*> m_list;  //!< list of availlable Parameters (no need to free)
			public:
				/**
				 * @brief Constructor.
				 */
				Interface();
				/**
				 * @brief Destructor.
				 */
				virtual ~Interface();
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
				std::string parameterGet(const std::string& _parameter) const;
				/**
				 * @brief Display all the parameter value with there name.
				 * @param[in] changeOnly check at true if the user want to display only parameter that are not at default value.
				 */
				void parameterDisplay(bool _changeOnly = false) const;
				/**
				 * @brief Called when a parameter change value.
				 * @param[in] _paramPointer Pointer on the parameter (to know which parameter have change);
				 */
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) { };
				/**
				 * @brief Get All the parameter configuration:
				 * @return map on the parameters
				 */
				std::map<std::string, std::string> parameterGetAll(bool _notIfDefault=true) const;
			public:
				/**
				 * @brief Get count of parameters.
				 * @return The number of the parameter.
				 */
				size_t getParameterCount() const;
				/**
				 * @brief Get name of a parameters.
				 * @return _id Id of the parameter.
				 * @return pointer on the parameter.
				 */
				ewol::parameter::Parameter* getParameterRaw(const size_t& _id) const;
				
				
				
		};
	};
};

#endif
