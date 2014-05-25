/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__

#include <list>
#include <vector>
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resource/Resource.h>
#include <ewol/object/RemoveEvent.h>

namespace ewol {
	namespace resource {
		class Manager{
			private:
				std::list<ewol::object::Owner<ewol::Resource>> m_resourceList;
				std::vector<ewol::object::Shared<ewol::Resource>> m_resourceListToUpdate;
				bool m_contextHasBeenRemoved;
			public:
				/**
				 * @brief initialize the internal variable
				 */
				Manager();
				/**
				 * @brief Uninitiamize the resource manager, free all resources previously requested
				 * @note when not free  == > generate warning, because the segfault can appear after...
				 */
				~Manager();
				/**
				 * @brief remove all resources (un-init) out of the destructor (due to the system implementation)
				 */
				void unInit();
				/**
				 * @brief display in the log all the resources loaded ...
				 */
				void display();
				/**
				 * @brief Reload all resources from files, and send there in openGL card if needed.
				 * @note If file is reference at THEME:XXX:filename if the Theme change the file will reload the newOne
				 */
				void reLoadResources();
				/**
				 * @brief Call by the system to send all the needed data on the graphic card chen they change ...
				 * @param[in] _object The resources that might be updated
				 */
				void update(const ewol::object::Shared<ewol::Resource>& _object);
				/**
				 * @brief Call by the system chen the openGL Context has been unexpectially removed  == > This reload all the texture, VBO and other ....
				 */
				void updateContext();
				/**
				 * @brief This is to inform the resources manager that we have no more openGl context ...
				 */
				void contextHasBeenDestroyed();
			public:
				// internal API to extent eResources in extern Soft
				ewol::object::Shared<ewol::Resource> localKeep(const std::string& _filename);
				void localAdd(const ewol::object::Shared<ewol::Resource>& _object);
				virtual bool checkResourceToRemove();
		};
	};
};


#endif

