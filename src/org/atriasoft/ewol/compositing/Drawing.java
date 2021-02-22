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
				Vector3f m_position;         //!< The current position to draw
				Vector3f m_clippingPosStart; //!< Clipping start position
				Vector3f m_clippingPosStop;  //!< Clipping stop position
				bool m_clippingEnable;   //!< true if the clipping must be activated
			private:
				etk::Color<> m_color;   //!< The text foreground color
				etk::Color<> m_colorBg; //!< The text background color
			private:
				ememory::SharedPtr<gale::resource::Program> m_GLprogram;  //!< pointer on the opengl display program
				int32_t m_GLPosition; //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix; //!< openGL id on the element (transformation matrix)
				int32_t m_GLMatrixPosition; //!< position matrix
				int32_t m_GLColor; //!< openGL id on the element (color buffer)
			protected:
				static const int32_t m_vboIdCoord;
				static const int32_t m_vboIdColor;
				ememory::SharedPtr<gale::resource::VirtualBufferObject> m_VBO;
			public:
				/**
				 * @brief Basic constructor
				 */
				Drawing();
				/**
				 * @brief Basic destructor
				 */
				virtual ~Drawing();
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram();
				/**
				 * @brief Un-Load the openGL program and get all the ID needed
				 */
				void unLoadProgram();
				float m_thickness; //!< when drawing line and other things
				int32_t m_triElement; //!< special counter of the single dot generated
				Vector3f m_triangle[3]; //!< Register every system with a combinaison of tiangle
				etk::Color<float,4> m_tricolor[3]; //!< Register every the associated color foreground
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
				void internalSetColor(const etk::Color<>& _color);
				/**
				 * @brief internal add of the specific point
				 * @param[in] _point The requeste dpoint to add
				 */
				void setPoint(const Vector3f& point);
				
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @brief clear alll tre registered element in the current element
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
					setPos(Vector3f(_pos.x(), _pos.y(), 0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos(const Vector3f& _pos) {
					m_position += _pos;
				};
				inline void setRelPos(const Vector2f& _pos) {
					setRelPos(Vector3f(_pos.x(), _pos.y(), 0));
				};
				/**
				 * @brief set the Color of the current foreground font
				 * @param[in] _color Color to set on foreground (for next print)
				 */
				void setColor(const etk::Color<>& _color) {
					m_color = _color;
				};
				/**
				 * @brief Get the foreground color of the font.
				 * @return Foreground color.
				 */
				const etk::Color<>& getColor() {
					return m_color;
				};
				/**
				 * @brief set the background color of the font (for selected Text (not the global BG))
				 * @param[in] _color Color to set on background (for next print)
				 */
				void setColorBg(const etk::Color<>& _color) {
					m_colorBg = _color;
				};
				/**
				 * @brief Get the background color of the font.
				 * @return Background color.
				 */
				const etk::Color<>& getColorBg() {
					return m_colorBg;
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in]_ pos Start position of the clipping
				 * @param[in] _width Width size of the clipping
				 */
				void setClippingWidth(const Vector3f& _pos, const Vector3f& _width) {
					setClipping(_pos, _pos+_width);
				};
				inline void setClippingWidth(const Vector2f& _pos, const Vector2f& _width) {
					setClippingWidth(Vector3f(_pos.x(),_pos.y(),-1), Vector3f(_width.x(),_width.y(), 2));
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _posEnd End position of the clipping
				 */
				void setClipping(const Vector3f& _pos, const Vector3f& _posEnd);
				inline void setClipping(const Vector2f& _pos, const Vector2f& _posEnd) {
					setClipping(Vector3f(_pos.x(),_pos.y(),-1), Vector3f(_posEnd.x(),_posEnd.y(), 1));
				};
				/**
				 * @brief enable/Disable the clipping (without lose the current clipping position)
				 * @brief _newMode The new status of the clipping
				 */
				void setClippingMode(bool _newMode) {
					m_clippingEnable = _newMode;
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
				void lineTo(const Vector3f& _dest);
				inline void lineTo(const Vector2f& _dest) {
					lineTo(Vector3f(_dest.x(), _dest.y(), 0));
				};
				/**
				 * @brief Relative drawing a line (spacial vector)
				 * @param[in] _vect Vector of the curent line.
				 */
				void lineRel(const Vector3f& _vect) {
					lineTo(m_position+_vect);
				};
				inline void lineRel(const Vector2f& _vect) {
					lineRel(Vector3f(_vect.x(), _vect.y(), 0));
				};
				/**
				 * @brief draw a 2D rectangle to the position requested.
				 * @param[in] _dest Position the the end of the rectangle
				 */
				void rectangle(const Vector3f& _dest);
				inline void rectangle(const Vector2f& _dest) {
					rectangle(Vector3f(_dest.x(), _dest.y(), 0));
				};
				/**
				 * @brief draw a 2D rectangle to the requested size.
				 * @param[in] _size size of the rectangle
				 */
				void rectangleWidth(const Vector3f& _size) {
					rectangle(m_position+_size);
				};
				inline void rectangleWidth(const Vector2f& _size) {
					rectangleWidth(Vector3f(_size.x(), _size.y(), 0));
				};
				/**
				 * @brief draw a 3D rectangle to the position requested.
				 * @param[in] _dest Position the the end of the rectangle
				 */
				void cube(const Vector3f& _dest);
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

