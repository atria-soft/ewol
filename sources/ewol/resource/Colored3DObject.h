/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __COLORED_3D_OBJECT_H__
#define __COLORED_3D_OBJECT_H__

#include <etk/types.h>
#include <ewol/resource/Resource.h>
#include <ewol/resource/Image.h>
#include <ewol/resource/Shader.h>
#include <ewol/resource/Program.h>

namespace ewol {
	namespace resource {
		class Colored3DObject : public ewol::Resource {
			protected:
				ewol::object::Shared<ewol::resource::Program> m_GLprogram;
				int32_t m_GLPosition;
				int32_t m_GLMatrix;
				int32_t m_GLColor;
			protected:
				Colored3DObject();
			public:
				virtual ~Colored3DObject();
			public:
				virtual void draw(std::vector<vec3>& _vertices,
				                  const etk::Color<float>& _color,
				                  bool _updateDepthBuffer=true,
				                  bool _depthtest=true);
				virtual void draw(std::vector<vec3>& _vertices,
				                  const etk::Color<float>& _color,
				                  mat4& _transformationMatrix,
				                  bool _updateDepthBuffer=true,
				                  bool _depthtest=true);
				virtual void drawLine(std::vector<vec3>& _vertices,
				                      const etk::Color<float>& _color,
				                      mat4& _transformationMatrix,
				                      bool _updateDepthBuffer=true,
				                      bool _depthtest=true);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static ewol::object::Shared<ewol::resource::Colored3DObject> keep();
		};
	};
};


#endif
