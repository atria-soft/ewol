/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/types.h>
#include <etk/os/FSNode.h>

#include <parserSVG/parserSVG.h>

#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/resources/Image.h>
#include <ewol/renderer/resources/Texture.h>


#include <ewol/renderer/resources/image/ImageBMP.h>
#include <ewol/renderer/resources/image/ImagePNG.h>


ewol::TextureFile::TextureFile(etk::UString genName, etk::UString tmpfileName, ivec2 size) :
	Texture(genName)
{
	etk::UString tmpName = tmpfileName;
	// get the upper paw2 ot the size requested...
	if (size.x()>0 && size.y()>0) {
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
		// generate the texture
		if (false == ewol::imagePNG::GenerateImage(tmpName, m_data)) {
			EWOL_ERROR("Error To load PNG file " << tmpName );
		}
	} else {
		EWOL_ERROR("Extention not managed " << tmpName << " Sopported extention : .bmp / .svg / .png");
	}
	Flush();
}

