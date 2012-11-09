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
	class Shader : public ewol::Resource
	{
		private :
			bool           m_exist;
			char*          m_fileData;
			GLuint         m_shader;
			GLenum         m_type;
		public:
			Shader(etk::UString& filename);
			virtual ~Shader(void);
			const char* GetType(void) { return "ewol::Shader"; };
			GLuint GetGL_ID(void) { return m_shader; };
			GLenum GetShaderType(void) { return m_type; };
			void UpdateContext(void);
			void RemoveContext(void);
			void RemoveContextToLate(void);
			void Reload(void);
	};
};

#endif

