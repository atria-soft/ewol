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
#include <ewol/renderer/resources/Image.h>

namespace ewol
{
	class MaterialGlId
	{
		public:
			// GL index
			int32_t m_GL_ambientFactor;
			int32_t m_GL_diffuseFactor;
			int32_t m_GL_specularFactor;
			int32_t m_GL_shininess;
			int32_t m_GL_texture0;
			MaterialGlId(void);
			void Link(ewol::Program* _prog, const etk::UString& _baseName);
	};
	class Material
	{
		private:
			// values
			vec4 m_ambientFactor;
			vec4 m_diffuseFactor;
			vec4 m_specularFactor;
			float m_shininess;
			ewol::TextureFile* m_texture0;
		public:
			Material(void);
			~Material(void);
			void Draw(ewol::Program* _prog, const MaterialGlId& _glID);
			void SetAmbientFactor(const vec4& _val) {
				m_ambientFactor = _val;
			}
			void SetDiffuseFactor(const vec4& _val) {
				m_diffuseFactor = _val;
			}
			void SetSpecularFactor(const vec4& _val) {
				m_specularFactor = _val;
			}
			void SetShininess(float _val) {
				m_shininess = _val;
			}
			void SetTexture0(const etk::UString& _filename);
	};
	
};


#endif

