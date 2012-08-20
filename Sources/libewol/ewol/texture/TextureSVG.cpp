/**
 *******************************************************************************
 * @file ewol/texture/TextureSVG.cpp
 * @brief ewol Texture SVG abstraction (Sources)
 * @author Edouard DUPIN
 * @date 28/03/2012
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



#include <ewol/texture/TextureSVG.h>


#undef __class__
#define __class__	"texture::TextureSVG"


ewol::texture::TextureSVG::TextureSVG(etk::File & fileName, int32_t width, int32_t height) : m_elementParsed(fileName)
{
	m_loadOK = false;
	if (false == m_elementParsed.IsLoadOk()) {
		EWOL_ERROR("Error To load SVG file " << fileName.GetCompleateName() );
	} else {
		m_elementParsed.GenerateAnImage(width, height);
		m_loadOK = true;
	}
	if (width == -1) {
		Vector2D<float> elementSize = m_elementParsed.GetDefinedSize();
		m_width = elementSize.x;
		m_height = elementSize.y;
	} else {
		m_width = width;
		m_height = height;
	}
}

ewol::texture::TextureSVG::~TextureSVG(void)
{
	
}

bool ewol::texture::TextureSVG::LoadOK(void)
{
	return m_loadOK;
}

int32_t ewol::texture::TextureSVG::Width(void)
{
	return m_width;
}

int32_t ewol::texture::TextureSVG::Height(void)
{
	return m_height;
}

uint8_t * ewol::texture::TextureSVG::Data(void)
{
	return m_elementParsed.GetPointerOnData();
}

uint32_t ewol::texture::TextureSVG::DataSize(void)
{
	return m_elementParsed.GetSizeOnData();
}

void ewol::texture::TextureSVG::Display(void)
{
	EWOL_DEBUG("SVG Texture : (" << m_width << "," << m_height << ") loadedOK=" << m_loadOK);
}
