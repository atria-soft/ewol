/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __OPEN_GL__SHADER_H__
#define __OPEN_GL__SHADER_H__
#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Resource.h>
#include <ewol/openGL/openGL.h>

namespace ewol
{
	/**
	 * @brief Shader is a specific resources for opengl, used only in @ref Program. This are components of the renderer pipe-line
	 */
	class Shader : public ewol::Resource
	{
		private :
			bool           m_exist;    //!< The shader file existed and has been loaded
			char*          m_fileData; //!< A copy of the data loaded from the file (usefull only when opengl context is removed)
			GLuint         m_shader;   //!< Opengl id of this element
			GLenum         m_type;     //!< Type of the current shader(vertex/fragment)
		public:
			/**
			 * @brief Contructor of an opengl Shader
			 * @param[in] filename Standard file name format. see @ref etk::FSNode
			 */
			Shader(etk::UString& filename);
			/**
			 * @brief Destructor, remove the current Shader
			 */
			virtual ~Shader(void);
			/**
			 * @brief Generic function that get the resouces name of his type.
			 * @return The define char of his name.
			 */
			const char* GetType(void) { return "ewol::Shader"; };
			/**
			 * @brief Get the opengl reference id of this shader.
			 * @return The opengl id.
			 */
			GLuint GetGL_ID(void) { return m_shader; };
			/**
			 * @brief Get the opengl type of this shader.
			 * @return The type of this loaded shader.
			 */
			GLenum GetShaderType(void) { return m_type; };
			/**
			 * @brief This load/reload the data in the opengl context, needed when removed previously.
			 */
			void UpdateContext(void);
			/**
			 * @brief Remove the data from the opengl context.
			 */
			void RemoveContext(void);
			/**
			 * @brief Special android spec! It inform us that all context is removed and after notify us...
			 */
			void RemoveContextToLate(void);
			/**
			 * @brief Relode the shader from the file. used when a request of resouces reload is done.
			 * @note this is really usefull when we tested the new themes or shader developpements.
			 */
			void Reload(void);
	};
};

#endif

