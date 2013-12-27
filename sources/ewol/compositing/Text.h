/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_TEXT_H__
#define __EWOL_COMPOSITING_TEXT_H__

#include <etk/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/resource/TexturedFont.h>
#include <exml/exml.h>
#include <string>

namespace ewol {
	namespace compositing {
		/**
		 * @brief This class represent the specific display for every char in the string ...
		 * @not-in-doc
		 */
		class TextDecoration {
			public:
				etk::Color<> m_colorBg; //!< display background color
				etk::Color<> m_colorFg; //!< display foreground color
				enum ewol::font::mode m_mode; //!< display mode Regular/Bold/Italic/BoldItalic
				TextDecoration(void) {
					m_colorBg = etk::color::blue;
					m_colorBg = etk::color::green;
					m_mode = ewol::font::Regular;
				}
		};
		
		class Text : public ewol::Compositing {
			public:
				enum aligneMode {
					alignDisable,
					alignRight,
					alignLeft,
					alignCenter,
					alignJustify
				};
			private:
				ewol::compositing::Drawing m_vectorialDraw; //!< This is used to draw background selection and other things ...
			public:
				ewol::compositing::Drawing& getDrawing(void) {
					return m_vectorialDraw;
				};
			private:
				int32_t m_nbCharDisplayed; //!< prevent some error in calculation size.
				vec3 m_sizeDisplayStart; //!< The start windows of the display.
				vec3 m_sizeDisplayStop; //!< The end windows of the display.
				bool m_needDisplay; //!< This just need the display and not the size rendering.
				vec3 m_position; //!< The current position to draw
				vec3 m_clippingPosStart; //!< Clipping start position
				vec3 m_clippingPosStop; //!< Clipping stop position
				bool m_clippingEnable; //!< true if the clipping must be activated
			private:
				etk::Color<> m_color; //!< The text foreground color
				etk::Color<> m_colorBg; //!< The text background color
				etk::Color<> m_colorCursor; //!< The text cursor color
				etk::Color<> m_colorSelection; //!< The text Selection color
			private:
				enum ewol::font::mode m_mode; //!< font display property : Regular/Bold/Italic/BoldItalic
				bool m_kerning; //!< Kerning enable or disable on the next elements displayed
				bool m_distanceField; //!< Texture in distance Field mode  == > maybe move this in the font property.
				char32_t m_previousCharcode; //!< we remember the previous charcode to perform the kerning. @ref Kerning
			private:
				float m_startTextpos; //!< start position of the Alignement (when \n the text return at this position)
				float m_stopTextPos; //!< end of the alignement (when a string is too hight it cut at the word previously this virtual line and the center is perform with this one)
				enum aligneMode m_alignement; //!< Current Alignement mode (justify/left/right ...)
			private:
				ewol::resource::Program* m_GLprogram; //!< pointer on the opengl display program
				int32_t m_GLPosition; //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix; //!< openGL id on the element (transformation matrix)
				int32_t m_GLColor; //!< openGL id on the element (color buffer)
				int32_t m_GLtexture; //!< openGL id on the element (Texture position)
				int32_t m_GLtexID; //!< openGL id on the element (texture ID)
			private:
				int32_t m_selectionStartPos; //!< start position of the Selection (if == m_cursorPos ==> no selection)
				int32_t m_cursorPos; //!< Cursor position (default no cursor  == > -100)
			private:
				ewol::resource::TexturedFont* m_font; //!< Font resources
			private: // Text
				std::vector<vec2 > m_coord; //!< internal coord of the object
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
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				Text(const std::string& _fontName="", int32_t _fontSize=-1);
				/**
				 * @brief generic destructor
				 */
				~Text(void);
			public: // Derived function
				virtual void translate(const vec3& _vect);
				virtual void rotate(const vec3& _vect, float _angle);
				virtual void scale(const vec3& _vect);
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @previous
				 */
				void draw(const mat4& _transformationMatrix, bool _enableDepthTest=false);
				/**
				 * @brief clear all the registered element in the current element
				 */
				void clear(void);
				/**
				 * @brief clear all the intermediate result detween 2 prints
				 */
				void reset(void);
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
				void setPos(const vec3& _pos);
				/**
				 * @previous
				 */
				inline void setPos(const vec2& _pos) {
					setPos(vec3(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos(const vec3& _pos);
				/**
				 * @previous
				 */
				inline void setRelPos(const vec2& _pos) {
					setRelPos(vec3(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set the Color of the current foreground font
				 * @param[in] _color Color to set on foreground (for next print)
				 */
				void setColor(const etk::Color<>& _color) { m_color = _color; };
				/**
				 * @brief set the background color of the font (for selected Text (not the global BG))
				 * @param[in] _color Color to set on background (for next print)
				 */
				void setColorBg(const etk::Color<>& _color);
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _width Width size of the clipping
				 */
				void setClippingWidth(const vec3& _pos, const vec3& _width) {
					setClipping(_pos, _pos+_width);
				}
				/**
				 * @previous
				 */
				void setClippingWidth(const vec2& _pos, const vec2& _width) {
					setClipping(_pos, _pos+_width);
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _posEnd End position of the clipping
				 */
				void setClipping(const vec3& _pos, const vec3& _posEnd);
				/**
				 * @previous
				 */
				void setClipping(const vec2& _pos, const vec2& _posEnd) {
					setClipping(vec3(_pos.x(),_pos.y(),-1), vec3(_posEnd.x(),_posEnd.y(),1) );
				};
				/**
				 * @brief enable/Disable the clipping (without lose the current clipping position)
				 * @brief _newMode The new status of the clipping
				 */
				// TODO : Rename setClippingActivity
				void setClippingMode(bool _newMode);
				/**
				 * @brief Specify the font size (this reset the internal element of the current text (system requirement)
				 * @param[in] _fontSize New font size
				 */
				void setFontSize(int32_t _fontSize);
				/**
				 * @brief Specify the font name (this reset the internal element of the current text (system requirement)
				 * @param[in] _fontName Current name of the selected font
				 */
				void setFontName(const std::string& _fontName);
				/**
				 * @brief Specify the font property (this reset the internal element of the current text (system requirement)
				 * @param[in] fontName Current name of the selected font
				 * @param[in] fontSize New font size
				 */
				void setFont(std::string _fontName, int32_t _fontSize);
				/**
				 * @brief Specify the font mode for the next @ref print
				 * @param[in] mode The font mode requested
				 */
				void setFontMode(enum ewol::font::mode _mode);
				/**
				 * @brief get the current font mode
				 * @return The font mode applied
				 */
				enum ewol::font::mode getFontMode(void);
				/**
				 * @brief enable or disable the bold mode
				 * @param[in] _status The new status for this display property
				 */
				void setFontBold(bool _status);
				/**
				 * @brief enable or disable the italic mode
				 * @param[in] _status The new status for this display property
				 */
				void setFontItalic(bool _status);
				/**
				 * @brief set the activation of the Kerning for the display (if it existed)
				 * @param[in] _newMode enable/Diasable the kerning on this font.
				 */
				void setKerningMode(bool _newMode);
				/**
				 * @brief Request the distance field mode for this text display
				 * @param[in] _newMode enable/Diasable the Distance Field on this font.
				 * @todo : not implemented for now
				 */
				void setDistanceFieldMode(bool _newMode);
				/**
				 * @brief display a compleat string in the current element.
				 * @param[in] _text The string to display.
				 */
				void print(const std::string& _text);
				/**
				 * @previous
				 */
				void print(const std::u32string& _text);
				/**
				 * @brief display a compleat string in the current element with the generic decoration specification. (basic html data)
				 * 
				 * [code style=xml]
				 * <br/>
				 * <br/><br/><br/>
				 * <center>
				 * 	text exemple <b>in bold</b> other text <b>bold part <i>boldItalic part</i></b> an other thext
				 * 	<font color="#FF0000">colored text <b>bold color text</b> <i>bold italic text</i> normal color text</font> the end of the string<br/>
				 * 	an an other thext
				 * </center>
				 * <br/><br/><br/>
				 * <left>
				 * 	plop 1
				 * </left>
				 * <br/><br/><br/>
				 * <right>
				 * 	plop 2
				 * </right>
				 * <br/><br/><br/>
				 * <justify>
				 * 	Un exemple de text
				 * </justify>
				 * [/code]
				 * 
				 * @note This is parsed with tiny xml, then be carfull that the XML is correct, and all balises are closed ... otherwite the display can not be done
				 * @param[in] _text The string to display.
				 * @TODO : implementation not done ....
				 */
				void printDecorated(const std::string& _text);
				/**
				 * @brief display a compleat string in the current element with the generic decoration specification. (basic html data)
				 * 
				 * [code style=xml]
				 * <html>
				 * 	<body>
				 * 		<br/>
				 * 		<br/><br/><br/>
				 * 		<center>
				 * 			text exemple <b>in bold</b> other text <b>bold part <i>boldItalic part</i></b> an other thext
				 * 			<font color="#FF0000">colored text <b>bold color text</b> <i>bold italic text</i> normal color text</font> the end of the string<br/>
				 * 			an an other thext
				 * 		</center>
				 * 		<br/><br/><br/>
				 * 		<left>
				 * 			plop 1
				 * 		</left>
				 * 		<br/><br/><br/>
				 * 		<right>
				 * 			plop 2
				 * 		</right>
				 * 		<br/><br/><br/>
				 * 		<justify>
				 * 			Un exemple de text
				 * 		</justify>
				 * 	</body>
				 * </html>
				 * [/code]
				 * 
				 * @note This is parsed with tiny xml, then be carfull that the XML is correct, and all balises are closed ... otherwite the display can not be done
				 * @param[in] _text The string to display.
				 * @TODO : implementation not done ....
				 */
				void printHTML(const std::string& _text);
				/**
				 * @brief display a compleat string in the current element whith specific decorations (advence mode).
				 * @param[in] _text The string to display.
				 * @param[in] _decoration The text decoration for the text that might be display (if the vector is smaller, the last parameter is get)
				 */
				void print(const std::string& _text, const std::vector<TextDecoration>& _decoration);
				/**
				 * @previous
				 */
				void print(const std::u32string& _text, const std::vector<TextDecoration>& _decoration);
				/**
				 * @brief display the current char in the current element (note that the kerning is availlable if the position is not changed)
				 * @param[in] _charcode Char that might be dispalyed
				 */
				void print(const char32_t& _charcode);
				/**
				 * @brief This generate the line return  == > it return to the alignement position start and at the correct line position ==> it might be use to not know the line height
				 */
				void forceLineReturn(void);
			private:
				/**
				 * @brief This parse a tinyXML node (void pointer to permit to hide tiny XML in include).
				 * @param[in] _element the exml element.
				 */
				void parseHtmlNode(exml::Element* _element);
			public:
				/**
				 * @brief This generate the possibility to generate the big text property
				 * @param[in] _startTextpos The x text start position of the display.
				 * @param[in] _stopTextPos The x text stop position of the display.
				 * @param[in] _alignement mode of alignement for the Text.
				 * @note The text align in center change of line every display done (even if it was just a char)
				 */
				void setTextAlignement(float _startTextpos, float _stopTextPos, enum ewol::compositing::Text::aligneMode _alignement=ewol::compositing::Text::alignDisable);
				/**
				 * @brief disable the alignement system
				 */
				void disableAlignement(void);
				/**
				 * @brief get the current alignement property
				 * @return the curent alignement type
				 */
				enum ewol::compositing::Text::aligneMode getAlignement(void);
				/**
				 * @brief calculate a theoric text size
				 * @param[in] _text The string to calculate dimention.
				 * @return The theoric size used.
				 */
				vec3 calculateSizeHTML(const std::string& _text);
				/**
				 * @brief calculate a theoric text size
				 * @param[in] _text The string to calculate dimention.
				 * @return The theoric size used.
				 */
				vec3 calculateSizeDecorated(const std::string& _text);
				/**
				 * @brief calculate a theoric text size
				 * @param[in] _text The string to calculate dimention.
				 * @return The theoric size used.
				 */
				vec3 calculateSize(const std::string& _text);
				/**
				 * @brief calculate a theoric charcode size
				 * @param[in] _charcode The µUnicode value to calculate dimention.
				 * @return The theoric size used.
				 */
				vec3 calculateSize(const char32_t& _charcode);
				/**
				 * @brief draw a cursor at the specify position
				 * @param[in] _isInsertMode True if the insert mode is activated
				 * @param[in] _cursorSize The sizae of the cursor that might be set when insert mode is set [default 20]
				 */
				void printCursor(bool _isInsertMode, float _cursorSize = 20.0f);
			private:
				/**
				 * @brief calculate the element number that is the first out the alignement range 
				 *        (start at the specify ID, and use start pos with current one)
				 * @param[in] _text The string that might be parsed.
				 * @param[in] _start The first elemnt that might be used to calculate.
				 * @param[out] _stop The last Id availlable in the current string.
				 * @param[out] _space Number of space in the string.
				 * @param[out] _freespace This represent the number of pixel present in the right white space.
				 * @return true if the rifht has free space that can be use for jystify.
				 * @return false if we find '\n'
				 */
				bool extrapolateLastId(const std::string& _text, const int32_t _start, int32_t& _stop, int32_t& _space, int32_t& _freeSpace);
				/**
				 * @previous
				 */
				bool extrapolateLastId(const std::u32string& _text, const int32_t _start, int32_t& _stop, int32_t& _space, int32_t& _freeSpace);
			private:
				// this section is reserved for HTML parsing and display:
				std::string m_htmlCurrrentLine; //!< current line for HTML display
				std::vector<TextDecoration> m_htmlDecoration; //!< current decoration for the HTML display
				TextDecoration m_htmlDecoTmp; //!< current decoration
				/**
				 * @brief add a line with the current m_htmlDecoTmp decoration
				 * @param[in] _data The cuurent data to add.
				 */
				void htmlAddData(const std::string& _data);
				/**
				 * @brief draw the current line
				 */
				void htmlFlush(void);
			public:
				/**
				 * @brief remove the cursor display
				 */
				void disableCursor(void);
				/**
				 * @brief set a cursor at a specific position:
				 * @param[in] _cursorPos id of the cursor position
				 */
				void setCursorPos(int32_t _cursorPos);
				/**
				 * @brief set a cursor at a specific position with his associated selection:
				 * @param[in] _cursorPos id of the cursor position
				 * @param[in] _selectionStartPos id of the starting of the selection
				 */
				void setCursorSelection(int32_t _cursorPos, int32_t _selectionStartPos);
				/**
				 * @brief change the selection color
				 * @param[in] _color New color for the Selection
				 */
				void setSelectionColor(const etk::Color<>& _color);
				/**
				 * @brief change the cursor color
				 * @param[in] _color New color for the Selection
				 */
				void setCursorColor(const etk::Color<>& _color);
		};
	};
};

#endif

