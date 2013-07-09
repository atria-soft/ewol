/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/os/FSNode.h>

#include <ewol/config.h>

#include <ewol/renderer/ResourceManager.h>

#include <ewol/renderer/resources/font/FontBase.h>
#include <ewol/renderer/resources/TexturedFont.h>


etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::font::mode_te& _obj)
{
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
#define __class__	"TexturedFont"

static int32_t nextP2(int32_t value)
{
	int32_t val=1;
	for (int32_t iii=1; iii<31; iii++) {
		if (value <= val) {
			return val;
		}
		val *=2;
	}
	EWOL_CRITICAL("impossible CASE....");
	return val;
}

static int32_t simpleSQRT(int32_t value)
{
	int32_t val=1;
	for (int32_t iii=1; iii<1000; iii++) {
		val =iii*iii;
		if (value <= val) {
			return iii;
		}
	}
	EWOL_CRITICAL("impossible CASE....");
	return val;
}

static bool& GetFontInSystem(void)
{
	static bool fontInOs = true;
	return fontInOs;
}

void ewol::font::SetFontPropety(bool inOSSystem)
{
	GetFontInSystem() = inOSSystem;
}

ewol::TexturedFont::TexturedFont(etk::UString fontName) : 
	ewol::Texture(fontName)
{
	m_font[0] = NULL;
	m_font[1] = NULL;
	m_font[2] = NULL;
	m_font[3] = NULL;
	
	m_modeWraping[0] = ewol::font::Regular;
	m_modeWraping[1] = ewol::font::Regular;
	m_modeWraping[2] = ewol::font::Regular;
	m_modeWraping[3] = ewol::font::Regular;
	
	m_lastGlyphPos[0].setValue(0,0);
	m_lastGlyphPos[1].setValue(0,0);
	m_lastGlyphPos[2].setValue(0,0);
	m_lastGlyphPos[3].setValue(0,0);
	
	m_lastRawHeigh[0] = 0;
	m_lastRawHeigh[1] = 0;
	m_lastRawHeigh[2] = 0;
	m_lastRawHeigh[3] = 0;
	
	int32_t tmpSize = 0;
	// extarct name and size :
	etk::Char tmpChar = fontName.c_str();
	const char * tmpData = tmpChar;
	const char * tmpPos = strchr(tmpData, ':');
	
	if (tmpPos==NULL) {
		m_size = 1;
		EWOL_CRITICAL("Can not parse the font name : \"" << fontName << "\" ??? ':' " );
		return;
	} else {
		if (sscanf(tmpPos+1, "%d", &tmpSize)!=1) {
			m_size = 1;
			EWOL_CRITICAL("Can not parse the font name : \"" << fontName << "\" ==> size ???");
			return;
		}
	}
	m_name = fontName.Extract(0, (tmpPos - tmpData));
	m_size = tmpSize;
	
	etk::UString fontBaseFolder("DATA:fonts");
	if (true==GetFontInSystem()) {
		#if defined(__TARGET_OS__Android)
			fontBaseFolder = "/system/font";
		#elif defined(__TARGET_OS__Linux)
			fontBaseFolder = "/usr/share/fonts/truetype";
		#endif
	}
	etk::FSNode myFolder(fontBaseFolder);
	// find the real Font name :
	etk::Vector<etk::UString> output;
	myFolder.FolderGetRecursiveFiles(output);
	etk::Vector<etk::UString> split = m_name.Split(';');
	EWOL_INFO("try to find font named : '" << split << "' in : '" << myFolder <<"'");
	//EWOL_CRITICAL("parse string : " << split);
	
	for (int32_t jjj=0; jjj<split.Size(); jjj++) {
		EWOL_INFO("    try with : '" << split[jjj] << "'");
		bool hasFindAFont = false;
		for (int32_t iii=0; iii<output.Size(); iii++) {
			//EWOL_DEBUG(" file : " << output[iii]);
			if(    true == output[iii].EndWith(split[jjj]+"-"+"bold"+".ttf", false)
			    || true == output[iii].EndWith(split[jjj]+"-"+"b"+".ttf", false)
			    || true == output[iii].EndWith(split[jjj]+"-"+"bd"+".ttf", false)
			    || true == output[iii].EndWith(split[jjj]+"bold"+".ttf", false)
			    || true == output[iii].EndWith(split[jjj]+"bd"+".ttf", false)
			    || true == output[iii].EndWith(split[jjj]+"b"+".ttf", false)) {
				EWOL_INFO(" find Font [Bold]        : " << output[iii]);
				m_fileName[ewol::font::Bold] = output[iii];
				hasFindAFont=true;
			} else if(    true == output[iii].EndWith(split[jjj]+"-"+"oblique"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"italic"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"Light"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"i"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"oblique"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"italic"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"light"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"i"+".ttf", false)) {
				EWOL_INFO(" find Font [Italic]      : " << output[iii]);
				m_fileName[ewol::font::Italic] = output[iii];
				hasFindAFont=true;
			} else if(    true == output[iii].EndWith(split[jjj]+"-"+"bolditalic"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"boldoblique"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"bi"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"z"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"bolditalic"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"boldoblique"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"bi"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"z"+".ttf", false)) {
				EWOL_INFO(" find Font [Bold-Italic] : " << output[iii]);
				m_fileName[ewol::font::BoldItalic] = output[iii];
				hasFindAFont=true;
			} else if(    true == output[iii].EndWith(split[jjj]+"-"+"regular"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"-"+"r"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"regular"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+"r"+".ttf", false)
			           || true == output[iii].EndWith(split[jjj]+".ttf", false)) {
				EWOL_INFO(" find Font [Regular]     : " << output[iii]);
				m_fileName[ewol::font::Regular] = output[iii];
				hasFindAFont=true;
			}
		}
		if (hasFindAFont==true) {
			EWOL_INFO("    Find this font : '" << split[jjj] << "'");
			break;
		} else if (jjj==split.Size()-1) {
			EWOL_ERROR("Find NO font in the LIST ... " << split);
		}
	}
	
	// try to find the reference mode :
	ewol::font::mode_te refMode = ewol::font::Regular;
	for(int32_t iii=3; iii>=0; iii--) {
		if (m_fileName[iii].IsEmpty()==false) {
			refMode = (ewol::font::mode_te)iii;
		}
	}
	
	EWOL_DEBUG("         Set reference mode : " << refMode);
	// generate the wrapping on the preventing error
	for(int32_t iii=3; iii>=0; iii--) {
		if (m_fileName[iii].IsEmpty()==false) {
			m_modeWraping[iii] = (ewol::font::mode_te)iii;
		} else {
			m_modeWraping[iii] = refMode;
		}
	}
	
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (m_fileName[iiiFontId].IsEmpty()==true) {
			EWOL_DEBUG("can not load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\" ==> size=" << m_size );
			m_font[iiiFontId] = NULL;
			continue;
		}
		EWOL_INFO("Load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\" ==> size=" << m_size);
		ewol::resource::Keep(m_fileName[iiiFontId], m_font[iiiFontId]);
		if (NULL == m_font[iiiFontId]) {
			return;
		}
		
		// set the bassic charset:
		m_listElement[iiiFontId].Clear();
		GlyphProperty tmpchar1;
		tmpchar1.m_UVal = 0;
		m_listElement[iiiFontId].PushBack(tmpchar1);
		for (int32_t iii=0x20; iii<0xFF; iii++) {
			GlyphProperty tmpchar;
			tmpchar.m_UVal = iii;
			m_listElement[iiiFontId].PushBack(tmpchar);
			if (0x7F == iii) {
				iii = 0x9F;
			}
		}
		
		/* this is a bad code for now ... */
		// ==> determine the texture Size
		GlyphProperty tmpproperty;
		tmpproperty.m_UVal = 'A';
		m_font[iiiFontId]->GetGlyphProperty(m_size, tmpproperty);
		
		int32_t nbElement = 0xFF - 0x20 + 1;
		int32_t coter = simpleSQRT(nbElement);
		// note : +1 is for the overlapping of the glyph (Part 1)
		int32_t glyphMaxWidth = tmpproperty.m_advance.x() +1;
		int32_t glyphMaxHeight = tmpproperty.m_advance.y() +1;
		int32_t textureWidth = nextP2(coter*glyphMaxWidth);
		int32_t nbRaws = textureWidth / glyphMaxWidth;
		if (nbRaws <= 0) {
			EWOL_ERROR("devide by 0");
			nbRaws = 1;
		}
		int32_t nbLine = (nbElement / nbRaws) + 1;
		int32_t textureHeight = nextP2(nbLine*glyphMaxHeight);
		// for android : 
		textureHeight = etk_max(textureHeight, textureWidth);
		textureWidth = textureHeight;
		
		if (iiiFontId == 0) {
			EWOL_DEBUG("Generate a text texture for char(" << nbRaws << "," << nbLine << ") with size=(" << textureWidth << "," << textureHeight << ")");
			// resize must be done on the texture ...
			SetImageSize(ivec2(textureWidth,textureHeight));
			// now we can acces directly on the image
			m_data.Clear(etk::Color<>(0x00000000));
		}
		m_height[iiiFontId] = m_font[iiiFontId]->GetHeight(m_size);
		
		int32_t CurrentLineHigh = 0;
		ivec2    glyphPosition(1,1);
		for (int32_t iii=0; iii<m_listElement[iiiFontId].Size(); iii++) {
			if (true == m_font[iiiFontId]->GetGlyphProperty(m_size, (m_listElement[iiiFontId])[iii])) {
				// change line if needed ...
				if (glyphPosition.x()+(m_listElement[iiiFontId])[iii].m_sizeTexture.x() > textureWidth) {
					glyphPosition.setX(0);
					glyphPosition.setY(glyphPosition.y()+CurrentLineHigh);
					CurrentLineHigh = 0;
				}
				// draw the glyph
				m_font[iiiFontId]->DrawGlyph(m_data, m_size, glyphPosition, (m_listElement[iiiFontId])[iii], iiiFontId);
				// set video position
				(m_listElement[iiiFontId])[iii].m_texturePosStart.setValue( (float)(glyphPosition.x()) / (float)textureWidth,
				                                                            (float)(glyphPosition.y()) / (float)textureHeight );
				(m_listElement[iiiFontId])[iii].m_texturePosStop.setValue( (float)(glyphPosition.x() + (m_listElement[iiiFontId])[iii].m_sizeTexture.x()) / (float)textureWidth,
				                                                           (float)(glyphPosition.y() + (m_listElement[iiiFontId])[iii].m_sizeTexture.y()) / (float)textureHeight );
				
				// update the maximum of the line hight : 
				if (CurrentLineHigh<(m_listElement[iiiFontId])[iii].m_sizeTexture.y()) {
					// note : +1 is for the overlapping of the glyph (Part 2)
					CurrentLineHigh = (m_listElement[iiiFontId])[iii].m_sizeTexture.y()+1;
				}
				// note : +1 is for the overlapping of the glyph (Part 3)
				// update the Bitmap position drawing : 
				glyphPosition.setX(glyphPosition.x() + (m_listElement[iiiFontId])[iii].m_sizeTexture.x()+1);
			}
			
		}
		//m_font[iiiFontId]->Display();
		// generate the kerning for all the characters : 
		m_font[iiiFontId]->GenerateKerning(m_size, m_listElement[iiiFontId]);
	}
	// For testing cheree the box are set)
	#if 0
	draw::Color tlpppp(0xFF,0xFF,0xFF,0x00);
	for(int32_t jjj=0; jjj < textureHeight;jjj++) {
		for(int32_t iii=0; iii < textureWidth; iii++){
			tlpppp = m_data.Get(ivec2(iii, jjj) );
			// set only alpha :
			tlpppp.a = etk_min( tlpppp.a+0x60, 0xFF);
			// real set of color
			m_data.Set(ivec2(iii, jjj), tlpppp );
		}
	}
	#endif
	EWOL_DEBUG("End generation of the Fond bitmap, start adding texture");
	//m_data.DistanceField();
	Flush();
	EWOL_DEBUG("Wrapping properties : ");
	EWOL_DEBUG("    " << ewol::font::Regular << "==>" << GetWrappingMode(ewol::font::Regular));
	EWOL_DEBUG("    " << ewol::font::Italic << "==>" << GetWrappingMode(ewol::font::Italic));
	EWOL_DEBUG("    " << ewol::font::Bold << "==>" << GetWrappingMode(ewol::font::Bold));
	EWOL_DEBUG("    " << ewol::font::BoldItalic << "==>" << GetWrappingMode(ewol::font::BoldItalic));
}

ewol::TexturedFont::~TexturedFont(void)
{
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (NULL!= m_font[iiiFontId]) {
			ewol::resource::Release(m_font[iiiFontId]);
			m_font[iiiFontId] = NULL;
		}
	}
}


bool ewol::TexturedFont::HasName(const etk::UString& fileName)
{
	etk::UString tmpName = m_name;
	tmpName += ":";
	tmpName += m_size;
	EWOL_VERBOSE("S : check : " << fileName << " ?= " << tmpName << " = " << (fileName==tmpName) );
	return (fileName==tmpName);
}


int32_t ewol::TexturedFont::GetIndex(const uniChar_t& charcode, const ewol::font::mode_te displayMode) const
{
	if (charcode < 0x20) {
		return 0;
	} else if (charcode < 0x80) {
		return charcode.Get() - 0x1F;
	} else {
		for (int32_t iii=0x80-0x20; iii < m_listElement[displayMode].Size(); iii++) {
			if (charcode == (m_listElement[displayMode])[iii].m_UVal) {
				return iii;
			}
		}
	}
	return 0;
}


ewol::GlyphProperty* ewol::TexturedFont::GetGlyphPointer(const uniChar_t& _charcode, const ewol::font::mode_te _displayMode)
{
	//EWOL_DEBUG("Get glyph property for mode: " << _displayMode << " ==> wrapping index : " << m_modeWraping[_displayMode]);
	int32_t index = GetIndex(_charcode, _displayMode);
	if(    index <0 
	    || index >=m_listElement[_displayMode].Size() ) {
		EWOL_ERROR(" Try to get glyph index inexistant ... ==> return the index 0 ... id=" << index);
		if (m_listElement[_displayMode].Size()>=0) {
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
