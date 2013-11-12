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
#include <ewol/renderer/openGL.h>
#include <ewol/resources/Resource.h>

namespace ewol {
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
			virtual ~Shader(void);
		public:
			/**
			 * @brief Generic function that get the resouces name of his type.
			 * @return The define char of his name.
			 */
			const char* getType(void) { return "ewol::Shader"; };
			/**
			 * @brief get the opengl reference id of this shader.
			 * @return The opengl id.
			 */
			GLuint getGL_ID(void) { return m_shader; };
			/**
			 * @brief get the opengl type of this shader.
			 * @return The type of this loaded shader.
			 */
			GLenum getShaderType(void) { return m_type; };
			/**
			 * @brief This load/reload the data in the opengl context, needed when removed previously.
			 */
			void updateContext(void);
			/**
			 * @brief remove the data from the opengl context.
			 */
			void removeContext(void);
			/**
			 * @brief Special android spec! It inform us that all context is removed and after notify us...
			 */
			void removeContextToLate(void);
			/**
			 * @brief Relode the shader from the file. used when a request of resouces reload is done.
			 * @note this is really usefull when we tested the new themes or shader developpements.
			 */
			void reload(void);
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the openGL Shader.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::Shader* keep(const std::string& _filename);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(ewol::Shader*& _object);
	};
};

#endif

