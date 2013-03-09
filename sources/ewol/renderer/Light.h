/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LIGHT_H__
#define __EWOL_LIGHT_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Vector4D.h>
#include <ewol/renderer/resources/Program.h>

namespace ewol
{
	class Light
	{
		private:
			// values
			vec3 m_direction;
			vec3 m_halfplane;
			vec4 m_ambientColor;
			vec4 m_diffuseColor;
			vec4 m_specularColor;
		private:
			// GL index
			int32_t m_GL_direction;
			int32_t m_GL_halfplane;
			int32_t m_GL_ambientColor;
			int32_t m_GL_diffuseColor;
			int32_t m_GL_specularColor;
		public:
			Light(void);
			~Light(void);
			void Link(ewol::Program* prog, const etk::UString& baseName);
			void Draw(ewol::Program* prog);
			void SetDirection(const vec3& val) {
				m_direction = val;
			}
			void SetHalfPlane(const vec3& val) {
				m_halfplane = val;
			}
			void SetAmbientColor(const vec4& val) {
				m_ambientColor = val;
			}
			void SetDiffuseColor(const vec4& val) {
				m_diffuseColor = val;
			}
			void SetSpecularColor(const vec4& val) {
				m_specularColor = val;
			}
	};
};


#endif
