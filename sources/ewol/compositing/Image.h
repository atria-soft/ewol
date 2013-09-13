/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_IMAGE_H__
#define __EWOL_COMPOSITING_IMAGE_H__

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/resources/ResourceManager.h>

namespace ewol
{
	class Image : public ewol::Compositing
	{
		private:
			vec3 m_position; //!< The current position to draw
			vec3 m_clippingPosStart; //!< Clipping start position
			vec3 m_clippingPosStop; //!< Clipping stop position
			bool m_clippingEnable; //!< true if the clipping must be activated
		private:
			etk::Color<> m_color; //!< The text foreground color
			float m_angle; //!< Angle to set at the axes
		private:
			ewol::Program* m_GLprogram; //!< pointer on the opengl display program
			int32_t m_GLPosition; //!< openGL id on the element (vertex buffer)
			int32_t m_GLMatrix; //!< openGL id on the element (transformation matrix)
			int32_t m_GLColor; //!< openGL id on the element (color buffer)
			int32_t m_GLtexture; //!< openGL id on the element (Texture position)
			int32_t m_GLtexID; //!< openGL id on the element (texture ID)
		private:
			ewol::TextureFile* m_resource; //!< texture resources
			etk::Vector<vec3 > m_coord; //!< internal coord of the object
			etk::Vector<vec2 > m_coordTex; //!< internal texture coordinate for every point
			etk::Vector<etk::Color<float> > m_coordColor; //!< internal color of the different point
		private:
			/**
			 * @brief Load the openGL program and get all the ID needed
			 */
			void LoadProgram(void);
		public:
			/**
			 * @brief generic constructor
			 * @param[in] _imageName Name of the file that might be loaded
			 */
			Image(const etk::UString& _imageName="");
			/**
			 * @brief generic destructor
			 */
			virtual ~Image(void);
		public:
			/**
			 * @brief Draw All the refistered text in the current element on openGL
			 * @param[in] _disableDepthTest Disable the Depth test for display
			 */
			void Draw(bool _disableDepthTest=true);
			/**
			 * @brief Clear alll tre registered element in the current element
			 */
			void Clear(void);
			/**
			 * @brief Get the current display position (sometime needed in the gui control)
			 * @return the current position.
			 */
			const vec3& GetPos(void) { return m_position; };
			/**
			 * @brief Set position for the next text writen
			 * @param[in] _pos Position of the text (in 3D)
			 */
			void SetPos(const vec3& _pos) { m_position = _pos; };
			inline void SetPos(const vec2& _pos) { SetPos(vec3(_pos.x(),_pos.y(),0)); };
			/**
			 * @brief Set relative position for the next text writen
			 * @param[in] _pos ofset apply of the text (in 3D)
			 */
			void SetRelPos(const vec3& _pos) { m_position += _pos; };
			inline void SetRelPos(const vec2& _pos) { SetRelPos(vec3(_pos.x(),_pos.y(),0)); };
			/**
			 * @brief Set the Color of the current foreground font
			 * @param[in] _color Color to set on foreground (for next print)
			 */
			void SetColor(const etk::Color<>& _color) { m_color = _color; };
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] _pos Start position of the clipping
			 * @param[in] _width Width size of the clipping
			 */
			void SetClippingWidth(const vec3& _pos, vec3 _width) { SetClipping(_pos, _pos+_width); };
			inline void SetClippingWidth(const vec2& _pos, const vec2& _width)  { SetClippingWidth(vec3(_pos.x(),_pos.y(),0), vec3(_width.x(),_width.y(),0)); };
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] _pos Start position of the clipping
			 * @param[in] _posEnd End position of the clipping
			 */
			void SetClipping(const vec3& _pos, vec3 _posEnd);
			inline void SetClipping(const vec2& _pos, const vec2& _posEnd)  { SetClipping(vec3(_pos.x(),_pos.y(),0), vec3(_posEnd.x(),_posEnd.y(),0)); };
			/**
			 * @brief Enable/Disable the clipping (without lose the current clipping position)
			 * @brief _newMode The new status of the clipping
			 */
			void SetClippingMode(bool _newMode) { m_clippingEnable = _newMode; };
			/**
			 * @brief Set a unique rotation of this element (not set in the Rotate Generic system)
			 * @param[in] _angle Angle to set in radiant.
			 */
			void SetAngle(float _angleRad);
			/**
			 * @brief Add a compleate of the image to display with the requested size
			 * @param[in] _size Size of the output image
			 */
			void Print(const ivec2& _size) { Print(vec2(_size.x(),_size.y())); };
			void Print(const vec2& _size);
			/**
			 * @brief Add a part of the image to display with the requested size
			 * @param[in] _size Size of the output image
			 * @param[in] _sourcePosStart Start position in the image [0..1] (can be bigger but this repeate the image).
			 * @param[in] _sourcePosStop Stop position in the image [0..1] (can be bigger but this repeate the image).
			 */
			void PrintPart(const vec2& _size,
			               const vec2& _sourcePosStart,
			               const vec2& _sourcePosStop);
			/**
			 * @brief Change the image Source ==> can not be done to display 2 images at the same time ...
			 * @param[in] _newFile New file of the Image
			 * @param[in] _size for the image when Verctorial image loading is requested
			 */
			void SetSource(const etk::UString& _newFile, int32_t _size=32) { SetSource(_newFile, vec2(_size,_size)); };
			void SetSource(const etk::UString& _newFile, const vec2& _size);
			/**
			 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
			 * @return the validity od the resources.
			 */
			bool HasSources(void);
			/**
			 * @brief Get the source image registered size in the file (<0 when multiple size image)
			 * @return tre image registered size
			 */
			vec2 GetRealSize(void);
	};
};

#endif

