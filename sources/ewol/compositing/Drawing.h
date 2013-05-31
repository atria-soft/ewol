/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_DRAWING_H__
#define __EWOL_COMPOSITING_DRAWING_H__

#include <draw/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/renderer/ResourceManager.h>


namespace ewol
{
	class Drawing : public ewol::Compositing
	{
		private:
			vec3 m_position;         //!< The current position to draw
			vec3 m_clippingPosStart; //!< Clipping start position
			vec3 m_clippingPosStop;  //!< Clipping stop position
			bool m_clippingEnable;   //!< true if the clipping must be activated
		private:
			draw::Color  m_color;   //!< The text foreground color
			draw::Color  m_colorBg; //!< The text background color
		private:
			ewol::Program*  m_GLprogram;  //!< pointer on the opengl display program
			int32_t         m_GLPosition; //!< openGL id on the element (vertex buffer)
			int32_t         m_GLMatrix;   //!< openGL id on the element (transformation matrix)
			int32_t         m_GLColor;    //!< openGL id on the element (color buffer)
		private: // Background Color (display only when needed)
			etk::Vector<vec3 >        m_coord;      //!< internal position for the text display
			etk::Vector<draw::Colorf> m_coordColor; //!< internal color of the background
		public:
			/**
			 * @brief Basic constructor
			 */
			Drawing(void);
			/**
			 * @brief Basic destructor
			 */
			~Drawing(void);
		private:
			/**
			 * @brief Load the openGL program and get all the ID needed
			 */
			void LoadProgram(void);
			/**
			 * @brief Un-Load the openGL program and get all the ID needed
			 */
			void UnLoadProgram(void);
			float        m_thickness;     //!< when drawing line and other things
			int32_t      m_triElement;    //!< special counter of the single dot generated
			vec3         m_triangle[3];   //!< Register every system with a combinaison of tiangle
			draw::Colorf m_tricolor[3];   //!< Register every the associated color foreground
		// internal API for the generation abstraction of triangles
			/**
			 * @brief Lunch the generation of triangle
			 */
			void GenerateTriangle(void);
			/**
			 * @brief in case of some error the count can be reset
			 */
			void ResetCount(void);
			/**
			 * @brief Set the Color of the current triangle drawing
			 * @param[in] _color Color to current dots generated
			 */
			void InternalSetColor(const draw::Color& _color);
			/**
			 * @brief internal add of the specific point
			 * @param[in] _point The requeste dpoint to add
			 */
			void SetPoint(const vec3& point);
			
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
			const vec3& GetPos(void) { return m_position; };
			/**
			 * @brief Set position for the next text writen
			 * @param[in] _pos Position of the text (in 3D)
			 */
			void SetPos(const vec3& _pos) { m_position = _pos; };
			inline void SetPos(const vec2& _pos) { SetPos(vec3(_pos.x(), _pos.y(), 0)); };
			/**
			 * @brief Set relative position for the next text writen
			 * @param[in] _pos ofset apply of the text (in 3D)
			 */
			void SetRelPos(const vec3& _pos) { m_position += _pos; };
			inline void SetRelPos(const vec2& _pos) { SetRelPos(vec3(_pos.x(), _pos.y(), 0)); };
			/**
			 * @brief Set the Color of the current foreground font
			 * @param[in] _color Color to set on foreground (for next print)
			 */
			void SetColor(const draw::Color& _color) { m_color = _color; };
			/**
			 * @brief Set the background color of the font (for selected Text (not the global BG))
			 * @param[in] _color Color to set on background (for next print)
			 */
			void SetColorBg(const draw::Color& _color) { m_colorBg = _color; };
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in]_ pos Start position of the clipping
			 * @param[in] _width Width size of the clipping
			 */
			void SetClippingWidth(const vec3& _pos, const vec3& _width) { SetClipping(_pos, _pos+_width); };
			inline void SetClippingWidth(const vec2& _pos, const vec2& _width) { SetClippingWidth(vec3(_pos.x(),_pos.y(),-1), vec3(_width.x(),_width.y(), 2)); };
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] _pos Start position of the clipping
			 * @param[in] _posEnd End position of the clipping
			 */
			void SetClipping(const vec3& _pos, const vec3& _posEnd);
			inline void SetClipping(const vec2& _pos, const vec2& _posEnd) { SetClipping(vec3(_pos.x(),_pos.y(),-1), vec3(_posEnd.x(),_posEnd.y(), 1)); };
			/**
			 * @brief Enable/Disable the clipping (without lose the current clipping position)
			 * @brief _newMode The new status of the clipping
			 */
			void SetClippingMode(bool _newMode) { m_clippingEnable = _newMode; };
			/**
			 * @brief Specify the line thickness for the next elements
			 * @param[in] _thickness The thickness disired for the next print
			 */
			void SetThickness(float _thickness);
			/**
			 * @brief Add a point reference at the current position (this is a vertex reference at the current position
			 */
			void AddVertex(void);
			/**
			 * @brief Draw a line to a specific position
			 * @param[in] _dest Position of the end of the line.
			 */
			void LineTo(const vec3& _dest);
			inline void LineTo(const vec2& _dest) { LineTo(vec3(_dest.x(), _dest.y(), 0)); };
			/**
			 * @brief Relative drawing a line (spacial vector)
			 * @param[in] _vect Vector of the curent line.
			 */
			void LineRel(const vec3& _vect) { LineTo(m_position+_vect); };
			inline void LineRel(const vec2& _vect) { LineRel(vec3(_vect.x(), _vect.y(), 0)); };
			/**
			 * @brief Draw a 2D rectangle to the position requested.
			 * @param[in] _dest Position the the end of the rectangle
			 */
			void Rectangle(const vec3& _dest);
			inline void Rectangle(const vec2& _dest) { Rectangle(vec3(_dest.x(), _dest.y(), 0)); };
			/**
			 * @brief Draw a 2D rectangle to the requested size.
			 * @param[in] _size size of the rectangle
			 */
			void RectangleWidth(const vec3& _size) { Rectangle(m_position+_size); };
			inline void RectangleWidth(const vec2& _size) { RectangleWidth(vec3(_size.x(), _size.y(), 0)); };
			/**
			 * @brief Draw a 3D rectangle to the position requested.
			 * @param[in] _dest Position the the end of the rectangle
			 */
			void Cube(const vec3& _dest);
			/**
			 * @brief Draw a 2D circle with the specify rafdius parameter.
			 * @param[in] _radius Distence to the dorder
			 * @param[in] _angleStart start angle of this circle ([0..2PI] otherwithe ==> disable)
			 * @param[in] _angleStop stop angle of this circle ([0..2PI] otherwithe ==> disable)
			 */
			void Circle(float _radius, float _angleStart = 0, float _angleStop = 2*M_PI);
	};
};

#endif

