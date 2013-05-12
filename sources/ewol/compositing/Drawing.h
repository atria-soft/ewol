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
			 * @param[in] color Color to current dots generated
			 */
			void InternalSetColor(draw::Color& color);
			/**
			 * @brief internal add of the specific point
			 * @param[in] point The requeste dpoint to add
			 */
			void SetPoint(vec3 point);
			
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
			void SetPos(vec3 pos);
			inline void SetPos(const vec2& pos) { SetPos(vec3(pos.x(), pos.y(), 0)); };
			/**
			 * @brief Set relative position for the next text writen
			 * @param[in] pos ofset apply of the text (in 3D)
			 */
			void SetRelPos(vec3 pos);
			inline void SetRelPos(const vec2& pos) { SetRelPos(vec3(pos.x(), pos.y(), 0)); };
			/**
			 * @brief Set the Color of the current foreground font
			 * @param[in] color Color to set on foreground (for next print)
			 */
			void SetColor(draw::Color color);
			/**
			 * @brief Set the background color of the font (for selected Text (not the global BG))
			 * @param[in] color Color to set on background (for next print)
			 */
			void SetColorBg(draw::Color color);
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] pos Start position of the clipping
			 * @param[in] width Width size of the clipping
			 */
			void SetClippingWidth(vec3 pos, vec3 width);
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] pos Start position of the clipping
			 * @param[in] posEnd End position of the clipping
			 */
			void SetClipping(vec3 pos, vec3 posEnd);
			/**
			 * @brief Enable/Disable the clipping (without lose the current clipping position)
			 * @brief newMode The new status of the clipping
			 */
			void SetClippingMode(bool newMode);
			/**
			 * @brief Specify the line thickness for the next elements
			 * @param[in] thickness The thickness disired for the next print
			 */
			void SetThickness(float thickness);
			/**
			 * @brief Add a point reference at the current position (this is a vertex reference at the current position
			 */
			void AddVertex(void);
			/**
			 * @brief Draw a line to a specific position
			 * @param[in] dest Position of the end of the line.
			 */
			void LineTo(vec3 dest);
			inline void LineTo(const vec2& dest) { LineTo(vec3(dest.x(), dest.y(), 0)); };
			/**
			 * @brief Relative drawing a line (spacial vector)
			 * @param[in] vect Vector of the curent line.
			 */
			void LineRel(vec3 vect);
			inline void LineRel(const vec2& vect) { LineRel(vec3(vect.x(), vect.y(), 0)); };
			/**
			 * @brief Draw a 2D rectangle to the position requested.
			 * @param[in] dest Position the the end of the rectangle
			 */
			void Rectangle(vec3 dest);
			inline void Rectangle(const vec2& dest) { Rectangle(vec3(dest.x(), dest.y(), 0)); };
			/**
			 * @brief Draw a 2D rectangle to the requested size.
			 * @param[in] width size of the rectangle
			 */
			void RectangleWidth(vec3 size);
			inline void RectangleWidth(const vec2& size) { RectangleWidth(vec3(size.x(), size.y(), 0)); };
			/**
			 * @brief Draw a 3D rectangle to the position requested.
			 * @param[in] dest Position the the end of the rectangle
			 */
			void Cube(vec3 dest);
			/**
			 * @brief Draw a 2D circle with the specify rafdius parameter.
			 * @param[in] radius Distence to the dorder
			 * @param[in] angleStart start angle of this circle ([0..2PI] otherwithe ==> disable)
			 * @param[in] angleStop stop angle of this circle ([0..2PI] otherwithe ==> disable)
			 */
			void Circle(float radius, float angleStart = 0, float angleStop = 2*M_PI);
	};
};

#endif

