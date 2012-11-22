/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURE_BITMAP_H__
#define __EWOL_TEXTURE_BITMAP_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <draw/Image.h>
#include <etk/os/FSNode.h>

namespace ewol
{
	namespace imageBMP
	{
		bool GenerateImage(etk::UString & inputFile, draw::Image & ouputImage);
	};
};

#endif

