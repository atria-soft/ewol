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


namespace ewol {
	namespace compositing {
		class Drawing : public ewol::Compositing {
			private:
				Vector3f this.position;         //!< The current position to draw
				Vector3f this.clippingPosStart; //!< Clipping start position
				Vector3f this.clippingPosStop;  //!< Clipping stop position
				boolean this.clippingEnable;   //!< true if the clipping must be activated
			private:
				etk::Color<> this.color;   //!< The text foreground color
				etk::Color<> this.colorBg; //!< The text background color
			private:
				ememory::Ptr<gale::resource::Program> this.GLprogram;  //!< pointer on the opengl display program
				int this.GLPosition; //!< openGL id on the element (vertex buffer)
				int this.GLMatrix; //!< openGL id on the element (transformation matrix)
				int this.GLMatrixPosition; //!< position matrix
				int this.GLColor; //!< openGL id on the element (color buffer)
			protected:
				static  int this.vboIdCoord;
				static  int this.vboIdColor;
				ememory::Ptr<gale::resource::VirtualBufferObject> this.VBO;
			public:
				/**
				 * @brief Basic ructor
				 */
				Drawing();
				/**
				 * @brief Basic destructor
				 */
				 ~Drawing();
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram();
				/**
				 * @brief Un-Load the openGL program and get all the ID needed
				 */
				void unLoadProgram();
				float this.thickness; //!< when drawing line and other things
				int this.triElement; //!< special counter of the single dot generated
				Vector3f this.triangle[3]; //!< Register every system with a combinaison of tiangle
				etk::Color<float,4> this.tricolor[3]; //!< Register every the associated color foreground
			// internal API for the generation abstraction of triangles
				/**
				 * @brief Lunch the generation of triangle
				 */
				void generateTriangle();
				/**
				 * @brief in case of some error the count can be reset
				 */
				void resetCount();
				/**
				 * @brief set the Color of the current triangle drawing
				 * @param[in] _color Color to current dots generated
				 */
				void internalSetColor( etk::Color<> _color);
				/**
				 * @brief internal add of the specific point
				 * @param[in] _point The requeste dpoint to add
				 */
				void setPoint( Vector3f point);
				
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(boolean _disableDepthTest=true);
				/**
				 * @brief clear alll tre registered element in the current element
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
					setPos(Vector3f(_pos.x(), _pos.y(), 0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos( Vector3f _pos) {
					this.position += _pos;
				};
				 void setRelPos( Vector2f _pos) {
					setRelPos(Vector3f(_pos.x(), _pos.y(), 0));
				};
				/**
				 * @brief set the Color of the current foreground font
				 * @param[in] _color Color to set on foreground (for next print)
				 */
				void setColor( etk::Color<> _color) {
					this.color = _color;
				};
				/**
				 * @brief Get the foreground color of the font.
				 * @return Foreground color.
				 */
				 etk::Color<> getColor() {
					return this.color;
				};
				/**
				 * @brief set the background color of the font (for selected Text (not the global BG))
				 * @param[in] _color Color to set on background (for next print)
				 */
				void setColorBg( etk::Color<> _color) {
					this.colorBg = _color;
				};
				/**
				 * @brief Get the background color of the font.
				 * @return Background color.
				 */
				 etk::Color<> getColorBg() {
					return this.colorBg;
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in]_ pos Start position of the clipping
				 * @param[in] _width Width size of the clipping
				 */
				void setClippingWidth( Vector3f _pos,  Vector3f _width) {
					setClipping(_pos, _pos+_width);
				};
				 void setClippingWidth( Vector2f _pos,  Vector2f _width) {
					setClippingWidth(Vector3f(_pos.x(),_pos.y(),-1), Vector3f(_width.x(),_width.y(), 2));
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _posEnd End position of the clipping
				 */
				void setClipping( Vector3f _pos,  Vector3f _posEnd);
				 void setClipping( Vector2f _pos,  Vector2f _posEnd) {
					setClipping(Vector3f(_pos.x(),_pos.y(),-1), Vector3f(_posEnd.x(),_posEnd.y(), 1));
				};
				/**
				 * @brief enable/Disable the clipping (without lose the current clipping position)
				 * @brief _newMode The new status of the clipping
				 */
				void setClippingMode(boolean _newMode) {
					this.clippingEnable = _newMode;
				};
				/**
				 * @brief Specify the line thickness for the next elements
				 * @param[in] _thickness The thickness disired for the next print
				 */
				void setThickness(float _thickness);
				/**
				 * @brief add a point reference at the current position (this is a vertex reference at the current position
				 */
				void addVertex();
				/**
				 * @brief draw a line to a specific position
				 * @param[in] _dest Position of the end of the line.
				 */
				void lineTo( Vector3f _dest);
				 void lineTo( Vector2f _dest) {
					lineTo(Vector3f(_dest.x(), _dest.y(), 0));
				};
				/**
				 * @brief Relative drawing a line (spacial vector)
				 * @param[in] _vect Vector of the curent line.
				 */
				void lineRel( Vector3f _vect) {
					lineTo(this.position+_vect);
				};
				 void lineRel( Vector2f _vect) {
					lineRel(Vector3f(_vect.x(), _vect.y(), 0));
				};
				/**
				 * @brief draw a 2D rectangle to the position requested.
				 * @param[in] _dest Position the the end of the rectangle
				 */
				void rectangle( Vector3f _dest);
				 void rectangle( Vector2f _dest) {
					rectangle(Vector3f(_dest.x(), _dest.y(), 0));
				};
				/**
				 * @brief draw a 2D rectangle to the requested size.
				 * @param[in] _size size of the rectangle
				 */
				void rectangleWidth( Vector3f _size) {
					rectangle(this.position+_size);
				};
				 void rectangleWidth( Vector2f _size) {
					rectangleWidth(Vector3f(_size.x(), _size.y(), 0));
				};
				/**
				 * @brief draw a 3D rectangle to the position requested.
				 * @param[in] _dest Position the the end of the rectangle
				 */
				void cube( Vector3f _dest);
				/**
				 * @brief draw a 2D circle with the specify rafdius parameter.
				 * @param[in] _radius Distence to the dorder
				 * @param[in] _angleStart start angle of this circle ([0..2PI] otherwithe  == > disable)
				 * @param[in] _angleStop stop angle of this circle ([0..2PI] otherwithe  == > disable)
				 */
				void circle(float _radius, float _angleStart = 0, float _angleStop = 2*M_PI);
		};
	};
};

