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

namespace ewol
{
	namespace resource {
		void Init(void);
		void UnInit(void);
		void Display(void);
		
		// specific for the test ...
		void ReLoadResources(void);
		
		void Update(ewol::Resource* object);
		// Specific to load or update the data in the openGl context ==> system use only
		void UpdateContext(void);
		void ContextHasBeenDestroyed(void);
		
		// return the type of the resource ...
		bool Keep(etk::UString& filename, ewol::TexturedFont*& object);
		bool Keep(etk::UString& filename, ewol::FontBase*& object);
		bool Keep(etk::UString& filename, ewol::Program*& object);
		bool Keep(etk::UString& filename, ewol::Shader*& object);
		bool Keep(ewol::Texture*& object); // no name needed here ...
		bool Keep(etk::UString& filename, ewol::TextureFile*& object, ivec2 size);
		bool Keep(etk::UString& accesMode, ewol::VirtualBufferObject*& object);
		bool Keep(etk::UString& filename, ewol::MeshObj*& object);
		bool Keep(etk::UString& filename, ewol::ConfigFile*& object);
		
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
	}
};


#endif

