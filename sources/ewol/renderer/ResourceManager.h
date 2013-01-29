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
#include <ewol/renderer/resources/Resource.h>
#include <ewol/renderer/resources/Shader.h>
#include <ewol/renderer/resources/Program.h>
#include <ewol/renderer/resources/VirtualBufferObject.h>
#include <ewol/renderer/resources/ConfigFile.h>
#include <ewol/renderer/resources/TexturedFont.h>
#include <ewol/renderer/resources/Texture.h>
#include <ewol/renderer/resources/Image.h>
#include <ewol/renderer/resources/MeshObj.h>
#include <ewol/renderer/resources/Colored3DObject.h>

namespace ewol
{
	namespace resource {
		/**
		 * @brief Initialize the internal variable
		 */
		void Init(void);
		/**
		 * @brief Uninitiamize the resource manager, free all resources previously requested
		 * @note when not free ==> generate warning, because the segfault can appear after...
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
		 * @param[in] object The resources that might be updated
		 */
		void Update(ewol::Resource* object);
		/**
		 * @brief Call by the system chen the openGL Context has been unexpectially removed ==> This reload all the texture, VBO and other ....
		 */
		void UpdateContext(void);
		/**
		 * @brief This is to inform the resources manager that we have no more openGl context ...
		 */
		void ContextHasBeenDestroyed(void);
		
		/**
		 * @brief Load the specify resources type
		 * @param[in] filename The filename of the resources
		 * @param[in,out] object The resources that might be instanciate.
		 * @return true if the resource has been loaded corectly.
		 * @return false An error occured ...
		 * @note when you call the Keep function, you must call the Realease function ==> otherwise the resources will never be freed
		 * @note The resources with the same name are loaded only one time, a counter prevent multiple loading...
		 */
		bool Keep(const etk::UString& filename, ewol::TexturedFont*& object);
		bool Keep(const etk::UString& filename, ewol::FontBase*& object);
		bool Keep(const etk::UString& filename, ewol::Program*& object);
		bool Keep(const etk::UString& filename, ewol::Shader*& object);
		bool Keep(ewol::Texture*& object); // no name needed here ...
		bool Keep(const etk::UString& filename, ewol::TextureFile*& object, ivec2 size);
		bool Keep(const etk::UString& accesMode, ewol::VirtualBufferObject*& object);
		bool Keep(const etk::UString& filename, ewol::MeshObj*& object);
		bool Keep(const etk::UString& filename, ewol::ConfigFile*& object);
		bool Keep(ewol::Colored3DObject*& object);
		
		/**
		 * @brief Release a resources and free it if the Last release is call.
		 * @param[in,out] object element to realease ==> is return at NULL value.
		 */
		void Release(ewol::Resource*& object);
		void Release(ewol::TexturedFont*& object);
		void Release(ewol::FontBase*& object);
		void Release(ewol::Program*& object);
		void Release(ewol::Shader*& object);
		void Release(ewol::Texture*& object);
		void Release(ewol::TextureFile*& object);
		void Release(ewol::VirtualBufferObject*& object);
		void Release(ewol::MeshObj*& object);
		void Release(ewol::ConfigFile*& object);
		void Release(ewol::Colored3DObject*& object);
	}
};


#endif

