/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_IMAGE_H__
#define __EWOL_IMAGE_H__

namespace ewol
{
	class Image : public ewol::Compositing
	{
		protected:
			etk::Vector2D<int32_t> m_size;
		public:
			Image(void);
			~Image(void);
			void Draw(void);
			void Clear(void);
			void SetPos(etk::Vector3D<float> pos);
			void SetRelPos(etk::Vector3D<float> pos);
			void SetAngle(etk::Vector3D<float> angle);
			void SetColor(etk::Color color);
			void SetSize(etk::Vector2D<int32_t> size);
			// TODO : ...
	};
};

#endif

