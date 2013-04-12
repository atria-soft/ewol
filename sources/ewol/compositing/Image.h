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
#include <ewol/renderer/ResourceManager.h>

namespace ewol
{
	class Image : public ewol::Compositing
	{
		private:
			vec3 m_position;         //!< The current position to draw
			vec3 m_clippingPosStart; //!< Clipping start position
			vec3 m_clippingPosStop;  //!< Clipping stop position
			bool m_clippingEnable;   //!< true if the clipping must be activated
		private:
			draw::Color m_color; //!< The text foreground color
			vec3        m_axes;  //!< Rotation axes (instant)
			float       m_angle; //!< Angle to set at the axes
		private:
			ewol::Program* m_GLprogram;  //!< pointer on the opengl display program
			int32_t        m_GLPosition; //!< openGL id on the element (vertex buffer)
			int32_t        m_GLMatrix;   //!< openGL id on the element (transformation matrix)
			int32_t        m_GLColor;    //!< openGL id on the element (color buffer)
			int32_t        m_GLtexture;  //!< openGL id on the element (Texture position)
			int32_t        m_GLtexID;    //!< openGL id on the element (texture ID)
		private:
			ewol::TextureFile*        m_resource;    //!< texture resources
			etk::Vector<vec3 >        m_coord;       //!< internal coord of the object
			etk::Vector<vec2 >        m_coordTex;    //!< internal texture coordinate for every point
			etk::Vector<draw::Colorf> m_coordColor;  //!< internal color of the different point
		private:
			/**
			 * @brief Load the openGL program and get all the ID needed
			 */
			void LoadProgram(void);
		public:
			/**
			 * @brief generic constructor
			 * @param[in] imageName Name of the file that might be loaded
			 */
			Image(const etk::UString& imageName="");
			/**
			 * @brief generic destructor
			 */
			~Image(void);
		public:
			/**
			 * @brief Draw All the refistered text in the current element on openGL
			 */
			void Draw(void);
			/**
			 * @brief Clear alll tre registered element in the current element
			 */
			void Clear(void);
			/**
			 * @brief Get the current display position (sometime needed in the gui control)
			 * @return the current position.
			 */
			vec3 GetPos(void);
			/**
			 * @brief Set position for the next text writen
			 * @param[in] pos Position of the text (in 3D)
			 */
			void SetPos(const vec3& pos);
			/**
			 * @brief Set relative position for the next text writen
			 * @param[in] pos ofset apply of the text (in 3D)
			 */
			void SetRelPos(const vec3& pos);
			/**
			 * @brief Set the Color of the current foreground font
			 * @param[in] color Color to set on foreground (for next print)
			 */
			void SetColor(const draw::Color& color);
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] pos Start position of the clipping
			 * @param[in] width Width size of the clipping
			 */
			void SetClippingWidth(const vec3& pos, vec3 width);
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] pos Start position of the clipping
			 * @param[in] posEnd End position of the clipping
			 */
			void SetClipping(const vec3& pos, vec3 posEnd);
			/**
			 * @brief Enable/Disable the clipping (without lose the current clipping position)
			 * @brief newMode The new status of the clipping
			 */
			void SetClippingMode(bool newMode);
			/**
			 * @brief Set a unique rotation of this element (not set in the Rotate Generic system)
			 * @param[in] axes Rotation axes selection
			 * @param[in] angle Angle to set on this axes
			 */
			void SetAngle(const vec3& axes, float angle);
			/**
			 * @brief Add a compleate of the image to display with the requested size
			 * @param[in] size Size of the output image
			 */
			void Print(const ivec2& size);
			void Print(const vec2& size);
			/**
			 * @brief Add a part of the image to display with the requested size
			 * @param[in] size Size of the output image
			 * @param[in] sourcePosStart Start position in the image [0..1] (can be bigger but this repeate the image).
			 * @param[in] sourcePosStop Stop position in the image [0..1] (can be bigger but this repeate the image).
			 */
			void PrintPart(const ivec2& size,
			               const vec2& sourcePosStart,
			               const vec2& sourcePosStop);
			/**
			 * @brief Change the image Source ==> can not be done to display 2 images at the same time ...
			 * @param[in] newFile New file of the Image
			 * @param[in] size for the image when Verctorial image loading is requested
			 */
			void SetSource(const etk::UString& newFile, int32_t size=32);
			void SetSource(const etk::UString& newFile, const vec2& size);
			/**
			 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
			 * @return the validity od the resources.
			 */
			bool HasSources(void);
	};
};

#endif

