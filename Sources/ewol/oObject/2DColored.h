/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_O_OBJECT_2D_COLORED_H__
#define __EWOL_O_OBJECT_2D_COLORED_H__

#include <ewol/oObject/OObject.h>
#include <ewol/ResourceManager.h>

namespace ewol {
	class OObject2DColored :public ewol::OObject
	{
		public:
			OObject2DColored(void);
			virtual ~OObject2DColored(void);
		public:
			virtual void Draw(void);
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
			etk::Vector<etk::Vector2D<float> >  m_coord;       //!< internal coord of the object
			etk::Vector<draw::Colorf>           m_coordColor;  //!< internal color of the different point
			draw::Colorf                        m_color[3];
			int32_t                             m_triElement;
			etk::Vector2D<float>                m_triangle[3];
			void GenerateTriangle(void);
			void ResetCount(void);
		public:
			void Clear(void);
			void SetColor(float red, float green, float blue, float alpha = 1.0);
			void SetColor(draw::Color color);
			void SetPoint(etk::Vector2D<float>  point);
			void SetPoint(float x, float y);
			void Line(float sx, float sy, float ex, float ey, float thickness);
			void Rectangle(float x, float y, float w, float h);
			void RectangleBorder(float x, float y, float w, float h, float thickness);
			void Circle(float x, float y, float radius, float thickness);
			void CirclePart(float x, float y, float radius, float thickness, float angleStart, float angleStop);
			void Disc(float x, float y, float radius);
			void DiscPart(float x, float y, float radius, float angleStart, float angleStop);
	};
};

#endif

