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
	namespace object {
		class SignalBase;
		class SignalList {
			friend class ewol::object::SignalBase; // to register parameter in the list.
			private:
				std::vector<ewol::object::SignalBase*> m_list;  //!< list of availlable Parameters
			public:
				/**
				 * @brief Constructor.
				 */
				SignalList();
				/**
				 * @brief Destructor.
				 */
				virtual ~SignalList();
				/**
				 * @brief Register a parameter class pointer in the List of parameters
				 * @note This class does not destroy the parameter pointer!!!
				 * @param[in] pointerOnParameter Pointer on the parameter that might be added.
				 */
				void signalAdd(SignalBase* _pointerOnParameter);
				/**
				 * @brief Get All the signal list:
				 * @return vector on all the signals names
				 */
				std::vector<std::string> signalGetAll() const;
				/**
				 * @brief Remove binding on all event class.
				 * @param[in] _object Object to unlink.
				 */
				void unBindAll(const std::shared_ptr<ewol::Object>& _object);
		};
	};
};

#endif
