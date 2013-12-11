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
#include <ewol/compositing/Compose.h>
#include <ewol/resources/ResourceManager.h>

namespace ewol {
	namespace compositing {
		class Area : public ewol::compositing::Compose {
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
				std::vector<vec3 > m_coord; //!< internal coord of the object
				std::vector<vec2 > m_coordTex; //!< internal texture coordinate for every point
				std::vector<etk::Color<float> > m_coordColor; //!< internal color of the different point
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram(void);
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
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @brief clear alll the registered element in the current element
				 */
				void clear(void);
				/**
				 * @brief get the current display position (sometime needed in the gui control)
				 * @return the current position.
				 */
				const vec3& getPos(void) {
					return m_position;
				};
				/**
				 * @brief set position for the next text writen
				 * @param[in] _pos Position of the text (in 3D)
				 */
				void setPos(const vec3& _pos) {
					m_position = _pos;
				};
				inline void setPos(const vec2& _pos) {
					setPos(vec3(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos(const vec3& _pos) {
					m_position += _pos;
				};
				inline void setRelPos(const vec2& _pos) {
					setRelPos(vec3(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief add a compleate of the image to display with the requested size
				 * @param[in] _size size of the output image
				 */
				void print(const ivec2& _size);
				
				egami::Image& get(void) {
					return m_resource->get();
				};
				void flush(void) {
					m_resource->flush();
				};
		};
	};
};

#endif

