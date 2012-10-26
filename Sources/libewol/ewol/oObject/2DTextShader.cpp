/**
 *******************************************************************************
 * @file ewol/oObject/2DTextColored.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 16/01/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
#ifdef __VIDEO__OPENGL_ES_2

#include <ewol/oObject/2DTextShader.h>
#include <ewol/openGL/openGL.h>
#include <ewol/texture/Texture.h>
#include <ewol/font/FontManager.h>
#include <ewol/ResourceManager.h>

#undef __class__
#define __class__	"ewol::OObject2DTextShader"

float DF_SoftEdge_min = 0.45;
float DF_SoftEdge_max = 0.55;
int32_t DF_SoftEdge = 0;



void ewol::OObject2DTextShader::SetFontProperty(etk::UString fontName, int32_t fontSize)
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

void ewol::OObject2DTextShader::SetFont(etk::UString fontName)
{
	// get old size
	int32_t fontSize = ewol::font::GetDefaultSize();
	if (NULL != m_font) {
		fontSize = m_font->GetFontSize();
	}
	SetFontProperty(fontName, fontSize);
}

void ewol::OObject2DTextShader::SetSize(int32_t fontSize)
{
	// get old size
	etk::UString fontName = ewol::font::GetDefaultFont();
	if (NULL != m_font) {
		fontName = m_font->GetName();
	}
	SetFontProperty(fontName, fontSize);
}

ewol::OObject2DTextShader::OObject2DTextShader(etk::UString fontName, int32_t size) :
	m_font(NULL)
{
	m_color = draw::color::black;
	SetFontProperty(fontName, size);
	etk::UString tmpString("fontDistanceField/font1.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
		m_GLSoftEdgeMin = m_GLprogram->GetUniform("EW_SoftEdgeMin");
		m_GLSoftEdgeMax = m_GLprogram->GetUniform("EW_SoftEdgeMax");
		m_GLSoftEdge    = m_GLprogram->GetUniform("EW_SoftEdge");
	}
}


// open with default font ...
ewol::OObject2DTextShader::OObject2DTextShader(void) :
	m_font(NULL)
{
	m_color = draw::color::black;
	SetFontProperty(ewol::font::GetDefaultFont(), ewol::font::GetDefaultSize());
	etk::UString tmpString("fontDistanceField/font1.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
		m_GLSoftEdgeMin = m_GLprogram->GetUniform("EW_SoftEdgeMin");
		m_GLSoftEdgeMax = m_GLprogram->GetUniform("EW_SoftEdgeMax");
		m_GLSoftEdge    = m_GLprogram->GetUniform("EW_SoftEdge");
	}
}


ewol::OObject2DTextShader::~OObject2DTextShader(void)
{
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	ewol::resource::Release(m_GLprogram);
}

void ewol::OObject2DTextShader::Draw(void)
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
	// set some other specific properties :
	m_GLprogram->Uniform1f(m_GLSoftEdgeMin, DF_SoftEdge_min);
	m_GLprogram->Uniform1f(m_GLSoftEdgeMax, DF_SoftEdge_max);
	m_GLprogram->Uniform1i(m_GLSoftEdge,    DF_SoftEdge);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
}

void ewol::OObject2DTextShader::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}

int32_t ewol::OObject2DTextShader::Text(etk::Vector2D<float> textPos, const etk::UString& unicodeString)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	int32_t nbElementInTheArray = m_coord.Size();
	int32_t size = 0;
	if (true==m_hasClipping) {
		size = m_font->Draw(textPos, unicodeString, m_coord, m_coordTex);
	} else {
		size = m_font->Draw(textPos, unicodeString, m_coord, m_coordTex);
	}
	// set the color ...
	for (int32_t iii=nbElementInTheArray; iii<m_coord.Size(); iii++) {
		m_coordColor.PushBack(m_color);
	}
	return size;
}

int32_t ewol::OObject2DTextShader::Text(etk::Vector2D<float> textPos, const uniChar_t unicodeChar)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	int32_t nbElementInTheArray = m_coord.Size();
	int32_t size = 0;
	if (true==m_hasClipping) {
		size = m_font->Draw(textPos, unicodeChar, m_coord, m_coordTex);
	} else {
		size = m_font->Draw(textPos, unicodeChar, m_coord, m_coordTex);
	}
	for (int32_t iii=nbElementInTheArray; iii<m_coord.Size(); iii++) {
		m_coordColor.PushBack(m_color);
	}
	return size;
}


void ewol::OObject2DTextShader::SetColor(draw::Color color)
{
	m_color = color;
}


void ewol::OObject2DTextShader::SetColor(float red, float green, float blue, float alpha)
{
	m_color = draw::Color(red, green, blue, alpha);
}

etk::Vector2D<float> ewol::OObject2DTextShader::GetSize(const uniChar_t unicodeChar)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return etk::Vector2D<float>(0,0);
	}
	return m_font->GetSize(unicodeChar);
}

etk::Vector2D<float> ewol::OObject2DTextShader::GetSize(const etk::UString& unicodeString)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return etk::Vector2D<float>(0,0);
	}
	return m_font->GetSize(unicodeString);
}

#endif
