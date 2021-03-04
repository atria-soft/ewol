/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Color.hpp>

#include <ewol/debug.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/resource/TexturedFont.hpp>
#include <exml/exml.hpp>
#include <etk/String.hpp>

namespace ewol {
	namespace compositing {
		/**
		 * @brief This class represent the specific display for every char in the string ...
		 * @not_in_doc
		 */
		class TextDecoration {
			public:
				etk::Color<float,4> this.colorBg; //!< display background color
				etk::Color<float,4> this.colorFg; //!< display foreground color
				enum ewol::font::mode this.mode; //!< display mode Regular/Bold/Italic/BoldItalic
				TextDecoration() {
					this.colorBg = etk::color::blue;
					this.colorBg = etk::color::green;
					this.mode = ewol::font::Regular;
				}
		};
		
		enum aligneMode {
			alignDisable,
			alignRight,
			alignLeft,
			alignCenter,
			alignJustify
		};
		
		class TextBase : public ewol::Compositing {
			protected:
				ewol::compositing::Drawing this.vectorialDraw; //!< This is used to draw background selection and other things ...
			public:
				 ewol::compositing::Drawing getDrawing() {
					return this.vectorialDraw;
				};
			protected:
				int this.nbCharDisplayed; //!< prevent some error in calculation size.
				Vector3f this.sizeDisplayStart; //!< The start windows of the display.
				Vector3f this.sizeDisplayStop; //!< The end windows of the display.
				boolean this.needDisplay; //!< This just need the display and not the size rendering.
				Vector3f this.position; //!< The current position to draw
				Vector3f this.clippingPosStart; //!< Clipping start position
				Vector3f this.clippingPosStop; //!< Clipping stop position
				boolean this.clippingEnable; //!< true if the clipping must be activated
			protected:
				etk::Color<float,4> this.defaultColorFg; //!< The text foreground color
				etk::Color<float,4> this.defaultColorBg; //!< The text background color
			protected:
				etk::Color<float,4> this.color; //!< The text foreground color
				etk::Color<float,4> this.colorBg; //!< The text background color
				etk::Color<float,4> this.colorCursor; //!< The text cursor color
				etk::Color<float,4> this.colorSelection; //!< The text Selection color
			protected:
				enum ewol::font::mode this.mode; //!< font display property : Regular/Bold/Italic/BoldItalic
				boolean this.kerning; //!< Kerning enable or disable on the next elements displayed
				Character this.previousCharcode; //!< we remember the previous charcode to perform the kerning. @ref Kerning
			protected:
				float this.startTextpos; //!< start position of the Alignement (when \n the text return at this position)
				float this.stopTextPos; //!< end of the alignement (when a string is too hight it cut at the word previously this  line and the center is perform with this one)
				enum aligneMode this.alignement; //!< Current Alignement mode (justify/left/right ...)
			protected:
				ememory::Ptr<gale::resource::Program> this.GLprogram; //!< pointer on the opengl display program
				int this.GLPosition; //!< openGL id on the element (vertex buffer)
				int this.GLMatrix; //!< openGL id on the element (transformation matrix)
				int this.GLColor; //!< openGL id on the element (color buffer)
				int this.GLtexture; //!< openGL id on the element (Texture position)
				int this.GLtexID; //!< openGL id on the element (texture ID)
				int this.GLtextWidth; //!< openGL Id on the texture width
				int this.GLtextHeight; //!< openGL Id on the texture height
			protected:
				int this.selectionStartPos; //!< start position of the Selection (if == this.cursorPos ==> no selection)
				int this.cursorPos; //!< Cursor position (default no cursor  == > -100)
			protected: // Text
				static  int this.vboIdCoord;
				static  int this.vboIdCoordText;
				static  int this.vboIdColor;
				static  int this.vboIdGlyphLevel;
				ememory::Ptr<gale::resource::VirtualBufferObject> this.VBO;
			public:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				 void loadProgram( String _shaderName);
			public:
				/**
				 * @brief generic ructor
				 */
				TextBase( String _shaderName = "DATA:///text.prog?lib=ewol", boolean _loadProgram = true);
				/**
				 * @brief generic destructor
				 */
				 ~TextBase();
			public: // Derived function
				void translate( Vector3f _vect);
				void rotate( Vector3f _vect, float _angle);
				void scale( Vector3f _vect);
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(boolean _disableDepthTest=true) {
					drawD(_disableDepthTest);
				}
				//! @previous
				void draw( mat4 _transformationMatrix, boolean _enableDepthTest=false) {
					drawMT(_transformationMatrix, _enableDepthTest);
				}
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				 void drawD(boolean _disableDepthTest) = 0;
				//! @previous
				 void drawMT( mat4 _transformationMatrix, boolean _enableDepthTest) = 0;
				/**
				 * @brief clear all the registered element in the current element
				 */
				 void clear();
				/**
				 * @brief clear all the intermediate result detween 2 prints
				 */
				 void reset();
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
				void setPos( Vector3f _pos);
				//! @previous
				 void setPos( Vector2f _pos) {
					setPos(Vector3f(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos( Vector3f _pos);
				//! @previous
				 void setRelPos( Vector2f _pos) {
					setRelPos(Vector3f(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set the default background color of the font (when reset, set this value ...)
				 * @param[in] _color Color to set on background
				 */
				void setDefaultColorBg( etk::Color<> _color) {
					this.defaultColorBg = _color;
				}
				/**
				 * @brief set the default Foreground color of the font (when reset, set this value ...)
				 * @param[in] _color Color to set on foreground
				 */
				void setDefaultColorFg( etk::Color<> _color) {
					this.defaultColorFg = _color;
				}
				/**
				 * @brief set the Color of the current foreground font
				 * @param[in] _color Color to set on foreground (for next print)
				 */
				void setColor( etk::Color<> _color) {
					this.color = _color;
				};
				/**
				 * @brief set the background color of the font (for selected Text (not the global BG))
				 * @param[in] _color Color to set on background (for next print)
				 */
				void setColorBg( etk::Color<> _color);
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _width Width size of the clipping
				 */
				void setClippingWidth( Vector3f _pos,  Vector3f _width) {
					setClipping(_pos, _pos+_width);
				}
				//! @previous
				void setClippingWidth( Vector2f _pos,  Vector2f _width) {
					setClipping(_pos, _pos+_width);
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _posEnd End position of the clipping
				 */
				void setClipping( Vector3f _pos,  Vector3f _posEnd);
				//! @previous
				void setClipping( Vector2f _pos,  Vector2f _posEnd) {
					setClipping(Vector3f(_pos.x(),_pos.y(),-1), Vector3f(_posEnd.x(),_posEnd.y(),1) );
				};
				/**
				 * @brief enable/Disable the clipping (without lose the current clipping position)
				 * @brief _newMode The new status of the clipping
				 */
				// TODO : Rename setClippingActivity
				void setClippingMode(boolean _newMode);
				/**
				 * @brief Specify the font size (this reset the internal element of the current text (system requirement)
				 * @param[in] _fontSize New font size
				 */
				 void setFontSize(int _fontSize) = 0;
				/**
				 * @brief Specify the font name (this reset the internal element of the current text (system requirement)
				 * @param[in] _fontName Current name of the selected font
				 */
				 void setFontName( String _fontName) = 0;
				/**
				 * @brief Specify the font property (this reset the internal element of the current text (system requirement)
				 * @param[in] fontName Current name of the selected font
				 * @param[in] fontSize New font size
				 */
				 void setFont(String _fontName, int _fontSize) = 0;
				/**
				 * @brief Specify the font mode for the next @ref print
				 * @param[in] mode The font mode requested
				 */
				 void setFontMode(enum ewol::font::mode _mode) = 0;
				/**
				 * @brief get the current font mode
				 * @return The font mode applied
				 */
				enum ewol::font::mode getFontMode() {
					return this.mode;
				};
				 float getHeight() = 0;
				 float getSize() = 0;
				 ewol::GlyphProperty * getGlyphPointer(Character _charcode) = 0;
				/**
				 * @brief enable or disable the bold mode
				 * @param[in] _status The new status for this display property
				 */
				void setFontBold(boolean _status);
				/**
				 * @brief enable or disable the italic mode
				 * @param[in] _status The new status for this display property
				 */
				void setFontItalic(boolean _status);
				/**
				 * @brief set the activation of the Kerning for the display (if it existed)
				 * @param[in] _newMode enable/Diasable the kerning on this font.
				 */
				void setKerningMode(boolean _newMode);
				/**
				 * @brief display a compleat string in the current element.
				 * @param[in] _text The string to display.
				 */
				void print( String _text);
				//! @previous
				void print( etk::UString _text);
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
				void printDecorated( String _text);
				//! @previous
				void printDecorated( etk::UString _text);
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
				void printHTML( String _text);
				//! @previous
				void printHTML( etk::UString _text);
				/**
				 * @brief display a compleat string in the current element whith specific decorations (advence mode).
				 * @param[in] _text The string to display.
				 * @param[in] _decoration The text decoration for the text that might be display (if the vector is smaller, the last parameter is get)
				 */
				void print( String _text,  List<TextDecoration> _decoration);
				//! @previous
				void print( etk::UString _text,  List<TextDecoration> _decoration);
				/**
				 * @brief display the current char in the current element (note that the kerning is availlable if the position is not changed)
				 * @param[in] _charcode Char that might be dispalyed
				 */
				 void printChar( Character _charcode) = 0;
				/**
				 * @brief This generate the line return  == > it return to the alignement position start and at the correct line position ==> it might be use to not know the line height
				 */
				void forceLineReturn();
			protected:
				/**
				 * @brief This parse a tinyXML node (void pointer to permit to hide tiny XML in include).
				 * @param[in] _element the exml element.
				 */
				void parseHtmlNode( exml::Element _element);
			public:
				/**
				 * @brief This generate the possibility to generate the big text property
				 * @param[in] _startTextpos The x text start position of the display.
				 * @param[in] _stopTextPos The x text stop position of the display.
				 * @param[in] _alignement mode of alignement for the Text.
				 * @note The text align in center change of line every display done (even if it was just a char)
				 */
				void setTextAlignement(float _startTextpos, float _stopTextPos, enum ewol::compositing::aligneMode _alignement=ewol::compositing::alignDisable);
				/**
				 * @brief disable the alignement system
				 */
				void disableAlignement();
				/**
				 * @brief get the current alignement property
				 * @return the curent alignement type
				 */
				enum ewol::compositing::aligneMode getAlignement();
				/**
				 * @brief calculate a theoric text size
				 * @param[in] _text The string to calculate dimention.
				 * @return The theoric size used.
				 */
				Vector3f calculateSizeHTML( String _text);
				//! @previous
				Vector3f calculateSizeHTML( etk::UString _text);
				/**
				 * @brief calculate a theoric text size
				 * @param[in] _text The string to calculate dimention.
				 * @return The theoric size used.
				 */
				Vector3f calculateSizeDecorated( String _text);
				//! @previous
				Vector3f calculateSizeDecorated( etk::UString _text);
				/**
				 * @brief calculate a theoric text size
				 * @param[in] _text The string to calculate dimention.
				 * @return The theoric size used.
				 */
				Vector3f calculateSize( String _text);
				//! @previous
				Vector3f calculateSize( etk::UString _text);
				/**
				 * @brief calculate a theoric charcode size
				 * @param[in] _charcode The Unicode value to calculate dimention.
				 * @return The theoric size used.
				 */
				 Vector3f calculateSize( Character _charcode) {
					return calculateSizeChar(_charcode);
				};
			protected:
				//! @previous
				 Vector3f calculateSizeChar( Character _charcode) = 0;
			public:
				/**
				 * @brief draw a cursor at the specify position
				 * @param[in] _isInsertMode True if the insert mode is activated
				 * @param[in] _cursorSize The sizae of the cursor that might be set when insert mode is set [default 20]
				 */
				void printCursor(boolean _isInsertMode, float _cursorSize = 20.0f);
			protected:
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
				boolean extrapolateLastId( String _text,  int _start, int _stop, int _space, int _freeSpace);
				//! @previous
				boolean extrapolateLastId( etk::UString _text,  int _start, int _stop, int _space, int _freeSpace);
			protected:
				// this section is reserved for HTML parsing and display:
				etk::UString this.htmlCurrrentLine; //!< current line for HTML display
				List<TextDecoration> this.htmlDecoration; //!< current decoration for the HTML display
				TextDecoration this.htmlDecoTmp; //!< current decoration
				/**
				 * @brief add a line with the current this.htmlDecoTmp decoration
				 * @param[in] _data The cuurent data to add.
				 */
				void htmlAddData( etk::UString _data);
				/**
				 * @brief draw the current line
				 */
				void htmlFlush();
			public:
				/**
				 * @brief remove the cursor display
				 */
				void disableCursor();
				/**
				 * @brief set a cursor at a specific position:
				 * @param[in] _cursorPos id of the cursor position
				 */
				void setCursorPos(int _cursorPos);
				/**
				 * @brief set a cursor at a specific position with his associated selection:
				 * @param[in] _cursorPos id of the cursor position
				 * @param[in] _selectionStartPos id of the starting of the selection
				 */
				void setCursorSelection(int _cursorPos, int _selectionStartPos);
				/**
				 * @brief change the selection color
				 * @param[in] _color New color for the Selection
				 */
				void setSelectionColor( etk::Color<> _color);
				/**
				 * @brief change the cursor color
				 * @param[in] _color New color for the Selection
				 */
				void setCursorColor( etk::Color<> _color);
		};
	}
}


