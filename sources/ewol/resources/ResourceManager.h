/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resources/Resource.h>
#include <ewol/resources/Shader.h>
#include <ewol/resources/Program.h>
#include <ewol/resources/VirtualBufferObject.h>
#include <ewol/resources/ConfigFile.h>
#include <ewol/resources/TexturedFont.h>
#include <ewol/resources/Texture.h>
#include <ewol/resources/Image.h>
#include <ewol/resources/Mesh.h>
#include <ewol/resources/Colored3DObject.h>

namespace ewol
{
	class ResourceManager
	{
		private:
			etk::Vector<ewol::Resource*> m_resourceList;
			etk::Vector<ewol::Resource*> m_resourceListToUpdate;
			bool m_contextHasBeenRemoved;
		public:
			/**
			 * @brief Initialize the internal variable
			 */
			ResourceManager(void);
			/**
			 * @brief Uninitiamize the resource manager, free all resources previously requested
			 * @note when not free ==> generate warning, because the segfault can appear after...
			 */
			~ResourceManager(void);
			/**
			 * @brief remove all resources (un-init) out of the destructor (due to the system implementation)
			 */
			void UnInit(void);
			/**
			 * @brief Display in the log all the resources loaded ...
			 */
			void Display(void);
			/**
			 * @brief Reload all resources from files, and send there in openGL card if needed.
			 * @note If File is reference at THEME:XXX:filename if the Theme change the file will reload the newOne
			 */
			void ReLoadResources(void);
			/**
			 * @brief Call by the system to send all the needed data on the graphic card chen they change ...
			 * @param[in] _object The resources that might be updated
			 */
			void Update(ewol::Resource* _object);
			/**
			 * @brief Call by the system chen the openGL Context has been unexpectially removed ==> This reload all the texture, VBO and other ....
			 */
			void UpdateContext(void);
			/**
			 * @brief This is to inform the resources manager that we have no more openGl context ...
			 */
			void ContextHasBeenDestroyed(void);
		public:
			// internal API to extent eResources in extern Soft
			ewol::Resource* LocalKeep(const etk::UString& _filename);
			void LocalAdd(ewol::Resource* _object);
		public:
			/**
			 * @brief Release a resources and free it if the Last release is call.
			 * @param[in,out] _object element to realease ==> is return at NULL value.
			 */
			void Release(ewol::Resource*& _object);
	};
};


#endif

