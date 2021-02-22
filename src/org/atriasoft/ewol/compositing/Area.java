/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <gale/resource/Program.hpp>
#include <ewol/resource/Texture.hpp>
#include <egami/Image.hpp>
#include <ememory/memory.hpp>

namespace ewol {
	namespace compositing {
		class Area : public ewol::Compositing {
			private:
				Vector3f m_position; //!< The current position to draw
				etk::Color<float,4> m_color; //!< The text foreground color
			private:
				ememory::SharedPtr<gale::resource::Program> m_GLprogram;  //!< pointer on the opengl display program
				int32_t m_GLPosition; //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix;   //!< openGL id on the element (transformation matrix)
				int32_t m_GLColor;    //!< openGL id on the element (color buffer)
				int32_t m_GLtexture;  //!< openGL id on the element (Texture position)
				int32_t m_GLtexID;    //!< openGL id on the element (texture ID)
			private:
				ememory::SharedPtr<ewol::resource::Texture> m_resource; //!< texture resources
			protected:
				static const int32_t m_vboIdCoord;
				static const int32_t m_vboIdCoordText;
				static const int32_t m_vboIdColor;
				ememory::SharedPtr<gale::resource::VirtualBufferObject> m_VBO;
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram();
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _size Basic size of the area.
				 */
				Area(const Vector2i& _size);
				/**
				 * @brief generic destructor
				 */
				virtual ~Area();
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @brief clear alll the registered element in the current element
				 */
				void clear();
				/**
				 * @brief get the current display position (sometime needed in the gui control)
				 * @return the current position.
				 */
				const Vector3f& getPos() {
					return m_position;
				};
				/**
				 * @brief set position for the next text writen
				 * @param[in] _pos Position of the text (in 3D)
				 */
				void setPos(const Vector3f& _pos) {
					m_position = _pos;
				};
				inline void setPos(const Vector2f& _pos) {
					setPos(Vector3f(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos(const Vector3f& _pos) {
					m_position += _pos;
				};
				inline void setRelPos(const Vector2f& _pos) {
					setRelPos(Vector3f(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief add a compleate of the image to display with the requested size
				 * @param[in] _size size of the output image
				 */
				void print(const Vector2i& _size);
				
				egami::Image& get() {
					return m_resource->get();
				};
				void flush() {
					m_resource->flush();
				};
		};
	};
};
