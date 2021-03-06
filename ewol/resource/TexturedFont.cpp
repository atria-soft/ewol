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

etk::Stream& ewol::operator <<(etk::Stream& _os, enum ewol::font::mode _obj) {
	switch(_obj) {
		default :
			_os << "error";
			break;
		case ewol::font::Regular:
			_os << "Regular";
			break;
		case ewol::font::Italic:
			_os << "Italic";
			break;
		case ewol::font::Bold:
			_os << "Bold";
			break;
		case ewol::font::BoldItalic:
			_os << "BoldItalic";
			break;
	}
	return _os;
}

ewol::resource::TexturedFont::TexturedFont():
  m_size(10) {
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
static etk::Vector<etk::Uri> explodeMultiplePath(const etk::Uri& _uri) {
	etk::Vector<etk::Uri> out;
	out.pushBack(_uri);
	if (_uri.getQuery().exist("lib") == true) {
		etk::Uri tmp = _uri;
		tmp.getQuery().erase("lib");
		out.pushBack(tmp);
	}
	return out;
}

void ewol::resource::TexturedFont::init(const etk::String& _fontName) {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init(_fontName);
	EWOL_DEBUG("Load font : '" << _fontName << "'" );

	m_font[0] = null;
	m_font[1] = null;
	m_font[2] = null;
	m_font[3] = null;
	
	m_modeWraping[0] = ewol::font::Regular;
	m_modeWraping[1] = ewol::font::Regular;
	m_modeWraping[2] = ewol::font::Regular;
	m_modeWraping[3] = ewol::font::Regular;
	
	m_lastGlyphPos[0].setValue(1,1);
	m_lastGlyphPos[1].setValue(1,1);
	m_lastGlyphPos[2].setValue(1,1);
	m_lastGlyphPos[3].setValue(1,1);
	
	m_lastRawHeigh[0] = 0;
	m_lastRawHeigh[1] = 0;
	m_lastRawHeigh[2] = 0;
	m_lastRawHeigh[3] = 0;
	
	int32_t tmpSize = 0;
	// extarct name and size :
	const char * tmpData = _fontName.c_str();
	const char * tmpPos = strchr(tmpData, ':');
	
	if (tmpPos == null) {
		m_size = 1;
		EWOL_CRITICAL("Can not parse the font name: '" << _fontName << "' ??? ':' " );
		return;
	} else {
		if (sscanf(tmpPos+1, "%d", &tmpSize)!=1) {
			m_size = 1;
			EWOL_CRITICAL("Can not parse the font name: '" << _fontName << "'  == > size ???");
			return;
		}
	}
	etk::String localName(_fontName, 0, (tmpPos - tmpData));
	if (tmpSize>400) {
		EWOL_ERROR("Font size too big ==> limit at 400 when exxeed ==> error: " << tmpSize << "==>30");
		tmpSize = 30;
	}
	m_size = tmpSize;
	
	etk::Vector<etk::Uri> folderList;
	if (ewol::getContext().getFontDefault().getUseExternal() == true) {
		#if defined(__TARGET_OS__Android)
			folderList.pushBack(etk::Path("/system/fonts"));
		#elif defined(__TARGET_OS__Linux)
			folderList.pushBack(etk::Path("/usr/share/fonts"));
		#endif
	}
	etk::Uri applicationBaseFont = ewol::getContext().getFontDefault().getFolder();
	for (auto &it : explodeMultiplePath(applicationBaseFont)) {
		folderList.pushBack(it);
	}
	for (size_t folderID = 0; folderID < folderList.size() ; folderID++) {
		etk::Vector<etk::Uri> output = etk::uri::listRecursive(folderList[folderID]);
		
		etk::Vector<etk::String> split = etk::split(localName, ';');
		EWOL_DEBUG("try to find font named : " << split << " in: " << output);
		//EWOL_CRITICAL("parse string : " << split);
		bool hasFindAFont = false;
		for (size_t jjj=0; jjj<split.size(); jjj++) {
			EWOL_DEBUG("    try with : '" << split[jjj] << "'");
			for (size_t iii=0; iii<output.size(); iii++) {
				etk::String nameFolder = output[iii].getPath().getString();
				//EWOL_DEBUG(" file : " << output[iii]);
				if(    etk::end_with(nameFolder, split[jjj]+"-"+"bold"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"-"+"b"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"-"+"bd"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"bold"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"bd"+".ttf", false) == true
				    || etk::end_with(nameFolder, split[jjj]+"b"+".ttf", false) == true) {
					EWOL_DEBUG(" find Font [Bold]        : " << output[iii]);
					m_fileName[ewol::font::Bold] = output[iii];
					hasFindAFont = true;
				} else if(    etk::end_with(nameFolder, split[jjj]+"-"+"oblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"italic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"Light"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"i"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"oblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"italic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"light"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"i"+".ttf", false) == true) {
					EWOL_DEBUG(" find Font [Italic]      : " << output[iii]);
					m_fileName[ewol::font::Italic] = output[iii];
					hasFindAFont = true;
				} else if(    etk::end_with(nameFolder, split[jjj]+"-"+"bolditalic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"boldoblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"bi"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"z"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"bolditalic"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"boldoblique"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"bi"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"z"+".ttf", false) == true) {
					EWOL_DEBUG(" find Font [Bold-Italic] : " << output[iii]);
					m_fileName[ewol::font::BoldItalic] = output[iii];
					hasFindAFont = true;
				} else if(    etk::end_with(nameFolder, split[jjj]+"-"+"regular"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"-"+"r"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"regular"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+"r"+".ttf", false) == true
				           || etk::end_with(nameFolder, split[jjj]+".ttf", false) == true) {
					EWOL_DEBUG(" find Font [Regular]     : " << output[iii]);
					m_fileName[ewol::font::Regular] = output[iii];
					hasFindAFont = true;
				}
			}
			if (hasFindAFont == true) {
				EWOL_DEBUG("    find this font : '" << split[jjj] << "'");
				break;
			} else if (jjj == split.size()-1) {
				EWOL_DEBUG("Find NO font in the LIST ... " << split);
			}
		}
		if (hasFindAFont == true) {
			EWOL_DEBUG("    find this font : '" << folderList[folderID] << "'");
			break;
		} else if (folderID == folderList.size()-1) {
			EWOL_ERROR("Find NO font in the LIST ... " << folderList);
		}
	}
	// try to find the reference mode :
	enum ewol::font::mode refMode = ewol::font::Regular;
	for(int32_t iii=3; iii >= 0; iii--) {
		if (m_fileName[iii].isEmpty() == false) {
			refMode = (enum ewol::font::mode)iii;
		}
	}
	EWOL_DEBUG("         set reference mode : " << refMode);
	// generate the wrapping on the preventing error
	for(int32_t iii=3; iii >= 0; iii--) {
		if (m_fileName[iii].isEmpty() == false) {
			m_modeWraping[iii] = (enum ewol::font::mode)iii;
		} else {
			m_modeWraping[iii] = refMode;
		}
	}
	
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (m_fileName[iiiFontId].isEmpty() == true) {
			EWOL_DEBUG("can not load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size );
			m_font[iiiFontId] = null;
			continue;
		}
		EWOL_DEBUG("Load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size);
		m_font[iiiFontId] = ewol::resource::FontFreeType::create(m_fileName[iiiFontId]);
		if (m_font[iiiFontId] == null) {
			EWOL_DEBUG("error in loading FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size );
		}
	}
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		// set the bassic charset:
		m_listElement[iiiFontId].clear();
		if (m_font[iiiFontId] == null) {
			continue;
		}
		m_height[iiiFontId] = m_font[iiiFontId]->getHeight(m_size);
		// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
		setImageSize(ivec2(256,32));
		// now we can acces directly on the image
		m_data.clear(etk::Color<>(0x00000000));
	}
	// add error glyph
	addGlyph(0);
	// by default we set only the first AINSI char availlable
	for (int32_t iii=0x20; iii<0x7F; iii++) {
		EWOL_VERBOSE("Add clyph :" << iii);
		addGlyph(iii);
	}
	flush();
	EWOL_DEBUG("Wrapping properties : ");
	EWOL_DEBUG("    " << ewol::font::Regular << " == >" << getWrappingMode(ewol::font::Regular));
	EWOL_DEBUG("    " << ewol::font::Italic << " == >" << getWrappingMode(ewol::font::Italic));
	EWOL_DEBUG("    " << ewol::font::Bold << " == >" << getWrappingMode(ewol::font::Bold));
	EWOL_DEBUG("    " << ewol::font::BoldItalic << " == >" << getWrappingMode(ewol::font::BoldItalic));
}

ewol::resource::TexturedFont::~TexturedFont() {
	
}

bool ewol::resource::TexturedFont::addGlyph(const char32_t& _val) {
	ethread::RecursiveLock lock(m_mutex);
	bool hasChange = false;
	// for each font :
	for (int32_t iii=0; iii<4 ; iii++) {
		if (m_font[iii] == null) {
			continue;
		}
		// add the curent "char"
		GlyphProperty tmpchar;
		tmpchar.m_UVal = _val;
		
		if (m_font[iii]->getGlyphProperty(m_size, tmpchar) == true) {
			//EWOL_DEBUG("load char : '" << _val << "'=" << _val.get());
			hasChange = true;
			// change line if needed ...
			if (m_lastGlyphPos[iii].x()+tmpchar.m_sizeTexture.x()+3 > m_data.getSize().x()) {
				m_lastGlyphPos[iii].setX(1);
				m_lastGlyphPos[iii] += ivec2(0, m_lastRawHeigh[iii]);
				m_lastRawHeigh[iii] = 0;
			}
			while(m_lastGlyphPos[iii].y()+tmpchar.m_sizeTexture.y()+3 > m_data.getSize().y()) {
				ivec2 size = m_data.getSize();
				size.setY(size.y()*2);
				m_data.resize(size, etk::Color<>(0));
				// note : need to rework all the lyer due to the fact that the texture is used by the faur type...
				for (size_t kkk=0; kkk<4 ; kkk++) {
					// change the coordonate on the element in the texture
					for (size_t jjj=0 ; jjj<m_listElement[kkk].size() ; ++jjj) {
						m_listElement[kkk][jjj].m_texturePosStart *= vec2(1.0f, 0.5f);
						m_listElement[kkk][jjj].m_texturePosSize *= vec2(1.0f, 0.5f);
					}
				}
			}
			// draw the glyph
			m_font[iii]->drawGlyph(m_data, m_size, m_lastGlyphPos[iii], tmpchar, iii);
			// set video position
			tmpchar.m_texturePosStart.setValue( (float)m_lastGlyphPos[iii].x() / (float)m_data.getSize().x(),
			                                    (float)m_lastGlyphPos[iii].y() / (float)m_data.getSize().y() );
			tmpchar.m_texturePosSize.setValue(  (float)tmpchar.m_sizeTexture.x() / (float)m_data.getSize().x(),
			                                    (float)tmpchar.m_sizeTexture.y() / (float)m_data.getSize().y() );
			
			// update the maximum of the line hight : 
			if (m_lastRawHeigh[iii]<tmpchar.m_sizeTexture.y()) {
				// note : +1 is for the overlapping of the glyph (Part 2)
				m_lastRawHeigh[iii] = tmpchar.m_sizeTexture.y()+1;
			}
			// note : +1 is for the overlapping of the glyph (Part 3)
			// update the Bitmap position drawing : 
			m_lastGlyphPos[iii] += ivec2(tmpchar.m_sizeTexture.x()+1, 0);
		} else {
			EWOL_WARNING("Did not find char : '" << _val << "'=" << _val);
			tmpchar.setNotExist();
		}
		m_listElement[iii].pushBack(tmpchar);
		//m_font[iii]->display();
		// generate the kerning for all the characters :
		if (tmpchar.exist() == true) {
			// TODO : set the kerning back ...
			//m_font[iii]->generateKerning(m_size, m_listElement[iii]);
		}
	}
	if (hasChange == true) {
		flush();
		ewol::getContext().forceRedrawAll();
		//egami::store(m_data, "fileFont.bmp"); // ==> for debug test only ...
	}
	return hasChange;
}

int32_t ewol::resource::TexturedFont::getIndex(char32_t _charcode, const enum ewol::font::mode _displayMode) {
	ethread::RecursiveLock lock(m_mutex);
	if (_charcode < 0x20) {
		return 0;
	} else if (_charcode < 0x80) {
		return _charcode - 0x1F;
	} else {
		for (size_t iii=0x80-0x20; iii < m_listElement[_displayMode].size(); iii++) {
			//EWOL_DEBUG("search : '" << charcode << "' =?= '" << (m_listElement[displayMode])[iii].m_UVal << "'");
			if (_charcode == (m_listElement[_displayMode])[iii].m_UVal) {
				//EWOL_DEBUG("search : '" << charcode << "'");
				if ((m_listElement[_displayMode])[iii].exist()) {
					//EWOL_DEBUG("return " << iii);
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

ewol::GlyphProperty* ewol::resource::TexturedFont::getGlyphPointer(const char32_t& _charcode, const enum ewol::font::mode _displayMode) {
	ethread::RecursiveLock lock(m_mutex);
	//EWOL_DEBUG("Get glyph property for mode: " << _displayMode << "  == > wrapping index : " << m_modeWraping[_displayMode]);
	int32_t index = getIndex(_charcode, _displayMode);
	if(    index < 0
	    || (size_t)index >= m_listElement[_displayMode].size() ) {
		EWOL_ERROR(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" << index);
		if (m_listElement[_displayMode].size() > 0) {
			return &((m_listElement[_displayMode])[0]);
		}
		return &m_emptyGlyph;
	}
	//EWOL_ERROR("      index=" << index);
	//EWOL_ERROR("      m_UVal=" << m_listElement[_displayMode][index].m_UVal);
	//EWOL_ERROR("      m_glyphIndex=" << m_listElement[_displayMode][index].m_glyphIndex);
	//EWOL_ERROR("      m_advance=" << m_listElement[_displayMode][index].m_advance);
	//EWOL_ERROR("      m_bearing=" << m_listElement[_displayMode][index].m_bearing);
	return &((m_listElement[_displayMode])[index]);
}

