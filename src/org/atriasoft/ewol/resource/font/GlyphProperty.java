/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/resource/font/Kerning.hpp>

namespace ewol {
	/*
	                             |            |          |            |            
	                             |            |          |            |            
	                             |            |          |            |            
	                      Y      |            |          |            |            
	                      ^      |------------|          |------------|            
	                      |                                                        
	     this.advance.y:/.  |                                                        
	                 |    |                                                        
	                 |    |                                                        
	this.sizeTex.x/.   |    |         |------------|          |------------|         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |     A      |          |     G      |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           \.   |    |         |------------|          |------------|         
	        /-.     |    |                                                        
	        \-.     \.  |                                                        
	this.bearing.y           |                                                        
	                      |____*________________________*____________>>   X        
	                                                                               
	                                                                               
	                           <-----------------------. : this.advance.x            
	                                                                               
	                                <-----------. : this.sizeTexture.x               
	                                                                               
	                           <--. : this.bearing.x                                 
	                       
	*/
	/**
	 * @not_in_doc
	 */
	class GlyphProperty {
		public:
			Character this.UVal; //!< Unicode value
		public:
			boolean this.exist;
		public:
			int this.glyphIndex; //!< Glyph index in the system
			Vector2i this.sizeTexture; //!< size of the element to display
			Vector2i this.bearing; //!< offset to display the data (can be negatif id the texture sise is bigger than the theoric places in the string)
			Vector2i this.advance; //!< space use in the display for this specific char
			Vector2f this.texturePosStart; //!< Texture normalized position (START)
			Vector2f this.texturePosSize; //!< Texture normalized position (SIZE)
		private:
			List<ewol::Kerning> this.kerning; //!< kerning values of link of all elements
		public:
			GlyphProperty() :
			  this.UVal(0),
			  this.exist(true),
			  this.glyphIndex(0),
			  this.sizeTexture(10,10),
			  this.bearing(2,2),
			  this.advance(10,10),
			  this.texturePosStart(0,0),
			  this.texturePosSize(0,0) {
				
			};
			float kerningGet( Character _charcode) {
				for(int iii=0; iii<this.kerning.size(); iii++ ) {
					if (this.kerning[iii].this.UVal == _charcode) {
						return this.kerning[iii].this.value;
					}
				}
				return 0;
			};
			void kerningAdd( Character _charcode, float _value) {
				this.kerning.pushBack(ewol::Kerning(_charcode, _value));
			};
			void kerningClear() {
				this.kerning.clear();
			};
			/**
			 * @brief get the status of the char, if it exist or not in the FONT
			 * @return true if the char is availlable, false otherwise
			 */
			boolean exist()  {
				return this.exist;
			};
			/**
			 * @brief set the element doen not exist !!!
			 */
			void setNotExist() {
				this.exist = false;
			};
	};
};

