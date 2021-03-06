/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <etk/uri/uri.hpp>
#include <egami/egami.hpp>

#include <gale/resource/Manager.hpp>

#include <ewol/resource/font/FontBase.hpp>
#include <ewol/resource/TexturedFont.hpp>
#include <ewol/resource/FontFreeType.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/resource/DistanceFieldFont.hpp>
#include <edtaa3/edtaa3func.h>
#include <ejson/ejson.hpp>

#define SIZE_GENERATION (30)

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::DistanceFieldFont);

ewol::resource::DistanceFieldFont::DistanceFieldFont() :
  ewol::resource::Texture(),
  m_borderSize(10),
  m_textureBorderSize(0,0) {
	addResourceType("ewol::resource::DistanceFieldFont");
	m_font = null;
	m_lastGlyphPos.setValue(1,1);
	m_lastRawHeigh = 0;
	m_sizeRatio = 1.0f;
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

void ewol::resource::DistanceFieldFont::init(const etk::String& _fontName) {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init(_fontName);
	etk::String localName = _fontName;
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
		EWOL_INFO("try to find font named : " << split << " in: " << output);
		//EWOL_CRITICAL("parse string : " << split);
		bool hasFindAFont = false;
		for (size_t jjj=0; jjj<split.size(); jjj++) {
			EWOL_INFO("    try with : '" << split[jjj] << "'");
			for (size_t iii=0; iii<output.size(); iii++) {
				etk::String nameFolder = output[iii].getPath().getString();
				//EWOL_DEBUG(" file : " << output[iii]);
				if(    true == etk::end_with(nameFolder, split[jjj]+"-"+"regular"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+"-"+"r"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+"regular"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+"r"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+".ttf", false)) {
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
	
	if (m_fileName.isEmpty() == true) {
		EWOL_ERROR("can not load FONT name : '" << _fontName << "'" );
		m_font = null;
		return;
	}
	EWOL_INFO("Load FONT name : '" << m_fileName << "'");
	m_font = ewol::resource::FontFreeType::create(m_fileName);
	if (m_font == null) {
		EWOL_ERROR("Pb Loading FONT name : '" << m_fileName << "'" );
	}
	
	// set the bassic charset:
	m_listElement.clear();
	if (m_font == null) {
		return;
	}
	if (importFromFile() == true) {
		EWOL_INFO("GET distance field from previous file");
		flush();
		return;
	}
	
	m_sizeRatio = ((float)SIZE_GENERATION) / ((float)m_font->getHeight(SIZE_GENERATION));
	// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
	setImageSize(ivec2(512,32));
	// now we can acces directly on the image
	m_data.clear(etk::Color<>(0x00000000));
	// add error glyph
	addGlyph(0);
	// by default we set only the first AINSI char availlable
	for (int32_t iii=0x20; iii<0x7F; iii++) {
		addGlyph(iii);
	}
	flush();
	if (true) {
		EWOL_ERROR("Save in cache the loaded data ..... ");
		egami::store(m_data, "CACHE:///fileFont.bmp"); // ==> for debug test only ...
		egami::store(m_data, "CACHE:///fileFont.png");
	}
	exportOnFile();
}

ewol::resource::DistanceFieldFont::~DistanceFieldFont() {
	
}


float ewol::resource::DistanceFieldFont::getDisplayRatio(float _size) {
	ethread::RecursiveLock lock(m_mutex);
	return _size / (float)SIZE_GENERATION;
}


void ewol::resource::DistanceFieldFont::generateDistanceField(const egami::ImageMono& _input, egami::Image& _output) {
	EWOL_INFO("Generate Distance field font [START]");
	EWOL_INFO("    _input.getSize()=" << _input.getSize());
	ethread::RecursiveLock lock(m_mutex);
	int32_t size = _input.getSize().x() * _input.getSize().y();
	etk::Vector<short> xdist;
	etk::Vector<short> ydist;
	etk::Vector<double> gx;
	etk::Vector<double> gy;
	etk::Vector<double> data;
	etk::Vector<double> outside;
	etk::Vector<double> inside;
	xdist.resize(size, 0);
	ydist.resize(size, 0);
	gx.resize(size, 0.0);
	gy.resize(size, 0.0);
	data.resize(size, 0.0);
	outside.resize(size, 0.0);
	inside.resize(size, 0.0);
	EWOL_INFO("    size=" << size);
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
	EWOL_INFO("    _output=" << _output);
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
			_output.set(ivec2(xxx, yyy), etk::Color<>((int32_t)val,(int32_t)val,(int32_t)val,255));
		}
	}
	EWOL_INFO("    _output=" << _output);
}

bool ewol::resource::DistanceFieldFont::addGlyph(const char32_t& _val) {
	ethread::RecursiveLock lock(m_mutex);
	bool hasChange = false;
	if (m_font == null) {
		return false;
	}
	// add the curent "char"
	GlyphProperty tmpchar;
	tmpchar.m_UVal = _val;
	egami::ImageMono imageGlyphRaw;
	egami::Image imageGlyphDistanceField(ivec2(32,32), egami::colorType::RGBA8);
	EWOL_DEBUG("Generate Glyph : " << _val);
	
	if (m_font->getGlyphProperty(SIZE_GENERATION, tmpchar) == true) {
		//EWOL_DEBUG("load char: '" << _val << "'=" << _val);
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
			EWOL_VERBOSE("resize " << m_data.getSize() << " => " << size);
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
		
		if (_val == 100) {
			EWOL_DEBUG("print char: " << _val << " size=" << imageGlyphDistanceField.getSize());
			for (int32_t yyy = 0; yyy < imageGlyphDistanceField.getSize().y(); ++yyy) {
				for (int32_t xxx = 0; xxx < imageGlyphDistanceField.getSize().x(); ++xxx) {
					EWOL_PRINT((int)(imageGlyphDistanceField.get(ivec2(xxx, yyy)).r()) << "	");
				}
			}
		}
		
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
	m_listElement.pushBack(tmpchar);
	//m_font[iii]->display();
	// generate the kerning for all the characters :
	if (tmpchar.exist() == true) {
		// TODO : set the kerning back ...
		//m_font[iii]->generateKerning(m_size, m_listElement[iii]);
	}
	if (hasChange == true) {
		flush();
		//EWOL_ERROR("Save in cache the loaded data ..... ");
		//egami::store(m_data, "CACHE:///fileFont.bmp"); // ==> for debug test only ...
		//egami::store(m_data, "CACHE:///fileFont.png");
	}
	return hasChange;
}

int32_t ewol::resource::DistanceFieldFont::getIndex(char32_t _charcode) {
	ethread::RecursiveLock lock(m_mutex);
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
	ethread::RecursiveLock lock(m_mutex);
	EWOL_VERBOSE("getGlyphPointer : " << uint32_t(_charcode));
	int32_t index = getIndex(_charcode);
	if(    index < 0
	    || (size_t)index >= m_listElement.size() ) {
		EWOL_ERROR(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" << index);
		if (m_listElement.size() > 0) {
			return &((m_listElement)[0]);
		}
		return null;
	}
	//EWOL_ERROR("      index=" << index);
	//EWOL_ERROR("      m_UVal=" << m_listElement[_displayMode][index].m_UVal);
	//EWOL_ERROR("      m_glyphIndex=" << m_listElement[_displayMode][index].m_glyphIndex);
	//EWOL_ERROR("      m_advance=" << m_listElement[_displayMode][index].m_advance);
	//EWOL_ERROR("      m_bearing=" << m_listElement[_displayMode][index].m_bearing);
	return &((m_listElement)[index]);
}

void ewol::resource::DistanceFieldFont::exportOnFile() {
	ethread::RecursiveLock lock(m_mutex);
	EWOL_DEBUG("EXPORT: DistanceFieldFont : file : '" << m_fileName << ".json'");
	ejson::Document doc;
	ejson::Array tmpList;
	for (size_t iii=0; iii<m_listElement.size(); ++iii) {
		ejson::Object tmpObj;
		tmpObj.add("m_UVal", ejson::String(etk::toString(m_listElement[iii].m_UVal)));
		tmpObj.add("m_glyphIndex", ejson::Number(m_listElement[iii].m_glyphIndex));
		tmpObj.add("m_sizeTexture", ejson::String((etk::String)m_listElement[iii].m_sizeTexture));
		tmpObj.add("m_bearing", ejson::String((etk::String)m_listElement[iii].m_bearing));
		tmpObj.add("m_advance", ejson::String((etk::String)m_listElement[iii].m_advance));
		tmpObj.add("m_texturePosStart", ejson::String((etk::String)m_listElement[iii].m_texturePosStart));
		tmpObj.add("m_texturePosSize", ejson::String((etk::String)m_listElement[iii].m_texturePosSize));
		tmpObj.add("m_exist", ejson::Boolean(m_listElement[iii].m_exist));
		tmpList.add(tmpObj);
	}
	doc.add("m_listElement", tmpList);
	doc.add("m_sizeRatio", ejson::Number(m_sizeRatio));
	doc.add("m_lastGlyphPos", ejson::String(m_lastGlyphPos));
	doc.add("m_lastRawHeigh", ejson::Number(m_lastRawHeigh));
	doc.add("m_borderSize", ejson::Number(m_borderSize));
	doc.add("m_textureBorderSize", ejson::String(m_textureBorderSize));
	etk::Uri tmpUri = m_fileName;
	tmpUri.setScheme("CACHE");
	tmpUri.setPath(m_fileName.getPath() + ".json");
	doc.store(tmpUri);
	//tmpUri.setPath(m_fileName.getPath() + ".bmp");
	//egami::store(m_data, tmpUri);
	tmpUri.setPath(m_fileName.getPath() + ".png");
	egami::store(m_data, tmpUri);
}

bool ewol::resource::DistanceFieldFont::importFromFile() {
	ethread::RecursiveLock lock(m_mutex);
	etk::Uri tmpUriJson = m_fileName;
	tmpUriJson.setScheme("CACHE");
	tmpUriJson.setPath(m_fileName.getPath() + ".json");
	etk::Uri tmpUriBmp = m_fileName;
	tmpUriBmp.setScheme("CACHE");
	tmpUriBmp.setPath(m_fileName.getPath() + ".png");
	EWOL_DEBUG("IMPORT: DistanceFieldFont : file : '" << tmpUriJson << "'");
	// test file existance:
	if (    etk::uri::exist(tmpUriJson) == false
	     || etk::uri::exist(tmpUriBmp) == false) {
		EWOL_DEBUG("Does not import file for distance field system");
		return false;
	}
	ejson::Document doc;
	if (doc.load(tmpUriJson) == false) {
		return false;
	}
	egami::Image tmpImage = egami::load(tmpUriBmp);
	if (tmpImage.exist() == false) {
		return false;
	}
	m_data = tmpImage;
	m_sizeRatio = doc["m_sizeRatio"].toNumber().get(0);
	m_lastGlyphPos = doc["m_lastGlyphPos"].toString().get("0,0");
	m_lastRawHeigh = doc["m_lastRawHeigh"].toNumber().get(0);
	m_borderSize = doc["m_borderSize"].toNumber().get(2);
	m_textureBorderSize = doc["m_textureBorderSize"].toString().get("0,0");
	ejson::Array tmpList = doc["m_listElement"].toArray();
	if (tmpList.exist() == false) {
		EWOL_ERROR("null pointer array");
		return false;
	}
	m_listElement.clear();
	for (const auto it : tmpList) {
		const ejson::Object tmpObj = it.toObject();
		if (tmpObj.exist() == false) {
			continue;
		}
		GlyphProperty prop;
		prop.m_UVal = etk::string_to_int32_t(tmpObj["m_UVal"].toString().get("0"));
		prop.m_glyphIndex = tmpObj["m_glyphIndex"].toNumber().get(0);
		prop.m_sizeTexture = tmpObj["m_sizeTexture"].toString().get("0,0");
		prop.m_bearing = tmpObj["m_bearing"].toString().get("0,0");
		prop.m_advance = tmpObj["m_advance"].toString().get("0,0");
		prop.m_texturePosStart = tmpObj["m_texturePosStart"].toString().get("0,0");
		prop.m_texturePosSize = tmpObj["m_texturePosSize"].toString().get("0,0");
		prop.m_exist = tmpObj["m_exist"].toBoolean().get(false);
		m_listElement.pushBack(prop);
	}
	return m_data.exist();
}
