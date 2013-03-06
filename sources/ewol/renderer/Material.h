/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_MATERIAL_H__
#define __EWOL_MATERIAL_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Vector4D.h>
#include <ewol/renderer/resources/Program.h>

namespace ewol
{
	class Material
	{
		private:
			// values
			vec4    m_ambientFactor;
			vec4    m_diffuseFactor;
			vec4    m_specularFactor;
			float   m_shininess;
		private:
			// GL index
			int32_t m_GL_ambientFactor;
			int32_t m_GL_diffuseFactor;
			int32_t m_GL_specularFactor;
			int32_t m_GL_shininess;
		public:
			Material(void);
			~Material(void);
			void Link(ewol::Program* prog, const etk::UString& baseName);
			void Draw(ewol::Program* prog);
			void SetAmbientFactor(const vec4& val) {
				m_ambientFactor = val;
			}
			void SetDiffuseFactor(const vec4& val) {
				m_diffuseFactor = val;
			}
			void SetSpecularFactor(const vec4& val) {
				m_specularFactor = val;
			}
			void SetShininess(float val) {
				m_shininess = val;
			}
	};
};


#endif

