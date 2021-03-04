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
  this.borderSize(10),
  this.textureBorderSize(0,0) {
	addResourceType("ewol::resource::DistanceFieldFont");
	this.font = null;
	this.lastGlyphPos.setValue(1,1);
	this.lastRawHeigh = 0;
	this.sizeRatio = 1.0f;
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

void ewol::resource::DistanceFieldFont::init( String _fontName) {
	ethread::RecursiveLock lock(this.mutex);
	ewol::resource::Texture::init(_fontName);
	String localName = _fontName;
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
		Log.info("try to find font named : " + split + " in: " + output);
		//Log.critical("parse string : " + split);
		boolean hasFindAFont = false;
		for (int jjj=0; jjj<split.size(); jjj++) {
			Log.info("    try with : '" + split[jjj] + "'");
			for (int iii=0; iii<output.size(); iii++) {
				String nameFolder = output[iii].getPath().getString();
				//Log.debug(" file : " + output[iii]);
				if(    true == etk::end_with(nameFolder, split[jjj]+"-"+"regular"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+"-"+"r"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+"regular"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+"r"+".ttf", false)
				    || true == etk::end_with(nameFolder, split[jjj]+".ttf", false)) {
					Log.info(" find Font [Regular]     : " + output[iii]);
					this.fileName = output[iii];
					hasFindAFont=true;
					break;
				}
			}
			if (hasFindAFont == true) {
				Log.info("    find this font : '" + split[jjj] + "'");
				break;
			} else if (jjj == split.size()-1) {
				Log.error("Find NO font in the LIST ... " + split);
			}
		}
		if (hasFindAFont == true) {
			Log.info("    find this font : '" + folderList[folderID] + "'");
			break;
		} else if (folderID == folderList.size()-1) {
			Log.error("Find NO font in the LIST ... " + folderList);
		}
	}
	
	if (this.fileName.isEmpty() == true) {
		Log.error("can not load FONT name : '" + _fontName + "'" );
		this.font = null;
		return;
	}
	Log.info("Load FONT name : '" + this.fileName + "'");
	this.font = ewol::resource::FontFreeType::create(this.fileName);
	if (this.font == null) {
		Log.error("Pb Loading FONT name : '" + this.fileName + "'" );
	}
	
	// set the bassic charset:
	this.listElement.clear();
	if (this.font == null) {
		return;
	}
	if (importFromFile() == true) {
		Log.info("GET distance field from previous file");
		flush();
		return;
	}
	
	this.sizeRatio = ((float)SIZE_GENERATION) / ((float)this.font.getHeight(SIZE_GENERATION));
	// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
	setImageSize(Vector2i(512,32));
	// now we can acces directly on the image
	this.data.clear(etk::Color<>(0x00000000));
	// add error glyph
	addGlyph(0);
	// by default we set only the first AINSI char availlable
	for (int iii=0x20; iii<0x7F; iii++) {
		addGlyph(iii);
	}
	flush();
	if (true) {
		Log.error("Save in cache the loaded data ..... ");
		egami::store(this.data, "CACHE:///fileFont.bmp"); // ==> for debug test only ...
		egami::store(this.data, "CACHE:///fileFont.png");
	}
	exportOnFile();
}

ewol::resource::DistanceFieldFont::~DistanceFieldFont() {
	
}


float ewol::resource::DistanceFieldFont::getDisplayRatio(float _size) {
	ethread::RecursiveLock lock(this.mutex);
	return _size / (float)SIZE_GENERATION;
}


void ewol::resource::DistanceFieldFont::generateDistanceField( egami::ImageMono _input, egami::Image _output) {
	Log.info("Generate Distance field font [START]");
	Log.info("    _input.getSize()=" + _input.getSize());
	ethread::RecursiveLock lock(this.mutex);
	int size = _input.getSize().x() * _input.getSize().y();
	List<short> xdist;
	List<short> ydist;
	List<double> gx;
	List<double> gy;
	List<double> data;
	List<double> outside;
	List<double> inside;
	xdist.resize(size, 0);
	ydist.resize(size, 0);
	gx.resize(size, 0.0);
	gy.resize(size, 0.0);
	data.resize(size, 0.0);
	outside.resize(size, 0.0);
	inside.resize(size, 0.0);
	Log.info("    size=" + size);
	// Convert img into double (data)
	double img_min = 255, img_max = -255;
	for (int yyy = 0; yyy < _input.getSize().y(); ++yyy) {
		for (int xxx = 0; xxx < _input.getSize().x(); ++xxx) {
			int iii = yyy * _input.getSize().x() + xxx;
			double v = _input.get(Vector2i(xxx, yyy));
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
	for (int yyy = 0; yyy < _input.getSize().y(); ++yyy) {
		for (int xxx = 0; xxx < _input.getSize().x(); ++xxx) {
			int iii = yyy * _input.getSize().x() + xxx;
			data[iii] = (_input.get(Vector2i(xxx, yyy))-img_min)/img_max;
		}
	}
	// Compute outside = edtaa3(bitmap); % Transform background (0's)
	computegradient(data[0], _input.getSize().x(), _input.getSize().y(), gx[0], gy[0]);
	edtaa3(data[0], gx[0], gy[0], _input.getSize().x(), _input.getSize().y(), xdist[0], &ydist[0], &outside[0]);
	for(int iii = 0; iii < outside.size(); ++iii) {
		if( outside[iii] < 0 ) {
			outside[iii] = 0.0;
		}
	}
	// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
	for(int iii = 0; iii < gx.size(); ++iii) {
		gx[iii] = 0;
	}
	for(int iii = 0; iii < gy.size(); ++iii) {
		gy[iii] = 0;
	}
	for(int iii = 0; iii < data.size(); ++iii) {
		data[iii] = 1 - data[iii];
	}
	computegradient( data[0], _input.getSize().x(), _input.getSize().y(), gx[0], gy[0]);
	edtaa3(data[0], gx[0], gy[0], _input.getSize().x(), _input.getSize().y(), xdist[0], &ydist[0], &inside[0]);
	for(int iii = 0; iii < inside.size(); ++iii) {
		if( inside[iii] < 0 ) {
			inside[iii] = 0.0;
		}
	}
	Log.info("    _output=" + _output);
	_output.resize(_input.getSize(), etk::Color<>(0));
	_output.clear(etk::Color<>(0));
	for (int xxx = 0; xxx < _output.getSize().x(); ++xxx) {
		for (int yyy = 0; yyy < _output.getSize().y(); ++yyy) {
			int iii = yyy * _output.getSize().x() + xxx;
			outside[iii] -= inside[iii];
			outside[iii] = 128+outside[iii]*16;
			if( outside[iii] < 0 ) {
				outside[iii] = 0;
			}
			if( outside[iii] > 255 ) {
				outside[iii] = 255;
			}
			int val = 255 - (unsigned char) outside[iii];
			// TODO : Remove multiple size of the map ...
			_output.set(Vector2i(xxx, yyy), etk::Color<>((int)val,(int)val,(int)val,255));
		}
	}
	Log.info("    _output=" + _output);
}

boolean ewol::resource::DistanceFieldFont::addGlyph( Character _val) {
	ethread::RecursiveLock lock(this.mutex);
	boolean hasChange = false;
	if (this.font == null) {
		return false;
	}
	// add the curent "char"
	GlyphProperty tmpchar;
	tmpchar.this.UVal = _val;
	egami::ImageMono imageGlyphRaw;
	egami::Image imageGlyphDistanceField(Vector2i(32,32), egami::colorType::RGBA8);
	Log.debug("Generate Glyph : " + _val);
	
	if (this.font.getGlyphProperty(SIZE_GENERATION, tmpchar) == true) {
		//Log.debug("load char: '" + _val + "'=" + _val);
		hasChange = true;
		// change line if needed ...
		if (this.lastGlyphPos.x() + tmpchar.this.sizeTexture.x()+this.borderSize*2.0 > this.data.getSize().x()) {
			this.lastGlyphPos.setX(1);
			this.lastGlyphPos += Vector2i(0, this.lastRawHeigh);
			this.lastRawHeigh = 0;
		}
		while(this.lastGlyphPos.y()+tmpchar.this.sizeTexture.y()+this.borderSize*2.0 > this.data.getSize().y()) {
			Vector2i size = this.data.getSize();
			size.setY(size.y()*2);
			Log.verbose("resize " + this.data.getSize() + " => " + size);
			this.data.resize(size, etk::Color<>(0));
			// change the coordonate on the element in the texture
			for (int jjj = 0; jjj < this.listElement.size(); ++jjj) {
				this.listElement[jjj].this.texturePosStart *= Vector2f(1.0f, 0.5f);
				this.listElement[jjj].this.texturePosSize *= Vector2f(1.0f, 0.5f);
			}
		}
		this.textureBorderSize = Vector2f(this.borderSize/(float)this.data.getSize().x(),
		                           this.borderSize/(float)this.data.getSize().y() );
		// draw the glyph
		this.font.drawGlyph(imageGlyphRaw, SIZE_GENERATION, tmpchar, this.borderSize);
		
		generateDistanceField(imageGlyphRaw, imageGlyphDistanceField);
		
		if (_val == 100) {
			Log.debug("print char: " + _val + " size=" + imageGlyphDistanceField.getSize());
			for (int yyy = 0; yyy < imageGlyphDistanceField.getSize().y(); ++yyy) {
				for (int xxx = 0; xxx < imageGlyphDistanceField.getSize().x(); ++xxx) {
					Log.print((int)(imageGlyphDistanceField.get(Vector2i(xxx, yyy)).r()) + "	");
				}
			}
		}
		
		this.data.insert(this.lastGlyphPos, imageGlyphDistanceField);
		
		// set image position
		tmpchar.this.texturePosStart.setValue( ((float)this.lastGlyphPos.x()+(this.borderSize*0.5f)) / (float)this.data.getSize().x(),
		                                    ((float)this.lastGlyphPos.y()+(this.borderSize*0.5f)) / (float)this.data.getSize().y() );
		tmpchar.this.texturePosSize.setValue(  ((float)imageGlyphRaw.getSize().x()-this.borderSize) / (float)this.data.getSize().x(),
		                                    ((float)imageGlyphRaw.getSize().y()-this.borderSize) / (float)this.data.getSize().y() );
		
		// update the maximum of the line hight : 
		if (this.lastRawHeigh < imageGlyphRaw.getSize().y()) {
			// note : +1 is for the overlapping of the glyph (Part 2)
			this.lastRawHeigh = imageGlyphRaw.getSize().y()+1;
		}
		// note : +1 is for the overlapping of the glyph (Part 3)
		// update the Bitmap position drawing : 
		this.lastGlyphPos += Vector2i(imageGlyphRaw.getSize().x()+1, 0);
	} else {
		Log.warning("Did not find char : '" + _val + "'=" + _val);
		tmpchar.setNotExist();
	}
	this.listElement.pushBack(tmpchar);
	//this.font[iii].display();
	// generate the kerning for all the characters :
	if (tmpchar.exist() == true) {
		// TODO : set the kerning back ...
		//this.font[iii].generateKerning(this.size, this.listElement[iii]);
	}
	if (hasChange == true) {
		flush();
		//Log.error("Save in cache the loaded data ..... ");
		//egami::store(this.data, "CACHE:///fileFont.bmp"); // ==> for debug test only ...
		//egami::store(this.data, "CACHE:///fileFont.png");
	}
	return hasChange;
}

int ewol::resource::DistanceFieldFont::getIndex(Character _charcode) {
	ethread::RecursiveLock lock(this.mutex);
	if (_charcode < 0x20) {
		return 0;
	} else if (_charcode < 0x80) {
		return _charcode - 0x1F;
	} else {
		for (int iii=0x80-0x20; iii < this.listElement.size(); iii++) {
			//Log.debug("search : '" + charcode + "' =?= '" + (this.listElement[displayMode])[iii].this.UVal + "'");
			if (_charcode == (this.listElement)[iii].this.UVal) {
				//Log.debug("search : '" + charcode + "'");
				if ((this.listElement)[iii].exist()) {
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

ewol::GlyphProperty* ewol::resource::DistanceFieldFont::getGlyphPointer( Character _charcode) {
	ethread::RecursiveLock lock(this.mutex);
	Log.verbose("getGlyphPointer : " + uint(_charcode));
	int index = getIndex(_charcode);
	if(    index < 0
	    || (int)index >= this.listElement.size() ) {
		Log.error(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" + index);
		if (this.listElement.size() > 0) {
			return ((this.listElement)[0]);
		}
		return null;
	}
	//Log.error("      index=" + index);
	//Log.error("      this.UVal=" + this.listElement[_displayMode][index].this.UVal);
	//Log.error("      this.glyphIndex=" + this.listElement[_displayMode][index].this.glyphIndex);
	//Log.error("      this.advance=" + this.listElement[_displayMode][index].this.advance);
	//Log.error("      this.bearing=" + this.listElement[_displayMode][index].this.bearing);
	return ((this.listElement)[index]);
}

void ewol::resource::DistanceFieldFont::exportOnFile() {
	ethread::RecursiveLock lock(this.mutex);
	Log.debug("EXPORT: DistanceFieldFont : file : '" + this.fileName + ".json'");
	ejson::Document doc;
	ejson::Array tmpList;
	for (int iii=0; iii<this.listElement.size(); ++iii) {
		ejson::Object tmpObj;
		tmpObj.add("this.UVal", ejson::String(etk::toString(this.listElement[iii].this.UVal)));
		tmpObj.add("this.glyphIndex", ejson::Number(this.listElement[iii].this.glyphIndex));
		tmpObj.add("this.sizeTexture", ejson::String((String)this.listElement[iii].this.sizeTexture));
		tmpObj.add("this.bearing", ejson::String((String)this.listElement[iii].this.bearing));
		tmpObj.add("this.advance", ejson::String((String)this.listElement[iii].this.advance));
		tmpObj.add("this.texturePosStart", ejson::String((String)this.listElement[iii].this.texturePosStart));
		tmpObj.add("this.texturePosSize", ejson::String((String)this.listElement[iii].this.texturePosSize));
		tmpObj.add("this.exist", ejson::Boolean(this.listElement[iii].this.exist));
		tmpList.add(tmpObj);
	}
	doc.add("this.listElement", tmpList);
	doc.add("this.sizeRatio", ejson::Number(this.sizeRatio));
	doc.add("this.lastGlyphPos", ejson::String(this.lastGlyphPos));
	doc.add("this.lastRawHeigh", ejson::Number(this.lastRawHeigh));
	doc.add("this.borderSize", ejson::Number(this.borderSize));
	doc.add("this.textureBorderSize", ejson::String(this.textureBorderSize));
	etk::Uri tmpUri = this.fileName;
	tmpUri.setScheme("CACHE");
	tmpUri.setPath(this.fileName.getPath() + ".json");
	doc.store(tmpUri);
	//tmpUri.setPath(this.fileName.getPath() + ".bmp");
	//egami::store(this.data, tmpUri);
	tmpUri.setPath(this.fileName.getPath() + ".png");
	egami::store(this.data, tmpUri);
}

boolean ewol::resource::DistanceFieldFont::importFromFile() {
	ethread::RecursiveLock lock(this.mutex);
	etk::Uri tmpUriJson = this.fileName;
	tmpUriJson.setScheme("CACHE");
	tmpUriJson.setPath(this.fileName.getPath() + ".json");
	etk::Uri tmpUriBmp = this.fileName;
	tmpUriBmp.setScheme("CACHE");
	tmpUriBmp.setPath(this.fileName.getPath() + ".png");
	Log.debug("IMPORT: DistanceFieldFont : file : '" + tmpUriJson + "'");
	// test file existance:
	if (    etk::uri::exist(tmpUriJson) == false
	     || etk::uri::exist(tmpUriBmp) == false) {
		Log.debug("Does not import file for distance field system");
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
	this.data = tmpImage;
	this.sizeRatio = doc["this.sizeRatio"].toNumber().get(0);
	this.lastGlyphPos = doc["this.lastGlyphPos"].toString().get("0,0");
	this.lastRawHeigh = doc["this.lastRawHeigh"].toNumber().get(0);
	this.borderSize = doc["this.borderSize"].toNumber().get(2);
	this.textureBorderSize = doc["this.textureBorderSize"].toString().get("0,0");
	ejson::Array tmpList = doc["this.listElement"].toArray();
	if (tmpList.exist() == false) {
		Log.error("null pointer array");
		return false;
	}
	this.listElement.clear();
	for ( auto it : tmpList) {
		 ejson::Object tmpObj = it.toObject();
		if (tmpObj.exist() == false) {
			continue;
		}
		GlyphProperty prop;
		prop.this.UVal = etk::string_to_int(tmpObj["this.UVal"].toString().get("0"));
		prop.this.glyphIndex = tmpObj["this.glyphIndex"].toNumber().get(0);
		prop.this.sizeTexture = tmpObj["this.sizeTexture"].toString().get("0,0");
		prop.this.bearing = tmpObj["this.bearing"].toString().get("0,0");
		prop.this.advance = tmpObj["this.advance"].toString().get("0,0");
		prop.this.texturePosStart = tmpObj["this.texturePosStart"].toString().get("0,0");
		prop.this.texturePosSize = tmpObj["this.texturePosSize"].toString().get("0,0");
		prop.this.exist = tmpObj["this.exist"].toBoolean().get(false);
		this.listElement.pushBack(prop);
	}
	return this.data.exist();
}
