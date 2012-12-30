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
#include <ewol/renderer/resources/Resource.h>
#include <ewol/renderer/resources/Image.h>
#include <ewol/renderer/resources/Shader.h>
#include <ewol/renderer/resources/Program.h>
#include <ewol/game/MeshObject.h>

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
			Colored3DObject(etk::UString genName);
			virtual ~Colored3DObject(void);
			virtual const char* GetType(void) { return "ewol::Colored3DObject"; };
			virtual void Draw(etk::Vector<vec3>& vertices,
			                  etk::Vector<draw::Colorf>& color,
			                  bool updateDepthBuffer=true);
			
	};
};


#endif
