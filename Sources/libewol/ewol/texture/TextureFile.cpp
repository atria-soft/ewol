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

#include <ewol/texture/TextureManager.h>
#include <ewol/texture/TextureFile.h>
#include <ewol/texture/Texture.h>


#include <ewol/texture/TextureBMP.h>
#include <parserSVG/parserSVG.h>
//#include <ewol/texture/TexturePNG.h>


ewol::TextureFile::TextureFile(etk::UString tmpfileName, Vector2D<int32_t> size) :
	m_counter(1),
	m_fileName(tmpfileName)
{
	etk::File fileName(tmpfileName, etk::FILE_TYPE_DATA);
	if (false == fileName.Exist()) {
		EWOL_ERROR("File does not Exist ... " << fileName);
	} else {
		// get the upper paw2 ot the size requested...
		if (size.x>0 && size.y>0) {
			m_texture.SetImageSize(size);
		}
		
		etk::UString fileExtention = fileName.GetExtention();
		if (fileExtention == "bmp") {
			// generate the texture
			ewol::imageBMP::GenerateImage(fileName, m_texture.Get());
		} else if (fileExtention == "svg") {
			svg::Parser m_element(fileName);
			if (false == m_element.IsLoadOk()) {
				EWOL_ERROR("Error To load SVG file " << fileName.GetCompleateName() );
			} else {
				m_element.GenerateAnImage(size, m_texture.Get());
			}
		} else if (fileExtention ==  "png") {
			EWOL_ERROR("Extention not supported now, but soon " << fileName );
		} else {
			EWOL_ERROR("Extention not managed " << fileName << " Sopported extention : .bmp / .svg / .png");
		}
		m_texture.Flush();
	}
}


ewol::TextureFile::~TextureFile(void)
{
	
}

