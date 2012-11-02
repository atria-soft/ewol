/**
 *******************************************************************************
 * @file ewol/texture/TextureFile.cpp
 * @brief ewol tecture file (sources)
 * @author Edouard DUPIN
 * @date 22/08/2012
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

#include <ewol/ResourceManager.h>
#include <ewol/texture/TextureFile.h>
#include <ewol/texture/Texture.h>

#include <etk/os/FSNode.h>

#include <ewol/texture/TextureBMP.h>
#include <parserSVG/parserSVG.h>
//#include <ewol/texture/TexturePNG.h>


ewol::TextureFile::TextureFile(etk::UString genName, etk::UString tmpfileName, etk::Vector2D<int32_t> size) :
	Texture(genName)
{
	etk::UString tmpName = etk::UString("DATA:") + tmpfileName;
	// get the upper paw2 ot the size requested...
	if (size.x>0 && size.y>0) {
		SetImageSize(size);
	}
	// load data
	if (true == tmpName.EndWith(".bmp") ) {
		// generate the texture
		if (false == ewol::imageBMP::GenerateImage(tmpName, m_data)) {
			EWOL_ERROR("Error To load BMP file " << tmpName );
		}
	} else if (true == tmpName.EndWith(".svg") ) {
		svg::Parser m_element(tmpName);
		if (false == m_element.IsLoadOk()) {
			EWOL_ERROR("Error To load SVG file " << tmpName );
		} else {
			// generate the texture
			m_element.GenerateAnImage(size, m_data);
		}
	} else if (true == tmpName.EndWith(".png") ) {
		EWOL_ERROR("Extention not supported now, but soon " << tmpName );
	} else {
		EWOL_ERROR("Extention not managed " << tmpName << " Sopported extention : .bmp / .svg / .png");
	}
	Flush();
}

