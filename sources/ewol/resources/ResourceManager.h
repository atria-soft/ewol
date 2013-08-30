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
		private:
			// internal API to extent eResources in extern Soft
			ewol::Resource* LocalKeep(const etk::UString& filename);
			void LocalAdd(ewol::Resource* object);
		public:
			/**
			 * @brief Load the specify resources type
			 * @param[in] filename The filename of the resources
			 * @param[in,out] object The resources that might be instanciate.
			 * @return true if the resource has been loaded corectly.
			 * @return false An error occured ...
			 * @note when you call the Keep function, you must call the Realease function ==> otherwise the resources will never be freed
			 * @note The resources with the same name are loaded only one time, a counter prevent multiple loading...
			 */
			static bool Keep(const etk::UString& filename, ewol::TexturedFont*& object);
			static bool Keep(const etk::UString& filename, ewol::FontBase*& object);
			static bool Keep(const etk::UString& filename, ewol::Program*& object);
			static bool Keep(const etk::UString& filename, ewol::Shader*& object);
			static bool Keep(ewol::Texture*& object); // no name needed here ...
			static bool Keep(const etk::UString& filename, ewol::TextureFile*& object, ivec2 size=ivec2(-1,-1));
			static bool Keep(const etk::UString& accesMode, ewol::VirtualBufferObject*& object);
			static bool Keep(const etk::UString& meshName, ewol::Mesh*& object);
			static bool Keep(const etk::UString& filename, ewol::ConfigFile*& object);
			static bool Keep(ewol::Colored3DObject*& object);
			
			// must became :
			/*
			ewol::Font* KeepFont(const etk::UString& _filename);
			ewol::Program* KeepProgram(const etk::UString& _filename);
			ewol::Shader* KeepShader(const etk::UString& _filename);
			ewol::Texture* KeepTexture(void);
			ewol::Texture* KeepTexture(const etk::UString& _filename, const ivec2& size=ivec2(-1,-1));
			void AddTextureResourceCreator(pf* _plop, const etk::UString& _ext);
			ewol::Audio* KeepAudio(const etk::UString& _filename, bool _inRam=false);
			void AddAudioResourceCreator(pf* _plop, const etk::UString& _ext);
			ewol::VirtualBufferObject* KeepVBO(const etk::UString& _accesMode);
			ewol::Mesh* KeepMesh(const etk::UString& _filename);
			ewol::ConfigFile* KeepConfigFile(const etk::UString& _filename);
			ewol::Colored3DObject* Keep3DObject(void);
			
			void Release(ewol::Resource*& object);
			*/
			/**
			 * @brief Release a resources and free it if the Last release is call.
			 * @param[in,out] object element to realease ==> is return at NULL value.
			 */
			void Release(ewol::Resource*& object);
			static void Release(ewol::TexturedFont*& object);
			static void Release(ewol::FontBase*& object);
			static void Release(ewol::Program*& object);
			static void Release(ewol::Shader*& object);
			static void Release(ewol::Texture*& object);
			static void Release(ewol::TextureFile*& object);
			static void Release(ewol::VirtualBufferObject*& object);
			static void Release(ewol::Mesh*& object);
			static void Release(ewol::ConfigFile*& object);
			static void Release(ewol::Colored3DObject*& object);
	};
};


#endif

