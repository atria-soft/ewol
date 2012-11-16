/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/Debug.h>
#include <ewol/compositing/Text.h>


/*
			// curent Drawing position
			etk::Vector3D<float> m_position;         //!< the next position to draw the text
			// clipping section
			etk::Vector3D<float> m_clippingPosition;
			etk::Vector3D<float> m_clippingSize;
			bool                 m_clippingEnable;
			// Basic color
			etk::Color           m_color;
*/

ewol::Text::Text(void) :
	m_GLprogram(NULL),
	m_font(NULL)
{
	m_color   = draw::color::black;
	m_colorBg = draw::color::none;
	SetFontProperty(ewol::font::GetDefaultFont(), ewol::font::GetDefaultSize());
	LoadProgram();
}


ewol::Text::Text(etk::UString fontName, int32_t fontSize) :
	m_GLprogram(NULL),
	m_font(NULL)
{
	m_color   = draw::color::black;
	m_colorBg = draw::color::none;
	SetFontProperty(fontName, fontSize);
	LoadProgram();
}


ewol::Text::~Text(void)
{
	
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	ewol::resource::Release(m_GLprogram);
}

void LoadProgram(void)
{
	etk::UString tmpString("DATA:text.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
	}
}

void ewol::Text::Draw(void)
{
	if (m_coord.Size()<=0 || NULL == m_font) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	etk::Matrix4 tmpMatrix = ewol::openGL::GetMatrix();
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID, m_font->GetId());
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// Texture :
	m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
}


void ewol::Text::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}


void ewol::Text::SetPos(etk::Vector3D<float> pos)
{
	
}


void ewol::Text::SetRelPos(etk::Vector3D<float> pos)
{
	
}


void ewol::Text::SetColor(draw::Color color)
{
	
}


void ewol::Text::SetColorBG(draw::Color color)
{
	
}


void ewol::Text::SetClipping(etk::Vector3D<float> pos, etk::Vector3D<float> width)
{
	
}


void ewol::Text::SetClippingMode(bool newMode)
{
	
}


void ewol::Text::SetFontSize(int32_t fontSize)
{
	// get old size
	etk::UString fontName = ewol::font::GetDefaultFont();
	if (NULL != m_font) {
		fontName = m_font->GetName();
	}
	SetFont(fontName, fontSize);
}


void ewol::Text::SetFontName(etk::UString fontName)
{
	// get old size
	int32_t fontSize = ewol::font::GetDefaultSize();
	if (NULL != m_font) {
		fontSize = m_font->GetFontSize();
	}
	SetFont(fontName, fontSize);
}


void ewol::Text::SetFont(etk::UString fontName, int32_t fontSize)
{
	// remove old one
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	etk::UString tmpName = fontName;
	tmpName += ":";
	tmpName += fontSize;
	// link to new One
	if (false == ewol::resource::Keep(tmpName, m_font)) {
		EWOL_ERROR("Can not get font resource");
	}
}


void ewol::Text::SetFontMode(ewol::font::mode_te mode)
{
	
}


void ewol::Text::SetKerningMode(bool newMode)
{
	
}


void ewol::Text::SetDistanceFieldMode(bool newMode)
{
	
}


void ewol::Text::Print(etk::UString& text)
{
	
}


void ewol::Text::PrintDecorated(etk::UString& text)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	int32_t nbElementInTheArray = m_coord.Size();
	int32_t size = 0;
	size = m_font->Draw(textPos, unicodeString, m_coord, m_coordTex, m_displayMode, m_hasClipping, m_clipping, displayMode);
	// set the color ...
	for (int32_t iii=nbElementInTheArray; iii<m_coord.Size(); iii++) {
		m_coordColor.PushBack(m_color);
	}
	return size;
}


void ewol::Text::Print(etk::UString& text, etk::Vector<TextDecoration>& decoration)
{
	
}


void ewol::Text::Print(uniChar_t charcode)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	int32_t nbElementInTheArray = m_coord.Size();
	int32_t size = 0;
	size = m_font->Draw(textPos, unicodeChar, m_coord, m_coordTex, m_displayMode, m_hasClipping, m_clipping, displayMode);
	for (int32_t iii=nbElementInTheArray; iii<m_coord.Size(); iii++) {
		m_coordColor.PushBack(m_color);
	}
	return size;
}


void ewol::Text::SetTextAlignement(float startTextpos, float stopTextPos, aligneMode_te alignement)
{
	
}


void ewol::Text::DisableAlignement(void)
{
	
}

