/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <egami/egami.hpp>

#include <gale/resource/Manager.hpp>

#include <ewol/resource/font/FontBase.hpp>
#include <ewol/resource/TexturedFont.hpp>
#include <ewol/resource/FontFreeType.hpp>
#include <ewol/context/Context.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::font::mode);
ETK_DECLARE_TYPE(ewol::resource::TexturedFont);

etk::Stream ewol::operator +(etk::Stream _os, enum ewol::font::mode _obj) {
	switch(_obj) {
		default :
			_os + "error";
			break;
		case ewol::font::Regular:
			_os + "Regular";
			break;
		case ewol::font::Italic:
			_os + "Italic";
			break;
		case ewol::font::Bold:
			_os + "Bold";
			break;
		case ewol::font::BoldItalic:
			_os + "BoldItalic";
			break;
	}
	return _os;
}

ewol::resource::TexturedFont::TexturedFont():
  this.size(10) {
	addResourceType("ewol::resource::TexturedFont");
}

/**
 * @brief Get all the Path contain in the specidy path:
 * @param[in] _path Generic path to parse ...
 * @return The list of path found
 * @example[start]
 *     auto out = explodeMultiplePath("DATA:///font?lib=ewol");
 *     // out contain: {"DATA:///font", "DATA:///font?lib=ewol"}
 * @example[stop]
 */
static List<etk::Uri> explodeMultiplePath( etk::Uri _uri) {
	List<etk::Uri> out;
	out.pushBack(_uri);
	if (_uri.getQuery().exist("lib") == true) {
		etk::Uri tmp = _uri;
		tmp.getQuery().erase("lib");
		out.pushBack(tmp);
	}
	return out;
}

void ewol::resource::TexturedFont::init( String _fontName) {
	ethread::RecursiveLock lock(this.mutex);
	ewol::resource::Texture::init(_fontName);
	Log.debug("Load font : '" + _fontName + "'" );

	this.font[0] = null;
	this.font[1] = null;
	this.font[2] = null;
	this.font[3] = null;
	
	this.modeWraping[0] = ewol::font::Regular;
	this.modeWraping[1] = ewol::font::Regular;
	this.modeWraping[2] = ewol::font::Regular;
	this.modeWraping[3] = ewol::font::Regular;
	
	this.lastGlyphPos[0].setValue(1,1);
	this.lastGlyphPos[1].setValue(1,1);
	this.lastGlyphPos[2].setValue(1,1);
	this.lastGlyphPos[3].setValue(1,1);
	
	this.lastRawHeigh[0] = 0;
	this.lastRawHeigh[1] = 0;
	this.lastRawHeigh[2] = 0;
	this.lastRawHeigh[3] = 0;
	
	int tmpSize = 0;
	// extarct name and size :
	 char * tmpData = _fontName.c_str();
	 char * tmpPos = strchr(tmpData, ':');
	
	if (tmpPos == null) {
		this.size = 1;
		Log.critical("Can not parse the font name: '" + _fontName + "' ??? ':' " );
		return;
	} else {
		if (sscanf(tmpPos+1, "%d", tmpSize)!=1) {
			this.size = 1;
			Log.critical("Can not parse the font name: '" + _fontName + "'  == > size ???");
			return;
		}
	}
	String localName(_fontName, 0, (tmpPos - tmpData));
	if (tmpSize>400) {
		Log.error("Font size too big ==> limit at 400 when exxeed ==> error: " + tmpSize + "==>30");
		tmpSize = 30;
	}
	this.size = tmpSize;
	
	List<etk::Uri> folderList;
	if (ewol::getContext().getFontDefault().getUseExternal() == true) {
		#if defined(__TARGET_OS__Android)
			folderList.pushBack(etk::Path("/system/fonts"));
		#elif defined(__TARGET_OS__Linux)
			folderList.pushBack(etk::Path("/usr/share/fonts"));
		#endif
	}
	etk::Uri applicationBaseFont = ewol::getContext().getFontDefault().getFolder();
	for (auto it : explodeMultiplePath(applicationBaseFont)) {
		folderList.pushBack(it);
	}
	for (int folderID = 0; folderID < folderList.size() ; folderID++) {
		List<etk::Uri> output = etk::uri::listRecursive(folderList[folderID]);
		
		List<String> split = etk::split(localName, ';');
		Log.debug("try to find font named : " + split + " in: " + output);
		//Log.critical("parse string : " + split);
		boolean hasFindAFont = false;
		for (int jjj=0; jjj<split.size(); jjj++) {
			Log.debug("    try with : '" + split[jjj] + "'");
			for (int iii=0; iii<output.size(); iii++) {
				String nameFolder = output[iii].getPath().getString();
				//Log.debug(" file : " + output[iii]);
				if(    etk::end_with(nameFolder, split[jjj]+"-"+"bold"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"-"+"b"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"-"+"bd"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"bold"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"bd"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"b"+".ttf", false) == true) {
					Log.debug(" find Font [Bold]        : " + output[iii]);
					this.fileName[ewol::font::Bold] = output[iii];
					hasFindAFont = true;
				} else if(    etk::end_with(nameFolder, split[jjj]+"-"+"oblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"italic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"Light"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"i"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"oblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"italic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"light"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"i"+".ttf", false) == true) {
					Log.debug(" find Font [Italic]      : " + output[iii]);
					this.fileName[ewol::font::Italic] = output[iii];
					hasFindAFont = true;
				} else if(    etk::end_with(nameFolder, split[jjj]+"-"+"bolditalic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"boldoblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"bi"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"z"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"bolditalic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"boldoblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"bi"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"z"+".ttf", false) == true) {
					Log.debug(" find Font [Bold-Italic] : " + output[iii]);
					this.fileName[ewol::font::BoldItalic] = output[iii];
					hasFindAFont = true;
				} else if(    etk::end_with(nameFolder, split[jjj]+"-"+"regular"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"r"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"regular"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"r"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+".ttf", false) == true) {
					Log.debug(" find Font [Regular]     : " + output[iii]);
					this.fileName[ewol::font::Regular] = output[iii];
					hasFindAFont = true;
				}
			}
			if (hasFindAFont == true) {
				Log.debug("    find this font : '" + split[jjj] + "'");
				break;
			} else if (jjj == split.size()-1) {
				Log.debug("Find NO font in the LIST ... " + split);
			}
		}
		if (hasFindAFont == true) {
			Log.debug("    find this font : '" + folderList[folderID] + "'");
			break;
		} else if (folderID == folderList.size()-1) {
			Log.error("Find NO font in the LIST ... " + folderList);
		}
	}
	// try to find the reference mode :
	enum ewol::font::mode refMode = ewol::font::Regular;
	for(int iii=3; iii >= 0; iii--) {
		if (this.fileName[iii].isEmpty() == false) {
			refMode = (enum ewol::font::mode)iii;
		}
	}
	Log.debug("         set reference mode : " + refMode);
	// generate the wrapping on the preventing error
	for(int iii=3; iii >= 0; iii--) {
		if (this.fileName[iii].isEmpty() == false) {
			this.modeWraping[iii] = (enum ewol::font::mode)iii;
		} else {
			this.modeWraping[iii] = refMode;
		}
	}
	
	for (int iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (this.fileName[iiiFontId].isEmpty() == true) {
			Log.debug("can not load FONT [" + iiiFontId + "] name : \"" + this.fileName[iiiFontId] + "\"  == > size=" + this.size );
			this.font[iiiFontId] = null;
			continue;
		}
		Log.debug("Load FONT [" + iiiFontId + "] name : \"" + this.fileName[iiiFontId] + "\"  == > size=" + this.size);
		this.font[iiiFontId] = ewol::resource::FontFreeType::create(this.fileName[iiiFontId]);
		if (this.font[iiiFontId] == null) {
			Log.debug("error in loading FONT [" + iiiFontId + "] name : \"" + this.fileName[iiiFontId] + "\"  == > size=" + this.size );
		}
	}
	for (int iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		// set the bassic charset:
		this.listElement[iiiFontId].clear();
		if (this.font[iiiFontId] == null) {
			continue;
		}
		this.height[iiiFontId] = this.font[iiiFontId].getHeight(this.size);
		// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
		setImageSize(Vector2i(256,32));
		// now we can acces directly on the image
		this.data.clear(etk::Color<>(0x00000000));
	}
	// add error glyph
	addGlyph(0);
	// by default we set only the first AINSI char availlable
	for (int iii=0x20; iii<0x7F; iii++) {
		Log.verbose("Add clyph :" + iii);
		addGlyph(iii);
	}
	flush();
	Log.debug("Wrapping properties : ");
	Log.debug("    " + ewol::font::Regular + " == >" + getWrappingMode(ewol::font::Regular));
	Log.debug("    " + ewol::font::Italic + " == >" + getWrappingMode(ewol::font::Italic));
	Log.debug("    " + ewol::font::Bold + " == >" + getWrappingMode(ewol::font::Bold));
	Log.debug("    " + ewol::font::BoldItalic + " == >" + getWrappingMode(ewol::font::BoldItalic));
}

ewol::resource::TexturedFont::~TexturedFont() {
	
}

boolean ewol::resource::TexturedFont::addGlyph( Character _val) {
	ethread::RecursiveLock lock(this.mutex);
	boolean hasChange = false;
	// for each font :
	for (int iii=0; iii<4 ; iii++) {
		if (this.font[iii] == null) {
			continue;
		}
		// add the curent "char"
		GlyphProperty tmpchar;
		tmpchar.this.UVal = _val;
		
		if (this.font[iii].getGlyphProperty(this.size, tmpchar) == true) {
			//Log.debug("load char : '" + _val + "'=" + _val.get());
			hasChange = true;
			// change line if needed ...
			if (this.lastGlyphPos[iii].x()+tmpchar.this.sizeTexture.x()+3 > this.data.getSize().x()) {
				this.lastGlyphPos[iii].setX(1);
				this.lastGlyphPos[iii] += Vector2i(0, this.lastRawHeigh[iii]);
				this.lastRawHeigh[iii] = 0;
			}
			while(this.lastGlyphPos[iii].y()+tmpchar.this.sizeTexture.y()+3 > this.data.getSize().y()) {
				Vector2i size = this.data.getSize();
				size.setY(size.y()*2);
				this.data.resize(size, etk::Color<>(0));
				// note : need to rework all the lyer due to the fact that the texture is used by the faur type...
				for (int kkk=0; kkk<4 ; kkk++) {
					// change the coordonate on the element in the texture
					for (int jjj=0 ; jjj<this.listElement[kkk].size() ; ++jjj) {
						this.listElement[kkk][jjj].this.texturePosStart *= Vector2f(1.0f, 0.5f);
						this.listElement[kkk][jjj].this.texturePosSize *= Vector2f(1.0f, 0.5f);
					}
				}
			}
			// draw the glyph
			this.font[iii].drawGlyph(this.data, this.size, this.lastGlyphPos[iii], tmpchar, iii);
			// set video position
			tmpchar.this.texturePosStart.setValue( (float)this.lastGlyphPos[iii].x() / (float)this.data.getSize().x(),
			                                    (float)this.lastGlyphPos[iii].y() / (float)this.data.getSize().y() );
			tmpchar.this.texturePosSize.setValue(  (float)tmpchar.this.sizeTexture.x() / (float)this.data.getSize().x(),
			                                    (float)tmpchar.this.sizeTexture.y() / (float)this.data.getSize().y() );
			
			// update the maximum of the line hight : 
			if (this.lastRawHeigh[iii]<tmpchar.this.sizeTexture.y()) {
				// note : +1 is for the overlapping of the glyph (Part 2)
				this.lastRawHeigh[iii] = tmpchar.this.sizeTexture.y()+1;
			}
			// note : +1 is for the overlapping of the glyph (Part 3)
			// update the Bitmap position drawing : 
			this.lastGlyphPos[iii] += Vector2i(tmpchar.this.sizeTexture.x()+1, 0);
		} else {
			Log.warning("Did not find char : '" + _val + "'=" + _val);
			tmpchar.setNotExist();
		}
		this.listElement[iii].pushBack(tmpchar);
		//this.font[iii].display();
		// generate the kerning for all the characters :
		if (tmpchar.exist() == true) {
			// TODO : set the kerning back ...
			//this.font[iii].generateKerning(this.size, this.listElement[iii]);
		}
	}
	if (hasChange == true) {
		flush();
		ewol::getContext().forceRedrawAll();
		//egami::store(this.data, "fileFont.bmp"); // ==> for debug test only ...
	}
	return hasChange;
}

int ewol::resource::TexturedFont::getIndex(Character _charcode,  enum ewol::font::mode _displayMode) {
	ethread::RecursiveLock lock(this.mutex);
	if (_charcode < 0x20) {
		return 0;
	} else if (_charcode < 0x80) {
		return _charcode - 0x1F;
	} else {
		for (int iii=0x80-0x20; iii < this.listElement[_displayMode].size(); iii++) {
			//Log.debug("search : '" + charcode + "' =?= '" + (this.listElement[displayMode])[iii].this.UVal + "'");
			if (_charcode == (this.listElement[_displayMode])[iii].this.UVal) {
				//Log.debug("search : '" + charcode + "'");
				if ((this.listElement[_displayMode])[iii].exist()) {
					//Log.debug("return " + iii);
					return iii;
				} else {
					return 0;
				}
			}
		}
	}
	if (addGlyph(_charcode) == true) {
		// TODO : This does not work due to the fact that the update of open GL is not done in the context main cycle !!!
		ewol::getContext().forceRedrawAll();
	}
	return 0;
}

ewol::GlyphProperty* ewol::resource::TexturedFont::getGlyphPointer( Character _charcode,  enum ewol::font::mode _displayMode) {
	ethread::RecursiveLock lock(this.mutex);
	//Log.debug("Get glyph property for mode: " + _displayMode + "  == > wrapping index : " + this.modeWraping[_displayMode]);
	int index = getIndex(_charcode, _displayMode);
	if(    index < 0
	    || (int)index >= this.listElement[_displayMode].size() ) {
		Log.error(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" + index);
		if (this.listElement[_displayMode].size() > 0) {
			return ((this.listElement[_displayMode])[0]);
		}
		return this.emptyGlyph;
	}
	//Log.error("      index=" + index);
	//Log.error("      this.UVal=" + this.listElement[_displayMode][index].this.UVal);
	//Log.error("      this.glyphIndex=" + this.listElement[_displayMode][index].this.glyphIndex);
	//Log.error("      this.advance=" + this.listElement[_displayMode][index].this.advance);
	//Log.error("      this.bearing=" + this.listElement[_displayMode][index].this.bearing);
	return ((this.listElement[_displayMode])[index]);
}

