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
			etk::Vector<uint32_t> m_listIndexFaces;
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
			
			void SetImageSize(const ivec2& _newSize) { if (m_texture0==NULL){return;} m_texture0->SetImageSize(_newSize); };
			// get the reference on this image to draw nomething on it ...
			egami::Image* Get(void) { if (m_texture0==NULL){return NULL;} return &m_texture0->Get(); };
			// Flush the data to send it at the OpenGl system
			void Flush(void) { if (m_texture0==NULL){return;} m_texture0->Flush(); };
	};
	
};


#endif

