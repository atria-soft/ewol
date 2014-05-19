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

namespace ewol {
	namespace object {
		class Manager {
			private:
				std::vector<ewol::object::Shared<ewol::Object>> m_eObjectList; // all widget allocated  == > all time increment ... never removed ...
				std::vector<ewol::object::Shared<ewol::Object>> m_eObjectAutoRemoveList; // all widget allocated
			public:
				Manager();
				~Manager();
				/**
				 * @brief remove all resources (un-init) out of the destructor (due to the system implementation)
				 */
				void unInit();
				
				void add(const ewol::object::Shared<ewol::Object>& _object);
				void rm(const ewol::object::Shared<ewol::Object>& _object);
				void addOwned(const ewol::object::Shared<ewol::Object>& _object);
				void rmOwned(const ewol::object::Shared<ewol::Object>& _object);
				int32_t getNumberObject();
				
				void autoRemove(const ewol::object::Shared<ewol::Object>& _object);
				void autoRespown(const ewol::object::Shared<ewol::Object>& _object);
				void removeAllAutoRemove();
				
				ewol::object::Shared<ewol::Object> get(const std::string& _name);
			private:
				void informOneObjectIsRemoved(const ewol::object::Shared<ewol::Object>& _object);
			private:
				ewol::object::MultiCast m_multiCast; //!< muticast manager
			public:
				ewol::object::MultiCast& multiCast() {
					return m_multiCast;
				};
		};
	};
};

#endif

