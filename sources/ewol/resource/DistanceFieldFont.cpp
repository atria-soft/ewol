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
#include <ewol/resource/DistanceFieldFont.h>
#include <edtaa3/edtaa3func.h>

#undef __class__
#define __class__ "resource::DistanceFieldFont"

#define SIZE_GENERATION (30)

ewol::resource::DistanceFieldFont::DistanceFieldFont(const std::string& _fontName) :
  ewol::resource::Texture(_fontName),
  m_borderSize(10),
  m_textureBorderSize(0,0) {
	addObjectType("ewol::resource::DistanceFieldFont");
	m_font = NULL;
	m_lastGlyphPos.setValue(1,1);
	m_lastRawHeigh = 0;
	m_sizeRatio = 1.0f;
	std::string localName = _fontName;
	std::vector<std::string> folderList;
	if (true == ewol::getContext().getFontDefault().getUseExternal()) {
		#if defined(__TARGET_OS__Android)
			folderList.push_back("/system/fonts");
		#elif defined(__TARGET_OS__Linux)
			folderList.push_back("/usr/share/fonts/truetype");
		#endif
	}
	folderList.push_back(ewol::getContext().getFontDefault().getFolder());
	for (size_t folderID = 0; folderID < folderList.size() ; folderID++) {
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
				if(    true == end_with(output[iii], split[jjj]+"-"+"regular"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"-"+"r"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"regular"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+"r"+".ttf", false)
				    || true == end_with(output[iii], split[jjj]+".ttf", false)) {
					EWOL_INFO(" find Font [Regular]     : " << output[iii]);
					m_fileName = output[iii];
					hasFindAFont=true;
					break;
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
	
	if (m_fileName.size() == 0) {
		EWOL_ERROR("can not load FONT name : '" << m_fileName << "'" );
		m_font = NULL;
		return;
	}
	EWOL_INFO("Load FONT name : '" << m_fileName << "'");
	m_font = ewol::resource::FontFreeType::keep(m_fileName);
	if (m_font == NULL) {
		EWOL_ERROR("Pb Loading FONT name : '" << m_fileName << "'" );
	}
	
	// set the bassic charset:
	m_listElement.clear();
	if (m_font == NULL) {
		return;
	}
	m_sizeRatio = ((float)SIZE_GENERATION) / ((float)m_font->getHeight(SIZE_GENERATION));
	// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
	setImageSize(ivec2(256,32));
	// now we can acces directly on the image
	m_data.clear(etk::Color<>(0x00000000));
	
	// add error glyph
	addGlyph(0);
	// by default we set only the first AINSI char availlable
	for (int32_t iii=0x20; iii<0x7F; iii++) {
		addGlyph(iii);
	}
	flush();
}

ewol::resource::DistanceFieldFont::~DistanceFieldFont(void) {
	ewol::resource::FontFreeType::release(m_font);
}


float ewol::resource::DistanceFieldFont::getDisplayRatio(float _size) {
	return _size / (float)SIZE_GENERATION;
}


void ewol::resource::DistanceFieldFont::generateDistanceField(const egami::ImageMono& _input, egami::Image& _output) {
	int32_t size = _input.getSize().x() * _input.getSize().y();
	std::vector<short> xdist(size);
	std::vector<short> ydist(size);
	std::vector<double> gx(size);
	std::vector<double> gy(size);
	std::vector<double> data(size);
	std::vector<double> outside(size);
	std::vector<double> inside(size);
	// Convert img into double (data)
	double img_min = 255, img_max = -255;
	for (int32_t yyy = 0; yyy < _input.getSize().y(); ++yyy) {
		for (int32_t xxx = 0; xxx < _input.getSize().x(); ++xxx) {
			int32_t iii = yyy * _input.getSize().x() + xxx;
			double v = _input.get(ivec2(xxx, yyy));
			data[iii] = v;
			if (v > img_max) {
				img_max = v;
			}
			if (v < img_min) {
				img_min = v;
			}
		}
	}
	// Rescale image levels between 0 and 1
	for (int32_t yyy = 0; yyy < _input.getSize().y(); ++yyy) {
		for (int32_t xxx = 0; xxx < _input.getSize().x(); ++xxx) {
			int32_t iii = yyy * _input.getSize().x() + xxx;
			data[iii] = (_input.get(ivec2(xxx, yyy))-img_min)/img_max;
		}
	}
	
	// Compute outside = edtaa3(bitmap); % Transform background (0's)
	computegradient(&data[0], _input.getSize().x(), _input.getSize().y(), &gx[0], &gy[0]);
	edtaa3(&data[0], &gx[0], &gy[0], _input.getSize().x(), _input.getSize().y(), &xdist[0], &ydist[0], &outside[0]);
	for(size_t iii = 0; iii < outside.size(); ++iii) {
		if( outside[iii] < 0 ) {
			outside[iii] = 0.0;
		}
	}
	
	// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
	for(size_t iii = 0; iii < gx.size(); ++iii) {
		gx[iii] = 0;
	}
	for(size_t iii = 0; iii < gy.size(); ++iii) {
		gy[iii] = 0;
	}
	for(size_t iii = 0; iii < data.size(); ++iii) {
		data[iii] = 1 - data[iii];
	}
	computegradient( &data[0], _input.getSize().x(), _input.getSize().y(), &gx[0], &gy[0]);
	edtaa3(&data[0], &gx[0], &gy[0], _input.getSize().x(), _input.getSize().y(), &xdist[0], &ydist[0], &inside[0]);
	for(size_t iii = 0; iii < inside.size(); ++iii) {
		if( inside[iii] < 0 ) {
			inside[iii] = 0.0;
		}
	}
	
	_output.resize(_input.getSize(), etk::Color<>(0));
	_output.clear(etk::Color<>(0));
	for (int32_t xxx = 0; xxx < _output.getSize().x(); ++xxx) {
		for (int32_t yyy = 0; yyy < _output.getSize().y(); ++yyy) {
			int32_t iii = yyy * _output.getSize().x() + xxx;
			outside[iii] -= inside[iii];
			outside[iii] = 128+outside[iii]*16;
			if( outside[iii] < 0 ) {
				outside[iii] = 0;
			}
			if( outside[iii] > 255 ) {
				outside[iii] = 255;
			}
			uint8_t val = 255 - (unsigned char) outside[iii];
			// TODO : Remove multiple size of the map ...
			_output.set(ivec2(xxx, yyy), etk::Color<>((int32_t)val,(int32_t)val,(int32_t)val,256));
		}
	}
}

bool ewol::resource::DistanceFieldFont::addGlyph(const char32_t& _val) {
	bool hasChange = false;
	if (m_font == NULL) {
		return false;
	}
	// add the curent "char"
	GlyphProperty tmpchar;
	tmpchar.m_UVal = _val;
	egami::ImageMono imageGlyphRaw;
	egami::Image imageGlyphDistanceField;
	EWOL_DEBUG("Generate Glyph : " << _val);
	
	if (m_font->getGlyphProperty(SIZE_GENERATION, tmpchar) == true) {
		//EWOL_DEBUG("load char : '" << _val << "'=" << _val.get());
		hasChange = true;
		// change line if needed ...
		if (m_lastGlyphPos.x() + tmpchar.m_sizeTexture.x()+m_borderSize*2.0 > m_data.getSize().x()) {
			m_lastGlyphPos.setX(1);
			m_lastGlyphPos += ivec2(0, m_lastRawHeigh);
			m_lastRawHeigh = 0;
		}
		while(m_lastGlyphPos.y()+tmpchar.m_sizeTexture.y()+m_borderSize*2.0 > m_data.getSize().y()) {
			ivec2 size = m_data.getSize();
			size.setY(size.y()*2);
			m_data.resize(size, etk::Color<>(0));
			// change the coordonate on the element in the texture
			for (size_t jjj = 0; jjj < m_listElement.size(); ++jjj) {
				m_listElement[jjj].m_texturePosStart *= vec2(1.0f, 0.5f);
				m_listElement[jjj].m_texturePosSize *= vec2(1.0f, 0.5f);
			}
		}
		m_textureBorderSize = vec2(m_borderSize/(float)m_data.getSize().x(),
		                           m_borderSize/(float)m_data.getSize().y() );
		// draw the glyph
		m_font->drawGlyph(imageGlyphRaw, SIZE_GENERATION, tmpchar, m_borderSize);
		
		generateDistanceField(imageGlyphRaw, imageGlyphDistanceField);
		/*
		if (_val == 'Z') {
			for (int32_t yyy = 0; yyy < imageGlyphDistanceField.getSize().y(); ++yyy) {
				for (int32_t xxx = 0; xxx < imageGlyphDistanceField.getSize().x(); ++xxx) {
					std::cout << (int)(imageGlyphDistanceField.get(ivec2(xxx, yyy)).r()) << "	";
				}
				//std::cout << std::endl;
			}
		}
		*/
		m_data.insert(m_lastGlyphPos, imageGlyphDistanceField);
		
		// set image position
		tmpchar.m_texturePosStart.setValue( ((float)m_lastGlyphPos.x()+(m_borderSize*0.5f)) / (float)m_data.getSize().x(),
		                                    ((float)m_lastGlyphPos.y()+(m_borderSize*0.5f)) / (float)m_data.getSize().y() );
		tmpchar.m_texturePosSize.setValue(  ((float)imageGlyphRaw.getSize().x()-m_borderSize) / (float)m_data.getSize().x(),
		                                    ((float)imageGlyphRaw.getSize().y()-m_borderSize) / (float)m_data.getSize().y() );
		
		// update the maximum of the line hight : 
		if (m_lastRawHeigh < imageGlyphRaw.getSize().y()) {
			// note : +1 is for the overlapping of the glyph (Part 2)
			m_lastRawHeigh = imageGlyphRaw.getSize().y()+1;
		}
		// note : +1 is for the overlapping of the glyph (Part 3)
		// update the Bitmap position drawing : 
		m_lastGlyphPos += ivec2(imageGlyphRaw.getSize().x()+1, 0);
	} else {
		EWOL_WARNING("Did not find char : '" << _val << "'=" << _val);
		tmpchar.setNotExist();
	}
	m_listElement.push_back(tmpchar);
	//m_font[iii]->display();
	// generate the kerning for all the characters :
	if (tmpchar.exist() == true) {
		// TODO : set the kerning back ...
		//m_font[iii]->generateKerning(m_size, m_listElement[iii]);
	}
	if (hasChange == true) {
		flush();
		egami::store(m_data, "fileFont.bmp"); // ==> for debug test only ...
	}
	return hasChange;
}

int32_t ewol::resource::DistanceFieldFont::getIndex(char32_t _charcode) {
	if (_charcode < 0x20) {
		return 0;
	} else if (_charcode < 0x80) {
		return _charcode - 0x1F;
	} else {
		for (size_t iii=0x80-0x20; iii < m_listElement.size(); iii++) {
			//EWOL_DEBUG("search : '" << charcode << "' =?= '" << (m_listElement[displayMode])[iii].m_UVal << "'");
			if (_charcode == (m_listElement)[iii].m_UVal) {
				//EWOL_DEBUG("search : '" << charcode << "'");
				if ((m_listElement)[iii].exist()) {
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

ewol::GlyphProperty* ewol::resource::DistanceFieldFont::getGlyphPointer(const char32_t& _charcode) {
	//EWOL_DEBUG("Get glyph property for mode: " << _displayMode << "  == > wrapping index : " << m_modeWraping[_displayMode]);
	int32_t index = getIndex(_charcode);
	if(    index < 0
	    || (size_t)index >= m_listElement.size() ) {
		EWOL_ERROR(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" << index);
		if (m_listElement.size() > 0) {
			return &((m_listElement)[0]);
		}
		return NULL;
	}
	//EWOL_ERROR("      index=" << index);
	//EWOL_ERROR("      m_UVal=" << m_listElement[_displayMode][index].m_UVal);
	//EWOL_ERROR("      m_glyphIndex=" << m_listElement[_displayMode][index].m_glyphIndex);
	//EWOL_ERROR("      m_advance=" << m_listElement[_displayMode][index].m_advance);
	//EWOL_ERROR("      m_bearing=" << m_listElement[_displayMode][index].m_bearing);
	return &((m_listElement)[index]);
}

ewol::resource::DistanceFieldFont* ewol::resource::DistanceFieldFont::keep(const std::string& _filename) {
	EWOL_VERBOSE("KEEP : DistanceFieldFont : file : '" << _filename << "'");
	ewol::resource::DistanceFieldFont* object = static_cast<ewol::resource::DistanceFieldFont*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	// need to crate a new one ...
	EWOL_DEBUG("CREATE: DistanceFieldFont : file : '" << _filename << "'");
	object = new ewol::resource::DistanceFieldFont(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::resource::DistanceFieldFont::release(ewol::resource::DistanceFieldFont*& _object) {
	if (NULL == _object) {
		return;
	}
	std::string name = _object->getName();
	int32_t count = _object->getCounter() - 1;
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	if (getManager().release(object2) == true) {
		EWOL_DEBUG("REMOVE: DistanceFieldFont : file : '" << name << "' count=" << count);
		//etk::displayBacktrace(false);
	}
	_object = NULL;
}
