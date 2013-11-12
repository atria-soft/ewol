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
#include <ewol/resources/Program.h>
#include <ewol/resources/Image.h>

namespace ewol {
	class MaterialGlId {
		public:
			// GL index
			int32_t m_GL_ambientFactor;
			int32_t m_GL_diffuseFactor;
			int32_t m_GL_specularFactor;
			int32_t m_GL_shininess;
			int32_t m_GL_texture0;
			MaterialGlId(void);
			void link(ewol::Program* _prog, const std::string& _baseName);
	};
	class Material {
		private:
			// values
			vec4 m_ambientFactor;
			vec4 m_diffuseFactor;
			vec4 m_specularFactor;
			float m_shininess;
			ewol::TextureFile* m_texture0;
		public:
			std::vector<uint32_t> m_listIndexFaces;
		public:
			Material(void);
			~Material(void);
			void draw(ewol::Program* _prog, const MaterialGlId& _glID);
			void setAmbientFactor(const vec4& _val) {
				m_ambientFactor = _val;
			}
			void setDiffuseFactor(const vec4& _val) {
				m_diffuseFactor = _val;
			}
			void setSpecularFactor(const vec4& _val) {
				m_specularFactor = _val;
			}
			void setShininess(float _val) {
				m_shininess = _val;
			}
			void setTexture0(const std::string& _filename);
			
			void setImageSize(const ivec2& _newSize) { if (m_texture0 == NULL){return;} m_texture0->setImageSize(_newSize); };
			// get the reference on this image to draw nomething on it ...
			egami::Image* get(void) { if (m_texture0 == NULL){return NULL;} return &m_texture0->get(); };
			// flush the data to send it at the openGl system
			void flush(void) { if (m_texture0 == NULL){return;} m_texture0->flush(); };
	};
	
};


#endif

