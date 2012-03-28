/**
 *******************************************************************************
 * @file ewol/Texture/TextureBMP.h
 * @brief ewol Texture Bitmap abstraction (header)
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

#ifndef __EWOL_TEXTURE_BITMAP_H__
#define __EWOL_TEXTURE_BITMAP_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>

namespace ewol
{
	namespace texture {
		#pragma pack(push,1)
		typedef struct
		{
			int16_t bfType;
			int32_t bfSize;
			int32_t bfReserved;
			int32_t bfOffBits;
		} bitmapFileHeader_ts;
		
		typedef struct
		{
			int32_t biSize;
			int32_t biWidth;
			int32_t biHeight;
			int16_t biPlanes;
			int16_t biBitCount;
			int32_t biCompression;
			int32_t biSizeImage;
			int32_t biXPelsPerMeter;
			int32_t biYPelsPerMeter;
			int32_t biClrUsed;
			int32_t biClrImportant;
		} bitmapInfoHeader_ts;
		#pragma pack(pop)
		
		typedef enum {
			BITS_16_R5G6B5,
			BITS_16_X1R5G5B5,
			BITS_24_R8G8B8,
			BITS_32_X8R8G8B8,
			BITS_32_A8R8G8B8
		} modeBitmap_te;
		
		class TextureBMP
		{
			private:
				modeBitmap_te       m_dataMode;
				int32_t             m_width;
				int32_t             m_height;
				int32_t             m_size;
				uint8_t *           m_data;
				uint8_t *           m_dataGenerate;
				bitmapFileHeader_ts m_FileHeader;
				bitmapInfoHeader_ts m_InfoHeader;
			public:
				TextureBMP(etk::File & fileName);
				~TextureBMP(void);
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

