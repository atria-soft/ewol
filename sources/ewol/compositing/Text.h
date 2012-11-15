/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXT_H__
#define __EWOL_TEXT_H__

namespace ewol
{
	class TextDecoration
	{
		public:
			etk::Color          m_colorBg;
			etk::Color          m_colorFg;
			ewol::font::mode_te m_mode;
	}
	
	
	class Text : public ewol::Compositing
	{
		typedef enum {
			alignDisable,
			alignRight,
			alignLeft,
			alignCenter,
			alignJustify
		} aligneMode_te;
		
		protected:
			// curent Drawing position
			etk::Vector3D<float> m_position;         //!< the next position to draw the text
			// clipping section
			etk::Vector3D<float> m_clippingPosition;
			etk::Vector3D<float> m_clippingSize;
			bool                 m_clippingEnable;
			// Basic color
			etk::Color           m_color;
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
		public :
			/**
			 * @brief Draw All the refistered text in the current element on openGL
			 */
			void Draw(void);
			/**
			 * @brief Clear alll tre registered element in the current element
			 */
			void Clear(void);
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
			void SetColor(etk::Color color);
			/**
			 * @brief Set the background color of the font (for selected Text (not the global BG))
			 * @param[in] color Color to set on background (for next print)
			 */
			void SetColorBG(etk::Color color);
			/**
			 * @brief Request a clipping area for the text (next draw only)
			 * @param[in] pos   Start position of the clipping
			 * @param[in] width End position of th clipping
			 */
			void SetClipping(etk::Vector3D<float> pos, etk::Vector3D<float> width);
			/**
			 * @brief Enable/Disable the clipping (without lose the current clipping position)
			 * @brief newMode The new status of the clipping
			 */
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
			 * @brief Set the activation of the Kerning for the display (if it existed)
			 * @param[in] newMode Enable/Diasable the kerning on this font.
			 */
			void SetKerningMode(bool newMode);
			/**
			 * @brief Request the distance field mode for this text display
			 * @param[in] newMode Enable/Diasable the Distance Field on this font.
			 * @todo : not implemented for now
			 */
			void SetDistanceFieldMode(bool newMode) { };
			/**
			 * @brief Display a compleat string in the current element.
			 * @param[in] text The string to display.
			 */
			void Print(etk::UString& text);
			/**
			 * @brief Display a compleat string in the current element with the generic decoration specification.
			 *   \<b\> ... \</b\> For bold text.
			 *   \<i\> ... \</i\> For italic text.
			 *   \<color="#54325165"\> ... \</color\> To specify a color.
			 *   \<left\> ... \</left\> To align left.
			 *   \<right\> ... \</right\> To align right.
			 *   \<center\> ... \</center\> To align center.
			 *   \<justify\> ... \</justify\> To align justify.
			 * @param[in] text The string to display.
			 */
			void PrintDecorated(etk::UString& text);
			/**
			 * @brief Display a compleat string in the current element whith specific decorations (advence mode).
			 * @param[in] text The string to display.
			 * @param[in] decoration The text decoration for the text that might be display (if the vector is smaller, the last parameter is get)
			 */
			void Print(etk::UString& text, etk::Vector<TextDecoration>& decoration);
			/**
			 * @brief Display the current char in the current element (note that the kerning is availlable if the position is not changed)
			 * @param[in] char that might be dispalyed
			 */
			void Print(uniChar_t charcode);
			/**
			 * @brief This generate the possibility to generate the big text property
			 * @param[in] startTextpos The x text start position of the display.
			 * @param[in] stopTextPos The x text stop position of the display.
			 * @param[in] alignement mode of alignement for the Text.
			 * @note The text align in center change of line every display done (even if it was just a char)
			 */
			void SetTextAlignement(float startTextpos, float stopTextPos, aligneMode_te alignement);
			/**
			 * @brief Disable the alignement system
			 */
			void DisableAlignement(void);
	};
};

#endif

