/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_O_OBJECT_3D_TEXTURED_H__
#define __EWOL_O_OBJECT_3D_TEXTURED_H__

#include <ewol/oObject/OObject.h>
#include <ewol/ResourceManager.h>

namespace ewol {
	class OObject3DTextured :public ewol::OObject
	{
		public:
			OObject3DTextured(etk::UString textureName, float sizeX=-1, float sizeY=-1);
			virtual ~OObject3DTextured(void);
		public:
			virtual void Draw(void);
			void Clear(void);
			void Rectangle(float x, float y, float w, float h, float texX=0.0, float texY=0.0, float texSX=1.0, float texSY=1.0, draw::Color tmpColor=draw::color::white);
			void Rectangle(float x, float y, float w, float h, draw::Color tmpColor);
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
			int32_t        m_GLtexture;
			int32_t        m_GLtexID;
			ewol::TextureFile*                   m_resource;    //!< texture resources
			etk::Vector<etk::Vector3D<float> >   m_coord;       //!< internal coord of the object
			etk::Vector<texCoord_ts>             m_coordTex;    //!< internal texture coordinate for every point
			etk::Vector<draw::Colorf>        m_coordColor;  //!< internal color of the different point
	};
};

#endif

