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
#include <ewol/resources/Program.h>

namespace ewol {
	class Light {
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
			void link(ewol::Program* _prog, const std::string& _baseName);
			void draw(ewol::Program* _prog);
			void setDirection(const vec3& val) {
				m_direction = val;
			}
			void setHalfPlane(const vec3& val) {
				m_halfplane = val;
			}
			void setAmbientColor(const vec4& val) {
				m_ambientColor = val;
			}
			void setDiffuseColor(const vec4& val) {
				m_diffuseColor = val;
			}
			void setSpecularColor(const vec4& val) {
				m_specularColor = val;
			}
			
		friend etk::CCout& operator <<(etk::CCout& _os, const ewol::Light& _obj);
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::Light& _obj);
};


#endif

