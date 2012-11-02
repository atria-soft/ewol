/**
 *******************************************************************************
 * @file ewol/texture/TextureBMP.cpp
 * @brief ewol Texture Bitmap abstraction (sources)
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

#include <etk/Types.h>
#include <draw/Image.h>
#include <ewol/texture/TextureBMP.h>
#include <etk/os/FSNode.h>

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


#undef __class__
#define __class__	"texture::TextureBMP"

bool ewol::imageBMP::GenerateImage(etk::UString & inputFile, draw::Image & ouputImage)
{
	modeBitmap_te       m_dataMode = BITS_16_R5G6B5;
	int32_t             m_width = 0;
	int32_t             m_height = 0;
	bitmapFileHeader_ts m_FileHeader;
	bitmapInfoHeader_ts m_InfoHeader;

	etk::FSNode fileName(inputFile);
	// Get the fileSize ...
	/*if (fileName.Size() < (int32_t)(sizeof(bitmapFileHeader_ts) + sizeof(bitmapInfoHeader_ts) ) ) {
		EWOL_ERROR("not enought data in the file named=\"" << fileName << "\"");
		return;
	}*/
	if (false==fileName.Exist()) {
		EWOL_ERROR("File does not existed=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	// get the data : 
	if (fileName.FileRead(&m_FileHeader,sizeof(bitmapFileHeader_ts),1) != 1) {
		EWOL_ERROR("error loading file header");
		fileName.FileClose();
		return false;
	}
	if (fileName.FileRead(&m_InfoHeader,sizeof(bitmapInfoHeader_ts),1) != 1) {
		EWOL_ERROR("error loading file header");
		fileName.FileClose();
		return false;
	}
	if(false == fileName.FileSeek(m_FileHeader.bfOffBits, SEEK_SET)) {
		EWOL_ERROR("error with the 'bfOffBits' in the file named=\"" << fileName << "\"");
		fileName.FileClose();
		return false;
	}
	// Check the header error : 
	if (m_FileHeader.bfType != 0x4D42) {
		EWOL_ERROR("the file=\"" << fileName << "\" is not a bitmap file ...");
		fileName.FileClose();
		return false;
	}
	if (m_FileHeader.bfReserved != 0x00000000) {
		EWOL_ERROR("the bfReserved feald is not at 0 ==> not supported format ...");
		fileName.FileClose();
		return false;
	}
	if(    m_InfoHeader.biBitCount == 16
	    && m_InfoHeader.biCompression == 0)
	{
		m_dataMode = BITS_16_X1R5G5B5;
	} else if(    m_InfoHeader.biBitCount == 16
	         && m_InfoHeader.biCompression == 3)
	{
		m_dataMode = BITS_16_R5G6B5;
	} else if(    m_InfoHeader.biBitCount == 24
	           && m_InfoHeader.biCompression == 0)
	{
		m_dataMode = BITS_24_R8G8B8;
	} else if(    m_InfoHeader.biBitCount == 32
	           && m_InfoHeader.biCompression == 3)
	{
		m_dataMode = BITS_32_X8R8G8B8;
	} else if(    m_InfoHeader.biBitCount == 32
	           && m_InfoHeader.biCompression == 0)
	{
		m_dataMode = BITS_32_A8R8G8B8;
	} else {
		EWOL_ERROR("the biBitCount & biCompression fealds are unknow ==> not supported format ...");
		fileName.FileClose();;
		return false;
	}
	m_width = m_InfoHeader.biWidth;
	m_height = m_InfoHeader.biHeight;
	// reallocate the image 
	ouputImage.Resize(etk::Vector2D<int32_t>(m_width,m_height));
	
	uint8_t* m_data = NULL;
	if(0 != m_InfoHeader.biSizeImage)
	{
		m_data=new uint8_t[m_InfoHeader.biSizeImage];
		if (fileName.FileRead(m_data,m_InfoHeader.biSizeImage,1) != 1){
			EWOL_CRITICAL("Can not read the file with the good size...");
		}
	}
	fileName.FileClose();
	
	draw::Color tmpColor(0,0,0,0);
	
	// need now to generate RGBA data ...
	switch(m_dataMode)
	{
		case BITS_16_R5G6B5:
			{
				uint16_t * pointer = (uint16_t*)m_data;
				for(int32_t yyy=0; yyy<m_height; yyy++) {
					for(int32_t xxx=0; xxx<m_width; xxx++) {
						tmpColor.r = (int8_t)((*pointer & 0xF800) >> 8);
						tmpColor.g = (int8_t)((*pointer & 0x07E0) >> 3);
						tmpColor.b = (int8_t)(*pointer << 3);
						tmpColor.a = 0xFF;
						ouputImage.Set(etk::Vector2D<int32_t>(xxx,yyy), tmpColor);
						pointer++;
					}
				}
			}
			break;
		case BITS_16_X1R5G5B5:
			{
				uint16_t * pointer = (uint16_t*)m_data;
				for(int32_t yyy=0; yyy<m_height; yyy++) {
					for(int32_t xxx=0; xxx<m_width; xxx++) {
						tmpColor.r = (int8_t)((*pointer & 0x7C00) >> 7);
						tmpColor.g = (int8_t)((*pointer & 0x03E0) >> 2);
						tmpColor.b = (int8_t)(*pointer << 3);
						tmpColor.a = 0xFF;
						ouputImage.Set(etk::Vector2D<int32_t>(xxx,yyy), tmpColor);
						pointer++;
					}
				}
			}
			break;
		case BITS_24_R8G8B8:
			{
				uint8_t * pointer = m_data;
				for(int32_t yyy=0; yyy<m_height; yyy++) {
					for(int32_t xxx=0; xxx<m_width; xxx++) {
						tmpColor.r = *pointer++;
						tmpColor.g = *pointer++;
						tmpColor.b = *pointer++;
						tmpColor.a = 0xFF;
						ouputImage.Set(etk::Vector2D<int32_t>(xxx,yyy), tmpColor);
					}
				}
			}
			break;
		case BITS_32_X8R8G8B8:
			{
				uint8_t * pointer = m_data;
				for(int32_t yyy=0; yyy<m_height; yyy++) {
					for(int32_t xxx=0; xxx<m_width; xxx++) {
						pointer++;
						tmpColor.r = *pointer++;
						tmpColor.g = *pointer++;
						tmpColor.b = *pointer++;
						tmpColor.a = 0xFF;
						ouputImage.Set(etk::Vector2D<int32_t>(xxx,yyy), tmpColor);
					}
				}
			}
			break;
		case BITS_32_A8R8G8B8:
			{
				uint8_t * pointer = m_data;
				for(int32_t yyy=0; yyy<m_height; yyy++) {
					for(int32_t xxx=0; xxx<m_width; xxx++) {
						tmpColor.r = *pointer++;
						tmpColor.g = *pointer++;
						tmpColor.b = *pointer++;
						tmpColor.a = *pointer++;
						ouputImage.Set(etk::Vector2D<int32_t>(xxx,yyy), tmpColor);
					}
				}
			}
			break;
		default:
			EWOL_DEBUG("        mode = ERROR");
			break;
	}
	if (NULL != m_data) {
		delete(m_data);
		m_data=NULL;
	}
	return true;
}
/*
void ewol::texture::TextureBMP::Display(void)
{
	if (NULL == m_data) {
		EWOL_ERROR("Might loading error of this Bitmap ...");
		return;
	}
	EWOL_DEBUG(" -----------------------------------------------------------");
	if (false) {
		EWOL_DEBUG("Display caracteristic of the bitmap : ");
		EWOL_DEBUG("    Header of file :");
		EWOL_DEBUG("        bfType    =" << m_FileHeader.bfType << "      19778 : must always be set to 'BM' to declare that this is a .bmp-file.");
		EWOL_DEBUG("        bfSize    =" << m_FileHeader.bfSize << "      specifies the size of the file in bytes.");
		EWOL_DEBUG("        bfReserved=" << m_FileHeader.bfReserved << "      must always be set to zero.");
		EWOL_DEBUG("        bfOffBits =" << m_FileHeader.bfOffBits << "      1078 : specifies the offset from the beginning of the file to the bitmap data.");
		EWOL_DEBUG("    info header of file :");
		EWOL_DEBUG("        biSize         =" << m_InfoHeader.biSize << "      specifies the size of the BITMAPINFOHEADER structure, in bytes.");
		EWOL_DEBUG("        biWidth        =" << m_InfoHeader.biWidth << "      specifies the width of the image, in pixels.");
		EWOL_DEBUG("        biHeight       =" << m_InfoHeader.biHeight << "      specifies the height of the image, in pixels.");
		EWOL_DEBUG("        biPlanes       =" << m_InfoHeader.biPlanes << "      specifies the number of planes of the target device, must be set to zero.");
		EWOL_DEBUG("        biBitCount     =" << m_InfoHeader.biBitCount << "      specifies the number of bits per pixel.");
		EWOL_DEBUG("        biCompression  =" << m_InfoHeader.biCompression << "      Specifies the type of compression, usually set to zero (no compression).");
		EWOL_DEBUG("        biSizeImage    =" << m_InfoHeader.biSizeImage << "      specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero.");
		EWOL_DEBUG("        biXPelsPerMeter=" << m_InfoHeader.biXPelsPerMeter << "      specifies the the horizontal pixels per meter on the designated targer device, usually set to zero.");
		EWOL_DEBUG("        biYPelsPerMeter=" << m_InfoHeader.biYPelsPerMeter << "      specifies the the vertical pixels per meter on the designated targer device, usually set to zero.");
		EWOL_DEBUG("        biClrUsed      =" << m_InfoHeader.biClrUsed << "      speglTexImage2Dcifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member.");
		EWOL_DEBUG("        biClrImportant =" << m_InfoHeader.biClrImportant << "      specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.");
	}
	EWOL_DEBUG("Bitmap : " << m_width << "x" << m_height);
	switch(m_dataMode)
	{
		case BITS_16_R5G6B5:
			EWOL_DEBUG("        mode = 16 bits R5G6B5");
			break;
		case BITS_16_X1R5G5B5:
			EWOL_DEBUG("        mode = 16 bits X1R5G5B5");
			break;
		case BITS_24_R8G8B8:
			EWOL_DEBUG("        mode = 24 bits R8G8B8");
			break;
		case BITS_32_X8R8G8B8:
			EWOL_DEBUG("        mode = 32 bits X8R8G8B8");
			break;
		case BITS_32_A8R8G8B8:
			EWOL_DEBUG("        mode = 32 bits A8R8G8B8");
			break;
		default:
			EWOL_DEBUG("        mode = ERROR");
			break;
	}
}
*/
