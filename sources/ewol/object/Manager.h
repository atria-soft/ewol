/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_OBJECT_MANAGER_H__
#define __EWOL_OBJECT_MANAGER_H__

#include <etk/types.h>
#include <ewol/object/Object.h>
#include <ewol/object/MultiCast.h>
#include <ewol/object/RemoveEvent.h>

namespace ewol {
	class Context;
	namespace object {
		class Manager {
			private:
				std::vector<ewol::object::RemoveEvent*> m_removeEventList;
				std::vector<ewol::object::Shared<ewol::Object>> m_eObjectList; // all widget allocated  == > all time increment ... never removed ...
				std::vector<ewol::object::Shared<ewol::Object>> m_eObjectListActive; // all active widget
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
				void add(const ewol::object::Shared<ewol::Object>& _object);
				/**
				 * @brief Called when an object request to be removed
				 * @param[in] _object Reference shared pointer on the object
				 */
				void remove(const ewol::object::Shared<ewol::Object>& _object);
				/**
				 * @brief Called when a user want to reuse an object that have been removed previously
				 * @param[in] _object Reference shared pointer on the object
				 */
				void respown(const ewol::object::Shared<ewol::Object>& _object);
			public:
				void removeAllRemovedObject();
				
				ewol::object::Shared<ewol::Object> get(const std::string& _name);
			private:
				void informOneObjectIsRemoved(const ewol::object::Shared<ewol::Object>& _object);
			private:
				ewol::object::MultiCast m_multiCast; //!< muticast manager
			public:
				ewol::object::MultiCast& multiCast() {
					return m_multiCast;
				};
				
				void add(ewol::object::RemoveEvent* _class);
				void rm(ewol::object::RemoveEvent* _class);
		};
	};
};

#endif

