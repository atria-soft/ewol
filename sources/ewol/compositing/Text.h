/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_TEXT_H__
#define __EWOL_COMPOSITING_TEXT_H__

#include <draw/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/renderer/ResourceManager.h>

namespace ewol
{
	/**
	 * @brief This class represent the specific display for every char in the string ...
	 */
	class TextDecoration
	{
		public:
			draw::Color         m_colorBg; //!< Display background color
			draw::Color         m_colorFg; //!< Display foreground color
			ewol::font::mode_te m_mode;    //!< Display mode Regular/Bold/Italic/BoldItalic
			TextDecoration(void)
			{
				m_colorBg = draw::color::blue;
				m_colorBg = draw::color::green;
				m_mode = ewol::font::Regular;
				
			}
	};
	
	class Text : public ewol::Compositing
	{
		public:
			typedef enum {
				alignDisable,
				alignRight,
				alignLeft,
				alignCenter,
				alignJustify
			} aligneMode_te;
		private:
			ewol::Drawing        m_vectorialDraw;      //!< This is used to draw background selection and other things ...
		private:
			int32_t              m_nbCharDisplayed;    //!< prevent some error in calculation size.
			etk::Vector3D<float> m_sizeDisplayStart;   //!< The start windows of the display.
			etk::Vector3D<float> m_sizeDisplayStop;    //!< The end windows of the display.
			bool                 m_needDisplay;        //!< This just need the display and not the size rendering.
			
			etk::Vector3D<float> m_position;           //!< The current position to draw
			etk::Vector3D<float> m_clippingPosStart;   //!< Clipping start position
			etk::Vector3D<float> m_clippingPosStop;    //!< Clipping stop position
			bool                 m_clippingEnable;     //!< true if the clipping must be activated
		private:
			draw::Color          m_color;              //!< The text foreground color
			draw::Color          m_colorBg;            //!< The text background color
			draw::Color          m_colorCursor;        //!< The text cursor color
			draw::Color          m_colorSelection;     //!< The text Selection color
		private:
			ewol::font::mode_te  m_mode;               //!< font display property : Regular/Bold/Italic/BoldItalic
			bool                 m_kerning;            //!< Kerning enable or disable on the next elements displayed
			bool                 m_distanceField;      //!< Texture in distance Field mode ==> maybe move this in the font property.
			uniChar_t            m_previousCharcode;   //!< we remember the previous charcode to perform the kerning. @ref Kerning
		private:
			float                m_startTextpos;       //!< start position of the Alignement (when \n the text return at this position)
			float                m_stopTextPos;        //!< end of the alignement (when a string is too hight it cut at the word previously this virtual line and the center is perform with this one)
			aligneMode_te        m_alignement;         //!< Current Alignement mode (justify/left/right ...)
		private:
			ewol::Program*       m_GLprogram;          //!< pointer on the opengl display program
			int32_t              m_GLPosition;         //!< openGL id on the element (vertex buffer)
			int32_t              m_GLMatrix;           //!< openGL id on the element (transformation matrix)
			int32_t              m_GLColor;            //!< openGL id on the element (color buffer)
			int32_t              m_GLtexture;          //!< openGL id on the element (Texture position)
			int32_t              m_GLtexID;            //!< openGL id on the element (texture ID)
		private:
			int32_t m_selectionStartPos; //!< start position of the Selection (if == m_cursorPos ==> no selection)
			int32_t m_cursorPos;         //!< Cursor position (default no cursor ==> -100)
		private:
			ewol::TexturedFont*                  m_font;          //!< Font resources
		private: // Text
			etk::Vector<etk::Vector2D<float> >   m_coord;         //!< internal coord of the object
			etk::Vector<etk::Vector2D<float> >   m_coordTex;      //!< internal texture coordinate for every point
			etk::Vector<draw::Colorf>            m_coordColor;    //!< internal color of the different point
		private:
			/**
			 * @brief Load the openGL program and get all the ID needed
			 */
			void LoadProgram(void);
		public:
			/**
			 * @brief generic constructor
			 */
			Text(void);
			/**
			 * @brief generic constructor
			 * @param[in] fontName Name of the font that might be loaded
			 * @param[in] fontSize Size of the font that might be loaded
			 */
			Text(etk::UString fontName, int32_t fontSize);
			/**
			 * @brief generic destructor
			 */
			~Text(void);
		public:
			// Derived function
			virtual void Translate(etk::Vector3D<float> vect);
			// Derived function
			virtual void Rotate(etk::Vector3D<float> vect, float angle);
			// Derived function
			virtual void Scale(etk::Vector3D<float> vect);
		public:
			/**
			 * @brief Draw All the refistered text in the current element on openGL
			 */
			void Draw(void);
			/**
			 * @brief Clear all the registered element in the current element
			 */
			void Clear(void);
			/**
			 * @brief Clear all the intermediate result detween 2 prints
			 */
			void Reset(void);
			/**
			 * @brief Get the current display position (sometime needed in the gui control)
			 * @return the current position.
			 */
			etk::Vector3D<float> GetPos(void);
			/**
			 * @brief Set position for the next text writen
			 * @param[in] pos Position of the text (in 3D)
			 */
			void SetPos(etk::Vector3D<float> pos);
			/**
			 * @brief Set relative position for the next text writen
			 * @param[in] pos ofset apply of the text (in 3D)
			 */
			void SetRelPos(etk::Vector3D<float> pos);
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
			void SetClippingWidth(etk::Vector3D<float> pos, etk::Vector3D<float> width);
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] pos Start position of the clipping
			 * @param[in] posEnd End position of the clipping
			 */
			void SetClipping(etk::Vector3D<float> pos, etk::Vector3D<float> posEnd);
			/**
			 * @brief Enable/Disable the clipping (without lose the current clipping position)
			 * @brief newMode The new status of the clipping
			 */
			// TODO : Rename SetClippingActivity
			void SetClippingMode(bool newMode);
			/**
			 * @brief Specify the font size (this reset the internal element of the current text (system requirement)
			 * @param[in] fontSize New font size
			 */
			void SetFontSize(int32_t fontSize);
			/**
			 * @brief Specify the font name (this reset the internal element of the current text (system requirement)
			 * @param[in] fontName Current name of the selected font
			 */
			void SetFontName(etk::UString fontName);
			/**
			 * @brief Specify the font property (this reset the internal element of the current text (system requirement)
			 * @param[in] fontName Current name of the selected font
			 * @param[in] fontSize New font size
			 */
			void SetFont(etk::UString fontName, int32_t fontSize);
			/**
			 * @brief Specify the font mode for the next @ref Print
			 * @param[in] mode The font mode requested
			 */
			void SetFontMode(ewol::font::mode_te mode);
			/**
			 * @brief Get the current font mode
			 * @return The font mode applied
			 */
			ewol::font::mode_te GetFontMode(void);
			/**
			 * @brief Enable or disable the bold mode
			 * @param[in] status The new status for this display property
			 */
			void SetFontBold(bool status);
			/**
			 * @brief Enable or disable the italic mode
			 * @param[in] status The new status for this display property
			 */
			void SetFontItalic(bool status);
			/**
			 * @brief Set the activation of the Kerning for the display (if it existed)
			 * @param[in] newMode Enable/Diasable the kerning on this font.
			 */
			void SetKerningMode(bool newMode);
			/**
			 * @brief Request the distance field mode for this text display
			 * @param[in] newMode Enable/Diasable the Distance Field on this font.
			 * @todo : not implemented for now
			 */
			void SetDistanceFieldMode(bool newMode);
			/**
			 * @brief Display a compleat string in the current element.
			 * @param[in] text The string to display.
			 */
			void Print(const etk::UString& text);
			/**
			 * @brief Display a compleat string in the current element with the generic decoration specification. (basic html data)
			 * <pre>
			 *	<br/>
			 *	<br/><br/><br/>
			 *	<center>
			 *		text exemple <b>in bold</b> other text <b>bold part <i>boldItalic part</i></b> an other thext
			 *		<font color=\"#FF0000\">colored text <b>bold color text</b> <i>bold italic text</i> normal color text</font> the end of the string<br/>
			 *		an an other thext
			 *	</center>
			 *	<br/><br/><br/>
			 *	<left>
			 *		plop 1
			 *	</left>
			 *	<br/><br/><br/>
			 *	<right>
			 *		plop 2
			 *	</right>
			 *	<br/><br/><br/>
			 *	<justify>
			 *		Un exemple de text
			 *	</justify>
			 * </pre>
			 * @note This is parsed with tiny xml, then be carfull that the XML is correct, and all balises are closed ... otherwite the display can not be done
			 * @param[in] text The string to display.
			 * @TODO : implementation not done ....
			 */
			void PrintDecorated(etk::UString& text);
			/**
			 * @brief Display a compleat string in the current element with the generic decoration specification. (basic html data)
			 * <pre>
			 *	<html>
			 *		<body>
			 *			<br/>
			 *			<br/><br/><br/>
			 *			<center>
			 *				text exemple <b>in bold</b> other text <b>bold part <i>boldItalic part</i></b> an other thext
			 *				<font color=\"#FF0000\">colored text <b>bold color text</b> <i>bold italic text</i> normal color text</font> the end of the string<br/>
			 *				an an other thext
			 *			</center>
			 *			<br/><br/><br/>
			 *			<left>
			 *				plop 1
			 *			</left>
			 *			<br/><br/><br/>
			 *			<right>
			 *				plop 2
			 *			</right>
			 *			<br/><br/><br/>
			 *			<justify>
			 *				Un exemple de text
			 *			</justify>
			 *		</body>
			 *	</html>
			 * </pre>
			 * @note This is parsed with tiny xml, then be carfull that the XML is correct, and all balises are closed ... otherwite the display can not be done
			 * @param[in] text The string to display.
			 * @TODO : implementation not done ....
			 */
			void PrintHTML(etk::UString text);
			/**
			 * @brief Display a compleat string in the current element whith specific decorations (advence mode).
			 * @param[in] text The string to display.
			 * @param[in] decoration The text decoration for the text that might be display (if the vector is smaller, the last parameter is get)
			 */
			void Print(const etk::UString& text, const etk::Vector<TextDecoration>& decoration);
			/**
			 * @brief Display the current char in the current element (note that the kerning is availlable if the position is not changed)
			 * @param[in] char that might be dispalyed
			 */
			void Print(const uniChar_t charcode);
			/**
			 * @brief This Generate the line return ==> it return to the alignement position start and at the correct line position ==> it might be use to not know the line height
			 */
			void ForceLineReturn(void);
		private:
			/**
			 * @brief This parse a tinyXML node (void pointer to permit to hide tiny XML in include).
			 * @param[in] element the tynyXML element : TiXmlNode* .
			 */
			void ParseHtmlNode(void* element);
		public:
			/**
			 * @brief This generate the possibility to generate the big text property
			 * @param[in] startTextpos The x text start position of the display.
			 * @param[in] stopTextPos The x text stop position of the display.
			 * @param[in] alignement mode of alignement for the Text.
			 * @note The text align in center change of line every display done (even if it was just a char)
			 */
			void SetTextAlignement(float startTextpos, float stopTextPos, ewol::Text::aligneMode_te alignement=ewol::Text::alignDisable);
			/**
			 * @brief Disable the alignement system
			 */
			void DisableAlignement(void);
			/**
			 * @brief Get the current alignement property
			 * @return the curent alignement type
			 */
			ewol::Text::aligneMode_te GetAlignement(void);
			/**
			 * @brief Calculate a theoric text size
			 * @param[in] text The string to calculate dimention.
			 * @return The theoric size used.
			 */
			etk::Vector3D<float> CalculateSizeHTML(const etk::UString& text);
			/**
			 * @brief Calculate a theoric text size
			 * @param[in] text The string to calculate dimention.
			 * @return The theoric size used.
			 */
			etk::Vector3D<float> CalculateSizeDecorated(const etk::UString& text);
			/**
			 * @brief Calculate a theoric text size
			 * @param[in] text The string to calculate dimention.
			 * @return The theoric size used.
			 */
			etk::Vector3D<float> CalculateSize(const etk::UString& text);
			/**
			 * @brief Calculate a theoric charcode size
			 * @param[in] charcode The µUnicode value to calculate dimention.
			 * @return The theoric size used.
			 */
			etk::Vector3D<float> CalculateSize(const uniChar_t charcode);
			/**
			 * @brief Draw a cursor at the specify position
			 * @param[in] isInsertMode True if the insert mode is activated
			 */
			void PrintCursor(bool isInsertMode);
		private:
			/**
			 * @brief Calculate the element number that is the first out the alignement range 
			 *        (start at the specify ID, and use start pos with current one)
			 * @param[in] text The string that might be parsed.
			 * @param[in] start The first elemnt that might be used to calculate.
			 * @param[out] stop The last Id availlable in the current string.
			 * @param[out] space Number of space in the string.
			 * @parma[out] freespace This represent the number of pixel present in the right white space.
			 * @return true if the rifht has free space that can be use for jystify (return false if we find \n
			 */
			bool ExtrapolateLastId(const etk::UString& text, const int32_t start, int32_t& stop, int32_t& space, int32_t& freeSpace);
		private:
			// this section is reserved for HTML parsing and display:
			etk::UString                 m_htmlCurrrentLine; //!< current line for HTML display
			etk::Vector<TextDecoration>  m_htmlDecoration;   //!< current decoration for the HTML display
			TextDecoration               m_htmlDecoTmp;      //!< current decoration
			/**
			 * @brief add a line with the current m_htmlDecoTmp decoration
			 * @param[in] the cuurent data to add.
			 */
			void HtmlAddData(etk::UString data);
			/**
			 * @brief Draw the current line
			 */
			void HtmlFlush(void);
		public:
			/**
			 * @brief Remove the cursor display
			 */
			void DisableCursor(void);
			/**
			 * @brief Set a cursor at a specific position:
			 * @param[in] cursorPos id of the cursor position
			 */
			void SetCursorPos(int32_t cursorPos);
			/**
			 * @brief Set a cursor at a specific position with his associated selection:
			 * @param[in] cursorPos id of the cursor position
			 * @param[in] selectionStartPos id of the starting of the selection
			 */
			void SetCursorSelection(int32_t cursorPos, int32_t selectionStartPos);
			/**
			 * @brief Change the selection color
			 * @param[in] color New color for the Selection
			 */
			void SetSelectionColor(draw::Color color);
			/**
			 * @brief Change the cursor color
			 * @param[in] color New color for the Selection
			 */
			void SetCursorColor(draw::Color color);
	};
};

#endif

