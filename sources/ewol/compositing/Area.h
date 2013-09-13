/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_AREA_H__
#define __EWOL_COMPOSITING_AREA_H__

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/resources/ResourceManager.h>

namespace ewol
{
	class Area : public ewol::Compositing
	{
		private:
			vec3 m_position; //!< The current position to draw
			etk::Color<> m_color; //!< The text foreground color
		private:
			ewol::Program* m_GLprogram;  //!< pointer on the opengl display program
			int32_t        m_GLPosition; //!< openGL id on the element (vertex buffer)
			int32_t        m_GLMatrix;   //!< openGL id on the element (transformation matrix)
			int32_t        m_GLColor;    //!< openGL id on the element (color buffer)
			int32_t        m_GLtexture;  //!< openGL id on the element (Texture position)
			int32_t        m_GLtexID;    //!< openGL id on the element (texture ID)
		private:
			ewol::Texture* m_resource; //!< texture resources
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
			 * @param[in] _size Basic size of the area.
			 */
			Area(const ivec2& _size);
			/**
			 * @brief generic destructor
			 */
			~Area(void);
		public:
			/**
			 * @brief Draw All the refistered text in the current element on openGL
			 */
			void Draw(bool _disableDepthTest=true);
			/**
			 * @brief Clear alll the registered element in the current element
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
			 * @brief Add a compleate of the image to display with the requested size
			 * @param[in] _size Size of the output image
			 */
			void Print(const ivec2& _size);
			
			egami::Image& Get(void) { return m_resource->Get(); };
			void Flush(void) { m_resource->Flush(); };
			
	};
};

#endif

