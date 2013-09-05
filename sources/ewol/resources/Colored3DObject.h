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
#include <ewol/resources/Resource.h>
#include <ewol/resources/Image.h>
#include <ewol/resources/Shader.h>
#include <ewol/resources/Program.h>

namespace ewol
{
	class Colored3DObject : public ewol::Resource
	{
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
		public:
			Colored3DObject(etk::UString _genName);
			virtual ~Colored3DObject(void);
			virtual const char* GetType(void) { return "ewol::Colored3DObject"; };
			virtual void Draw(etk::Vector<vec3>& _vertices,
			                  const etk::Color<float>& _color,
			                  bool _updateDepthBuffer=true,
			                  bool _depthtest=true);
			virtual void Draw(etk::Vector<vec3>& _vertices,
			                  const etk::Color<float>& _color,
			                  mat4& _transformationMatrix,
			                  bool _updateDepthBuffer=true,
			                  bool _depthtest=true);
			virtual void DrawLine(etk::Vector<vec3>& _vertices,
			                      const etk::Color<float>& _color,
			                      mat4& _transformationMatrix,
			                      bool _updateDepthBuffer=true,
			                      bool _depthtest=true);
			
	};
};


#endif