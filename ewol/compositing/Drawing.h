/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_COMPOSITING_DRAWING_H__
#define __EWOL_COMPOSITING_DRAWING_H__

#include <etk/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <gale/resource/Program.h>


namespace ewol {
	namespace compositing {
		class Drawing : public ewol::Compositing {
			private:
				vec3 m_position;         //!< The current position to draw
				vec3 m_clippingPosStart; //!< Clipping start position
				vec3 m_clippingPosStop;  //!< Clipping stop position
				bool m_clippingEnable;   //!< true if the clipping must be activated
			private:
				etk::Color<> m_color;   //!< The text foreground color
				etk::Color<> m_colorBg; //!< The text background color
			private:
				std::shared_ptr<gale::resource::Program> m_GLprogram;  //!< pointer on the opengl display program
				int32_t m_GLPosition; //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix; //!< openGL id on the element (transformation matrix)
				int32_t m_GLMatrixPosition; //!< position matrix
				int32_t m_GLColor; //!< openGL id on the element (color buffer)
			public: // Background Color (display only when needed)
				std::vector<vec3 > m_coord; //!< internal position for the text display
				std::vector<etk::Color<float> > m_coordColor; //!< internal color of the background
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
				vec3 m_triangle[3]; //!< Register every system with a combinaison of tiangle
				etk::Color<float> m_tricolor[3]; //!< Register every the associated color foreground
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
				void setPoint(const vec3& point);
				
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
				const vec3& getPos() {
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
					setPos(vec3(_pos.x(), _pos.y(), 0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos(const vec3& _pos) {
					m_position += _pos;
				};
				inline void setRelPos(const vec2& _pos) {
					setRelPos(vec3(_pos.x(), _pos.y(), 0));
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
				void setClippingWidth(const vec3& _pos, const vec3& _width) {
					setClipping(_pos, _pos+_width);
				};
				inline void setClippingWidth(const vec2& _pos, const vec2& _width) {
					setClippingWidth(vec3(_pos.x(),_pos.y(),-1), vec3(_width.x(),_width.y(), 2));
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _posEnd End position of the clipping
				 */
				void setClipping(const vec3& _pos, const vec3& _posEnd);
				inline void setClipping(const vec2& _pos, const vec2& _posEnd) {
					setClipping(vec3(_pos.x(),_pos.y(),-1), vec3(_posEnd.x(),_posEnd.y(), 1));
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
				void lineTo(const vec3& _dest);
				inline void lineTo(const vec2& _dest) {
					lineTo(vec3(_dest.x(), _dest.y(), 0));
				};
				/**
				 * @brief Relative drawing a line (spacial vector)
				 * @param[in] _vect Vector of the curent line.
				 */
				void lineRel(const vec3& _vect) {
					lineTo(m_position+_vect);
				};
				inline void lineRel(const vec2& _vect) {
					lineRel(vec3(_vect.x(), _vect.y(), 0));
				};
				/**
				 * @brief draw a 2D rectangle to the position requested.
				 * @param[in] _dest Position the the end of the rectangle
				 */
				void rectangle(const vec3& _dest);
				inline void rectangle(const vec2& _dest) {
					rectangle(vec3(_dest.x(), _dest.y(), 0));
				};
				/**
				 * @brief draw a 2D rectangle to the requested size.
				 * @param[in] _size size of the rectangle
				 */
				void rectangleWidth(const vec3& _size) {
					rectangle(m_position+_size);
				};
				inline void rectangleWidth(const vec2& _size) {
					rectangleWidth(vec3(_size.x(), _size.y(), 0));
				};
				/**
				 * @brief draw a 3D rectangle to the position requested.
				 * @param[in] _dest Position the the end of the rectangle
				 */
				void cube(const vec3& _dest);
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

#endif

