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

#include <ewol/resource/Manager.h>

#include <ewol/resource/font/FontBase.h>
#include <ewol/resource/TexturedFont.h>
#include <ewol/resource/FontFreeType.h>
#include <ewol/context/Context.h>


std::ostream& ewol::operator <<(std::ostream& _os, enum ewol::font::mode _obj) {
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
#define __class__ "resource::TexturedFont"

ewol::resource::TexturedFont::TexturedFont(const std::string& _fontName) :
  ewol::resource::Texture(_fontName) {
	addObjectType("ewol::resource::TexturedFont");
	EWOL_DEBUG("Load font : '" << _fontName << "'" );

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
	const char * tmpData = _fontName.c_str();
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
	std::string localName(_fontName, 0, (tmpPos - tmpData));
	if (tmpSize>400) {
		EWOL_ERROR("Font size too big ==> limit at 400 when exxeed ==> error : " << tmpSize << "==>30");
		tmpSize = 30;
	}
	m_size = tmpSize;
	
	std::vector<std::string> folderList;
	if (true == ewol::getContext().getFontDefault().getUseExternal()) {
		#if defined(__TARGET_OS__Android)
			folderList.push_back("/system/fonts");
		#elif defined(__TARGET_OS__Linux)
			folderList.push_back("/usr/share/fonts/truetype");
		#endif
	}
	folderList.push_back(ewol::getContext().getFontDefault().getFolder());
	for (size_t folderID=0; folderID<folderList.size() ; folderID++) {
		etk::FSNode myFolder(folderList[folderID]);
		// find the real Font name :
		std::vector<std::string> output;
		myFolder.folderGetRecursiveFiles(output);
		std::vector<std::string> split = std::split(localName, ';');
		EWOL_INFO("try to find font named : " << split << " in: " << myFolder);
		//EWOL_CRITICAL("parse string : " << split);
		bool hasFindAFont = false;
		for (size_t jjj=0; jjj<split.size(); jjj++) {
			EWOL_INFO("    try with : '" << split[jjj] << "'");
			for (size_t iii=0; iii<output.size(); iii++) {
				//EWOL_DEBUG(" file : " << output[iii]);
				if(    true == end_with(output[iii], split[jjj]+"-"+"bold"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"-"+"b"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"-"+"bd"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"bold"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"bd"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"b"+".ttf", false)) {
					EWOL_INFO(" find Font [Bold]        : " << output[iii]);
					m_fileName[ewol::font::Bold] = output[iii];
					hasFindAFont=true;
				} else if(    true == end_with(output[iii], split[jjj]+"-"+"oblique"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"italic"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"Light"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"i"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"oblique"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"italic"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"light"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"i"+".ttf", false)) {
					EWOL_INFO(" find Font [Italic]      : " << output[iii]);
					m_fileName[ewol::font::Italic] = output[iii];
					hasFindAFont=true;
				} else if(    true == end_with(output[iii], split[jjj]+"-"+"bolditalic"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"boldoblique"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"bi"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"z"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"bolditalic"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"boldoblique"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"bi"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"z"+".ttf", false)) {
					EWOL_INFO(" find Font [Bold-Italic] : " << output[iii]);
					m_fileName[ewol::font::BoldItalic] = output[iii];
					hasFindAFont=true;
				} else if(    true == end_with(output[iii], split[jjj]+"-"+"regular"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"-"+"r"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"regular"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+"r"+".ttf", false)
				           || true == end_with(output[iii], split[jjj]+".ttf", false)) {
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
	enum ewol::font::mode refMode = ewol::font::Regular;
	for(int32_t iii=3; iii >= 0; iii--) {
		if (m_fileName[iii].size() != 0) {
			refMode = (enum ewol::font::mode)iii;
		}
	}
	
	EWOL_DEBUG("         set reference mode : " << refMode);
	// generate the wrapping on the preventing error
	for(int32_t iii=3; iii >= 0; iii--) {
		if (m_fileName[iii].size() != 0) {
			m_modeWraping[iii] = (enum ewol::font::mode)iii;
		} else {
			m_modeWraping[iii] = refMode;
		}
	}
	
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (m_fileName[iiiFontId].size() == 0) {
			EWOL_DEBUG("can not load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size );
			m_font[iiiFontId] = NULL;
			continue;
		}
		EWOL_INFO("Load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\"  == > size=" << m_size);
		m_font[iiiFontId] = ewol::resource::FontFreeType::keep(m_fileName[iiiFontId]);
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

ewol::resource::TexturedFont::~TexturedFont(void) {
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		ewol::resource::FontFreeType::release(m_font[iiiFontId]);
	}
}

bool ewol::resource::TexturedFont::addGlyph(const char32_t& _val) {
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
		m_listElement[iii].push_back(tmpchar);
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

int32_t ewol::resource::TexturedFont::getIndex(char32_t _charcode, const enum ewol::font::mode _displayMode) {
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
	//EWOL_DEBUG("Get glyph property for mode: " << _displayMode << "  == > wrapping index : " << m_modeWraping[_displayMode]);
	int32_t index = getIndex(_charcode, _displayMode);
	if(    index < 0
	    || (size_t)index >= m_listElement[_displayMode].size() ) {
		EWOL_ERROR(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" << index);
		if (m_listElement[_displayMode].size() > 0) {
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

ewol::resource::TexturedFont* ewol::resource::TexturedFont::keep(const std::string& _filename) {
	EWOL_VERBOSE("KEEP : TexturedFont : file : '" << _filename << "'");
	ewol::resource::TexturedFont* object = NULL;
	ewol::Resource* object2 = getManager().localKeep(_filename);
	if (NULL != object2) {
		object = dynamic_cast<ewol::resource::TexturedFont*>(object2);
		if (NULL == object) {
			EWOL_CRITICAL("Request resource file : '" << _filename << "' With the wrong type (dynamic cast error)");
			return NULL;
		}
	}
	if (NULL != object) {
		return object;
	}
	// need to crate a new one ...
	EWOL_INFO("CREATE: TexturedFont : file : '" << _filename << "'");
	object = new ewol::resource::TexturedFont(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::resource::TexturedFont::release(ewol::resource::TexturedFont*& _object) {
	if (NULL == _object) {
		return;
	}
	EWOL_VERBOSE("RELEASE: TexturedFont : file : '" << _object->getName() << "' count=" << _object->getCounter());
	std::string name = _object->getName();
	int32_t count = _object->getCounter() - 1;
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	if (getManager().release(object2) == true) {
		EWOL_ERROR("REMOVE: TexturedFont : file : '" << name << "' count=" << count);
		//etk::displayBacktrace(false);
	}
	_object = NULL;
}
