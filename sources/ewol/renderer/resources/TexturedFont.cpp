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
	
	m_lastGlyphPos[0].x = 0;
	m_lastGlyphPos[0].y = 0;
	m_lastGlyphPos[1].x = 0;
	m_lastGlyphPos[1].y = 0;
	m_lastGlyphPos[2].x = 0;
	m_lastGlyphPos[2].y = 0;
	m_lastGlyphPos[3].x = 0;
	m_lastGlyphPos[3].y = 0;
	
	m_lastRawHeigh[0] = 0;
	m_lastRawHeigh[1] = 0;
	m_lastRawHeigh[2] = 0;
	m_lastRawHeigh[3] = 0;
	
	int32_t tmpSize = 0;
	// extarct name and size :
	char * tmpData = fontName.c_str();
	char * tmpPos = strchr(tmpData, ':');
	
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
	
	// find the real Font name :
	etk::Vector<etk::UString> output;
	#ifdef __EWOL_INTEGRATED_FONT__
		etk::FSNode myFolder("DATA:fonts");
	#else
		#if defined(__TARGET_OS__Android)
			etk::FSNode myFolder("/system/font"); 
		#elif defined(__TARGET_OS__Linux)
			etk::FSNode myFolder("/usr/share/fonts/truetype");
		#endif
	#endif
	myFolder.FolderGetRecursiveFiles(output);
	for (int32_t iii=0; iii<output.Size(); iii++) {
		//EWOL_DEBUG(" file : " << output[iii]);
		if(    true == output[iii].EndWith(m_name+"-"+"bold"+".ttf", false)
		    || true == output[iii].EndWith(m_name+"-"+"b"+".ttf", false)
		    || true == output[iii].EndWith(m_name+"-"+"bd"+".ttf", false)
		    || true == output[iii].EndWith(m_name+"bold"+".ttf", false)
		    || true == output[iii].EndWith(m_name+"bd"+".ttf", false)
		    || true == output[iii].EndWith(m_name+"b"+".ttf", false)) {
			EWOL_INFO(" find Font [Bold]        : " << output[iii]);
			m_fileName[ewol::font::Bold] = output[iii];
		} else if(    true == output[iii].EndWith(m_name+"-"+"oblique"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"italic"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"Light"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"i"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"oblique"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"italic"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"light"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"i"+".ttf", false)) {
			EWOL_INFO(" find Font [Italic]      : " << output[iii]);
			m_fileName[ewol::font::Italic] = output[iii];
		} else if(    true == output[iii].EndWith(m_name+"-"+"bolditalic"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"boldoblique"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"bi"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"z"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"bolditalic"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"boldoblique"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"bi"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"z"+".ttf", false)) {
			EWOL_INFO(" find Font [Bold-Italic] : " << output[iii]);
			m_fileName[ewol::font::BoldItalic] = output[iii];
		} else if(    true == output[iii].EndWith(m_name+"-"+"regular"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"-"+"r"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"regular"+".ttf", false)
		           || true == output[iii].EndWith(m_name+"r"+".ttf", false)
		           || true == output[iii].EndWith(m_name+".ttf", false)) {
			EWOL_INFO(" find Font [Regular]     : " << output[iii]);
			m_fileName[ewol::font::Regular] = output[iii];
		}
	}
	// try to find the reference mode :
	ewol::font::mode_te refMode = ewol::font::Regular;
	for(int32_t iii=3; iii>=0; iii--) {
		if (m_fileName[iii] != "") {
			refMode = (ewol::font::mode_te)iii;
		}
	}
	// generate the wrapping on the preventing error
	for(int32_t iii=3; iii>=0; iii--) {
		if (m_fileName[iii] != "") {
			m_modeWraping[iii] = (ewol::font::mode_te)iii;
		} else {
			m_modeWraping[iii] = refMode;
		}
	}
	
	for (int32_t iiiFontId=0; iiiFontId<4 ; iiiFontId++) {
		if (m_fileName[iiiFontId] == "") {
			EWOL_CRITICAL("can not load FONT [" << iiiFontId << "] name : \"" << m_fileName[iiiFontId] << "\" ==> size=" << m_size );
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
		int32_t glyphMaxWidth = tmpproperty.m_advance.x +1;
		int32_t glyphMaxHeight = tmpproperty.m_advance.y +1;
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
			SetImageSize(etk::Vector2D<int32_t>(textureWidth,textureHeight));
			// now we can acces directly on the image
			m_data.SetFillColor(draw::Color(0x00000000));
			m_data.Clear();
		}
		m_height[iiiFontId] = m_font[iiiFontId]->GetHeight(m_size);
		
		int32_t CurrentLineHigh = 0;
		etk::Vector2D<int32_t>    glyphPosition(1,1);
		for (int32_t iii=0; iii<m_listElement[iiiFontId].Size(); iii++) {
			if (true == m_font[iiiFontId]->GetGlyphProperty(m_size, (m_listElement[iiiFontId])[iii])) {
				// change line if needed ...
				if (glyphPosition.x+(m_listElement[iiiFontId])[iii].m_sizeTexture.x > textureWidth) {
					glyphPosition.x = 0;
					glyphPosition.y += CurrentLineHigh;
					CurrentLineHigh = 0;
				}
				// draw the glyph
				m_font[iiiFontId]->DrawGlyph(m_data, m_size, glyphPosition, (m_listElement[iiiFontId])[iii], iiiFontId);
				// set video position
				(m_listElement[iiiFontId])[iii].m_texturePosStart.x = (float)(glyphPosition.x) / (float)textureWidth;
				(m_listElement[iiiFontId])[iii].m_texturePosStart.y = (float)(glyphPosition.y) / (float)textureHeight;
				(m_listElement[iiiFontId])[iii].m_texturePosStop.x  = (float)(glyphPosition.x + (m_listElement[iiiFontId])[iii].m_sizeTexture.x) / (float)textureWidth;
				(m_listElement[iiiFontId])[iii].m_texturePosStop.y  = (float)(glyphPosition.y + (m_listElement[iiiFontId])[iii].m_sizeTexture.y) / (float)textureHeight;
				
				// update the maximum of the line hight : 
				if (CurrentLineHigh<(m_listElement[iiiFontId])[iii].m_sizeTexture.y) {
					// note : +1 is for the overlapping of the glyph (Part 2)
					CurrentLineHigh = (m_listElement[iiiFontId])[iii].m_sizeTexture.y+1;
				}
				// note : +1 is for the overlapping of the glyph (Part 3)
				// update the Bitmap position drawing : 
				glyphPosition.x += (m_listElement[iiiFontId])[iii].m_sizeTexture.x+1;
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
			tlpppp = m_data.Get(etk::Vector2D<int32_t>(iii, jjj) );
			// set only alpha :
			tlpppp.a = etk_min( tlpppp.a+0x60, 0xFF);
			// real set of color
			m_data.Set(etk::Vector2D<int32_t>(iii, jjj), tlpppp );
		}
	}
	#endif
	EWOL_DEBUG("End generation of the Fond bitmap, start adding texture");
	//m_data.DistanceField();
	Flush();
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


bool ewol::TexturedFont::HasName(etk::UString& fileName)
{
	etk::UString tmpName = m_name;
	tmpName += ":";
	tmpName += m_size;
	EWOL_VERBOSE("S : check : " << fileName << " ?= " << tmpName << " = " << (fileName==tmpName) );
	return (fileName==tmpName);
}


int32_t ewol::TexturedFont::GetIndex(const uniChar_t charcode, const ewol::font::mode_te displayMode) const
{
	if (charcode < 0x20) {
		return 0;
	} else if (charcode < 0x80) {
		return charcode - 0x1F;
	} else {
		for (int32_t iii=0x80-0x20; iii < m_listElement[displayMode].Size(); iii++) {
			if ((m_listElement[displayMode])[iii].m_UVal == charcode) {
				return iii;
			}
		}
	}
	return 0;
}


ewol::GlyphProperty* ewol::TexturedFont::GetGlyphPointer(const uniChar_t charcode, const ewol::font::mode_te displayMode)
{
	int32_t index = GetIndex(charcode, displayMode);
	return &((m_listElement[displayMode])[index]);
}
