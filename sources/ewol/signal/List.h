/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#ifndef __EWOL_SIGNAL_LIST_H__
#define __EWOL_SIGNAL_LIST_H__

#include <vector>
#include <map>

namespace ewol {
	namespace signal {
		class Base;
		class List {
			friend class ewol::signal::Base; // to register parameter in the list.
			private:
				std::vector<ewol::signal::Base*> m_list;  //!< list of availlable Parameters
			public:
				/**
				 * @brief Constructor.
				 */
				List();
				/**
				 * @brief Destructor.
				 */
				~List();
				/**
				 * @brief Register a parameter class pointer in the List of parameters
				 * @note This class does not destroy the parameter pointer!!!
				 * @param[in] pointerOnParameter Pointer on the parameter that might be added.
				 */
				void signalAdd(ewol::signal::Base* _pointerOnParameter);
				/**
				 * @brief Get All the signal list:
				 * @return vector on all the signals names
				 */
				std::vector<std::string> signalGetAll() const;
				/**
				 * @brief Remove binding on all event class.
				 * @param[in] _sharedPtr sharedPtr to unlink (no type needed ...).
				 */
				void signalUnBindAll(const std::shared_ptr<void>& _sharedPtr);
		};
	};
};

#endif
