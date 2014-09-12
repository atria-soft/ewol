/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_OBJECT_MANAGER_H__
#define __EWOL_OBJECT_MANAGER_H__

#include <etk/types.h>
#include <ewol/object/Object.h>

namespace ewol {
	class Context;
	namespace object {
		class Manager {
			private:
				std::vector<std::weak_ptr<ewol::Object>> m_eObjectList; // all widget allocated  == > all time increment ... never removed ...
				Context& m_context;
			public:
				Manager(Context& _context);
				virtual ~Manager();
				/**
				 * @brief remove all resources (un-init) out of the destructor (due to the system implementation)
				 */
				void unInit();
				/**
				 * @brief Get the number of loaded object in the system
				 * @return number of Object
				 */
				int32_t getNumberObject();
				void displayListObject();
			private:
				friend class ewol::Object;
				/**
				 * @brief Internal API that used only with Object toi reference itself in the manager.
				 * @note The manager remove the object when the refecence Low down 1 (last keeper)
				 * @param[in] _object Reference shared pointer on the object
				 */
				void add(const std::shared_ptr<ewol::Object>& _object);
			public:
				void cleanInternalRemoved();
				
				std::shared_ptr<ewol::Object> get(const std::string& _name);
			
			public:
				/**
				 * @brief retrive an object with his name
				 * @param[in] _name Name of the object
				 * @return the requested object or nullptr
				 */
				std::shared_ptr<ewol::Object> getObjectNamed(const std::string& _name);
			private:
				std::vector<std::shared_ptr<ewol::Object>> m_workerList;
			public:
				/**
				 * @brief Add a worker on the system list.
				 * @param[in] _worker Worker to add in the list.
				 */
				void workerAdd(const std::shared_ptr<ewol::Object>& _worker);
				/**
				 * @brief Remove a worker on the system list.
				 * @param[in] _worker Worker to add in the list.
				 */
				void workerRemove(const std::shared_ptr<ewol::Object>& _worker);
		};
	};
};

#endif

