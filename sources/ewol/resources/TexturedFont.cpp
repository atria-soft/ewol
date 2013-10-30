/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/os/FSNode.h>
#include <egami/egami.h>

#include <ewol/resources/ResourceManager.h>
#include <ewol/renderer/eContext.h>

#include <ewol/resources/font/FontBase.h>
#include <ewol/resources/TexturedFont.h>
#include <ewol/resources/FontFreeType.h>


etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::font::mode_te& _obj) {
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

#undef __class__
#define __class__ "TexturedFont"

ewol::TexturedFont::TexturedFont(const etk::UString& _fontName) :
  ewol::Texture(_fontName) {
	m_font[0] = NULL;
	m_font[1] = NULL;
	m_font[2] = NULL;
	m_font[3] = NULL;
	
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
	etk::Char tmpChar = _fontName.c_str();
	const char * tmpData = tmpChar;
	const char * tmpPos = strchr(tmpData, ':');
	
	if (tmpPos == NULL) {
		m_size = 1;
		EWOL_CRITICAL("Can not parse the font name : \"" << _fontName << "\" ??? ':' " );
		return;
	} else {
		if (sscanf(tmpPos+1, "%d", &tmpSize)!=1) {
			m_size = 1;
			EWOL_CRITICAL("Can not parse the font name : \"" << _fontName << "\"  == > size ???");
			return;
		}
	}
	etk::UString localName = _fontName.extract(0, (tmpPos - tmpData));
	m_size = tmpSize;
	
	etk::Vector<etk::UString> folderList;
	if (true == ewol::getContext().getFontDefault().getUseExternal()) {
		#if defined(__TARGET_OS__Android)
			folderList.pushBack("/system/fonts");
		#elif defined(__TARGET_OS__Linux)
			folderList.pushBack("/usr/share/fonts/truetype");
		#endif
	}
	folderList.pushBack(ewol::getContext().getFontDefault().getFolder());
	for (int32_t folderID=0; folderID<folderList.size() ; folderID++) {
		etk::FSNode myFolder(folderList[folderID]);
		// find the real Font name :
		etk::Vector<etk::UString> output;
		myFolder.folderGetRecursiveFiles(output);
		etk::Vector<etk::UString> split = localName.split(';');
		EWOL_INFO("try to find font named : " << split << " in: " << myFolder);
		//EWOL_CRITICAL("parse string : " << split);
		bool hasFindAFont = false;
		for (int32_t jjj=0; jjj<split.size(); jjj++) {
			EWOL_INFO("    try with : '" << split[jjj] << "'");
			for (int32_t iii=0; iii<output.size(); iii++) {
				//EWOL_DEBUG(" file : " << output[iii]);
				if(    true == output[iii].endWith(split[jjj]+"-"+"bold"+".ttf", false)
				    || true == output[iii].endWith(split[jjj]+"-"+"b"+".ttf", false)
				    || true == output[iii].endWith(split[jjj]+"-"+"bd"+".ttf", false)
				    || true == output[iii].endWith(split[jjj]+"bold"+".ttf", false)
				    || true == output[iii].endWith(split[jjj]+"bd"+".ttf", false)
				    || true == output[iii].endWith(split[jjj]+"b"+".ttf", false)) {
					EWOL_INFO(" find Font [Bold]        : " << output[iii]);
					m_fileName[ewol::font::Bold] = output[iii];
					hasFindAFont=true;
				} else if(    true == output[iii].endWith(split[jjj]+"-"+"oblique"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"italic"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"Light"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"i"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"oblique"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"italic"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"light"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"i"+".ttf", false)) {
					EWOL_INFO(" find Font [Italic]      : " << output[iii]);
					m_fileName[ewol::font::Italic] = output[iii];
					hasFindAFont=true;
				} else if(    true == output[iii].endWith(split[jjj]+"-"+"bolditalic"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"boldoblique"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"bi"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"z"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"bolditalic"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"boldoblique"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"bi"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"z"+".ttf", false)) {
					EWOL_INFO(" find Font [Bold-Italic] : " << output[iii]);
					m_fileName[ewol::font::BoldItalic] = output[iii];
					hasFindAFont=true;
				} else if(    true == output[iii].endWith(split[jjj]+"-"+"regular"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"-"+"r"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"regular"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+"r"+".ttf", false)
				           || true == output[iii].endWith(split[jjj]+".ttf", false)) {
					EWOL_INFO(" find Font [Regular]     : " << output[iii]);
					m_fileName[ewol::font::Regular] = output[iii];
					hasFindAFont=true;
				}
			}
			if (hasFindAFont == true) {
				EWOL_INFO("    find this font : '" << split[jjj] << "'");
				break;
			} else if (jjj == split.size()-1) {
				EWOL_ERROR("Find NO font in the LIST ... " << split);
			}
		}
		if (hasFindAFont == true) {
			EWOL_INFO("    find this font : '" << folderList[folderID] << "'");
			break;
		} else if (folderID == folderList.size()-1) {
			EWOL_ERROR("Find NO font in the LIST ... " << folderList);
		}
	}
	// try to find the reference mode :
	ewol::font::mode_te refMode = ewol::font::Regular;
	for(int32_t iii=3; iii >= 0; iii--) {
		if (m_fileName[iii].isEmpty() == false) {
			refMode = (ewol::font::mode_te)iii;
		}
	}
	
	EWOL_DEBUG("         set reference mode : " << refMode);
	// generate the wrapping on the preventing error
	for(int32_t iii=3; iii >= 0; iii--) {
		if (m_fileName[iii].isEmpty() == false) {
			m_modeWraping[iii] = (ewol::font::mode_te)iii;
		} else {
			m_modeWraping[iii] = refMode;
		}
	}
	
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (m_fileName[iiiFontId].isEmpty() == true) {
			EWOL_DEBUG("can not load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size );
			m_font[iiiFontId] = NULL;
			continue;
		}
		EWOL_INFO("Load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size);
		m_font[iiiFontId] = ewol::FontFreeType::keep(m_fileName[iiiFontId]);
		if (m_font[iiiFontId] == NULL) {
			EWOL_DEBUG("error in loading FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size );
		}
	}
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		// set the bassic charset:
		m_listElement[iiiFontId].clear();
		if (m_font[iiiFontId] == NULL) {
			continue;
		}
		m_height[iiiFontId] = m_font[iiiFontId]->getHeight(m_size);
		// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
		setImageSize(ivec2(256,32));
		// now we can acces directly on the image
		m_data.clear(etk::Color<>(0x00000000));
	}
	// add error glyph
	{
		etk::UChar tmpchar;
		tmpchar.set(0);
		addGlyph(tmpchar);
	}
	// by default we set only the first AINSI char availlable
	for (int32_t iii=0x20; iii<0x7F; iii++) {
		etk::UChar tmpchar;
		tmpchar.set(iii);
		addGlyph(tmpchar);
	}
	flush();
	EWOL_DEBUG("Wrapping properties : ");
	EWOL_DEBUG("    " << ewol::font::Regular << " == >" << getWrappingMode(ewol::font::Regular));
	EWOL_DEBUG("    " << ewol::font::Italic << " == >" << getWrappingMode(ewol::font::Italic));
	EWOL_DEBUG("    " << ewol::font::Bold << " == >" << getWrappingMode(ewol::font::Bold));
	EWOL_DEBUG("    " << ewol::font::BoldItalic << " == >" << getWrappingMode(ewol::font::BoldItalic));
}

ewol::TexturedFont::~TexturedFont(void) {
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		ewol::FontFreeType::release(m_font[iiiFontId]);
	}
}

bool ewol::TexturedFont::addGlyph(const etk::UChar& _val) {
	bool hasChange = false;
	// for each font :
	for (int32_t iii=0; iii<4 ; iii++) {
		if (m_font[iii] == NULL) {
			continue;
		}
		// add the curent "char"
		GlyphProperty tmpchar;
		tmpchar.m_UVal = _val;
		
		if (true == m_font[iii]->getGlyphProperty(m_size, tmpchar)) {
			//EWOL_DEBUG("load char : '" << _val << "'=" << _val.get());
			hasChange = true;
			// change line if needed ...
			if (m_lastGlyphPos[iii].x()+tmpchar.m_sizeTexture.x() > m_data.getSize().x()) {
				m_lastGlyphPos[iii].setX(1);
				m_lastGlyphPos[iii] += ivec2(0, m_lastRawHeigh[iii]);
				m_lastRawHeigh[iii] = 0;
			}
			while(m_lastGlyphPos[iii].y()+tmpchar.m_sizeTexture.y() > m_data.getSize().y()) {
				ivec2 size = m_data.getSize();
				size.setY(size.y()*2);
				m_data.resize(size, etk::Color<>(0));
				// note : need to rework all the lyer due to the fact that the texture is used by the faur type...
				for (int32_t kkk=0; kkk<4 ; kkk++) {
					// change the coordonate on the element in the texture
					for (int32_t jjj=0 ; jjj<m_listElement[kkk].size() ; ++jjj) {
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
			EWOL_WARNING("Did not find char : '" << _val << "'=" << _val.get());
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
		//egami::store(m_data, "fileFont.bmp"); // ==> for debug test only ...
	}
	return hasChange;
}

int32_t ewol::TexturedFont::getIndex(const etk::UChar& _charcode, const ewol::font::mode_te _displayMode) {
	if (_charcode.get() < 0x20) {
		return 0;
	} else if (_charcode.get() < 0x80) {
		return _charcode.get() - 0x1F;
	} else {
		for (int32_t iii=0x80-0x20; iii < m_listElement[_displayMode].size(); iii++) {
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

ewol::GlyphProperty* ewol::TexturedFont::getGlyphPointer(const etk::UChar& _charcode, const ewol::font::mode_te _displayMode) {
	//EWOL_DEBUG("Get glyph property for mode: " << _displayMode << "  == > wrapping index : " << m_modeWraping[_displayMode]);
	int32_t index = getIndex(_charcode, _displayMode);
	if(    index < 0
	    || index >= m_listElement[_displayMode].size() ) {
		EWOL_ERROR(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" << index);
		if (m_listElement[_displayMode].size() >= 0) {
			return &((m_listElement[_displayMode])[0]);
		}
		return NULL;
	}
	//EWOL_ERROR("      index=" << index);
	//EWOL_ERROR("      m_UVal=" << m_listElement[_displayMode][index].m_UVal);
	//EWOL_ERROR("      m_glyphIndex=" << m_listElement[_displayMode][index].m_glyphIndex);
	//EWOL_ERROR("      m_advance=" << m_listElement[_displayMode][index].m_advance);
	//EWOL_ERROR("      m_bearing=" << m_listElement[_displayMode][index].m_bearing);
	return &((m_listElement[_displayMode])[index]);
}

ewol::TexturedFont* ewol::TexturedFont::keep(const etk::UString& _filename) {
	EWOL_VERBOSE("KEEP : TexturedFont : file : '" << _filename << "'");
	ewol::TexturedFont* object = static_cast<ewol::TexturedFont*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	// need to crate a new one ...
	EWOL_DEBUG("CREATE: TexturedFont : file : '" << _filename << "'");
	object = new ewol::TexturedFont(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::TexturedFont::release(ewol::TexturedFont*& _object) {
	if (NULL == _object) {
		return;
	}
	etk::UString name = _object->getName();
	int32_t count = _object->m_counter - 1;
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	if (getManager().release(object2) == true) {
		EWOL_DEBUG("REMOVE: TexturedFont : file : '" << name << "' count=" << count);
		//etk::displayBacktrace(false);
	}
	_object = NULL;
}
