/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Resource.h>
#include <ewol/openGL/Shader.h>
#include <ewol/openGL/Program.h>
#include <ewol/openGL/VirtualBufferObject.h>
#include <ewol/SimpleConfigFile.h>
#include <ewol/font/Font.h>
#include <ewol/font/TexturedFont.h>
#include <ewol/font/DistantFieldFont.h>
#include <ewol/texture/Texture.h>
#include <ewol/texture/TextureFile.h>
#include <ewol/Mesh/MeshObj.h>

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
		bool Keep(etk::UString& filename, ewol::Font*& object);
		bool Keep(etk::UString& filename, ewol::Program*& object);
		bool Keep(etk::UString& filename, ewol::Shader*& object);
		bool Keep(etk::UString& filename, ewol::DistantFieldFont*& object);
		bool Keep(ewol::Texture*& object); // no name needed here ...
		bool Keep(etk::UString& filename, ewol::TextureFile*& object, etk::Vector2D<int32_t> size);
		bool Keep(etk::UString& accesMode, ewol::VirtualBufferObject*& object);
		bool Keep(etk::UString& filename, ewol::MeshObj*& object);
		bool Keep(etk::UString& filename, ewol::SimpleConfigFile*& object);
		
		void Release(ewol::Resource*& object);
		void Release(ewol::TexturedFont*& object);
		void Release(ewol::Font*& object);
		void Release(ewol::Program*& object);
		void Release(ewol::Shader*& object);
		void Release(ewol::DistantFieldFont*& object);
		void Release(ewol::Texture*& object);
		void Release(ewol::TextureFile*& object);
		void Release(ewol::VirtualBufferObject*& object);
		void Release(ewol::MeshObj*& object);
		void Release(ewol::SimpleConfigFile*& object);
	}
};


#endif

