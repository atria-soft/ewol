/**
 *******************************************************************************
 * @file ewol/Texture/TextureSVG.h
 * @brief ewol Texture SVG abstraction (header)
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

#ifndef __EWOL_TEXTURE_SVG_H__
#define __EWOL_TEXTURE_SVG_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/os/File.h>
#include <parserSVG/parserSVG.h>

namespace ewol
{
	namespace texture {
		class TextureSVG
		{
			private:
				svg::Parser         m_elementParsed;
				int32_t             m_width;
				int32_t             m_height;
				int32_t             m_size;
				uint8_t *           m_data;
				bool                m_loadOK;
			public:
				TextureSVG(etk::File & fileName, int32_t width, int32_t height);
				~TextureSVG(void);
				bool LoadOK(void);
				int32_t Width(void);
				int32_t Height(void);
				uint8_t * Data(void);
				uint32_t DataSize(void);
				void Display(void);
		};
	};
};

#endif

