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
				Vector3f this.position; //!< The current position to draw
				etk::Color<float,4> this.color; //!< The text foreground color
			private:
				ememory::Ptr<gale::resource::Program> this.GLprogram;  //!< pointer on the opengl display program
				int this.GLPosition; //!< openGL id on the element (vertex buffer)
				int this.GLMatrix;   //!< openGL id on the element (transformation matrix)
				int this.GLColor;    //!< openGL id on the element (color buffer)
				int this.GLtexture;  //!< openGL id on the element (Texture position)
				int this.GLtexID;    //!< openGL id on the element (texture ID)
			private:
				ememory::Ptr<ewol::resource::Texture> this.resource; //!< texture resources
			protected:
				static  int this.vboIdCoord;
				static  int this.vboIdCoordText;
				static  int this.vboIdColor;
				ememory::Ptr<gale::resource::VirtualBufferObject> this.VBO;
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram();
			public:
				/**
				 * @brief generic ructor
				 * @param[in] _size Basic size of the area.
				 */
				Area( Vector2i _size);
				/**
				 * @brief generic destructor
				 */
				 ~Area();
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(boolean _disableDepthTest=true);
				/**
				 * @brief clear alll the registered element in the current element
				 */
				void clear();
				/**
				 * @brief get the current display position (sometime needed in the gui control)
				 * @return the current position.
				 */
				 Vector3f getPos() {
					return this.position;
				};
				/**
				 * @brief set position for the next text writen
				 * @param[in] _pos Position of the text (in 3D)
				 */
				void setPos( Vector3f _pos) {
					this.position = _pos;
				};
				 void setPos( Vector2f _pos) {
					setPos(Vector3f(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos( Vector3f _pos) {
					this.position += _pos;
				};
				 void setRelPos( Vector2f _pos) {
					setRelPos(Vector3f(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief add a compleate of the image to display with the requested size
				 * @param[in] _size size of the output image
				 */
				void print( Vector2i _size);
				
				egami::Image get() {
					return this.resource.get();
				};
				void flush() {
					this.resource.flush();
				};
		};
	};
};
