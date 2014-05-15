/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __OPEN_GL__SHADER_H__
#define __OPEN_GL__SHADER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/openGL/openGL.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace resource {
		/**
		 * @brief Shader is a specific resources for opengl, used only in @ref Program. This are components of the renderer pipe-line
		 */
		class Shader : public ewol::Resource {
			private :
				bool m_exist; //!< The shader file existed and has been loaded
				char* m_fileData; //!< A copy of the data loaded from the file (usefull only when opengl context is removed)
				GLuint m_shader; //!< opengl id of this element
				GLenum m_type; //!< Type of the current shader(vertex/fragment)
			protected:
				/**
				 * @brief Contructor of an opengl Shader
				 * @param[in] filename Standard file name format. see @ref etk::FSNode
				 */
				Shader(const std::string& _filename);
				/**
				 * @brief Destructor, remove the current Shader
				 */
				virtual ~Shader();
			public:
				/**
				 * @brief get the opengl reference id of this shader.
				 * @return The opengl id.
				 */
				GLuint getGL_ID() {
					return m_shader;
				};
				/**
				 * @brief get the opengl type of this shader.
				 * @return The type of this loaded shader.
				 */
				GLenum getShaderType() {
					return m_type;
				};
				/**
				 * @brief This load/reload the data in the opengl context, needed when removed previously.
				 */
				void updateContext();
				/**
				 * @brief remove the data from the opengl context.
				 */
				void removeContext();
				/**
				 * @brief Special android spec! It inform us that all context is removed and after notify us...
				 */
				void removeContextToLate();
				/**
				 * @brief Relode the shader from the file. used when a request of resouces reload is done.
				 * @note this is really usefull when we tested the new themes or shader developpements.
				 */
				void reload();
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the openGL Shader.
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ewol::resource::Shader* keep(const std::string& _filename);
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::Shader*& _object);
		};
	};
};

#endif

