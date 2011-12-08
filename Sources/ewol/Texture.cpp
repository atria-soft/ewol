/**
 *******************************************************************************
 * @file ewol/Texture.cpp
 * @brief ewol Texture loading system (sources)
 * @author Edouard DUPIN
 * @date 28/10/2011
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



#include <ewol/Texture.h>
#include <importgl.h>

extern "C"
{
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
};


#undef __class__
#define __class__	"ewol::Bitmap"

class Bitmap
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
		Bitmap(etk::File & fileName) : m_data(NULL), m_dataGenerate(NULL)
		{
			m_dataMode = BITS_16_R5G6B5;
			m_width = 0;
			m_height = 0;
			m_size = 0;

			FILE *File=NULL;
			// Get the fileSize ...
			/*if (fileName.Size() < (int32_t)(sizeof(bitmapFileHeader_ts) + sizeof(bitmapInfoHeader_ts) ) ) {
				EWOL_ERROR("not enought data in the file named=\"" << fileName << "\"");
				return;
			}*/
			File=fopen(fileName.GetCompleateName().c_str(),"rb");
			if(NULL == File) {
				EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
				return;
			}
			// get the data : 
			if (fread(&m_FileHeader,sizeof(bitmapFileHeader_ts),1,File) != 1) {
				EWOL_ERROR("error loading file header");
				fclose(File);
				return;
			}
			if (fread(&m_InfoHeader,sizeof(bitmapInfoHeader_ts),1,File) != 1) {
				EWOL_ERROR("error loading file header");
				fclose(File);
				return;
			}
			// TODO : do otherwise ...
			fseek(File,m_FileHeader.bfOffBits,SEEK_SET);
			if(ferror(File)) {
				EWOL_ERROR("error with the 'bfOffBits' in the file named=\"" << fileName << "\"");
				fclose(File);
				return;
			}
			// Check the header error : 
			if (m_FileHeader.bfType != 0x4D42) {
				EWOL_ERROR("the file=\"" << fileName << "\" is not a bitmap file ...");
				fclose(File);
				return;
			}
			if (m_FileHeader.bfReserved != 0x00000000) {
				EWOL_ERROR("the bfReserved feald is not at 0 ==> not supported format ...");
				fclose(File);
				return;
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
				fclose(File);
				return;
			}
			m_width = m_InfoHeader.biWidth;
			m_height = m_InfoHeader.biHeight;
			
			if(0 != m_InfoHeader.biSizeImage)
			{
				m_data=new uint8_t[m_InfoHeader.biSizeImage];
				if (fread(m_data,m_InfoHeader.biSizeImage,1,File) != 1){
					EWOL_CRITICAL("Can not read the file with the good size...");
				}
				// allocate the destination data ...
				m_dataGenerate=new uint8_t[m_width*m_height*4];
			}
			fclose(File);
			// need now to generate RGBA data ...
			switch(m_dataMode)
			{
				case BITS_16_R5G6B5:
					{
						uint16_t * pointer = (uint16_t*)m_data;
						for(int32_t yyy=0; yyy<m_height; yyy++) {
							for(int32_t xxx=0; xxx<m_width; xxx++) {
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 0] = (int8_t)((*pointer & 0xF800) >> 8);
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 1] = (int8_t)((*pointer & 0x07E0) >> 3);
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 2] = (int8_t)(*pointer << 3);
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 3] = 0xFF;
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
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 0] = (int8_t)((*pointer & 0x7C00) >> 7);
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 1] = (int8_t)((*pointer & 0x03E0) >> 2);
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 2] = (int8_t)(*pointer << 3);
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 3] = 0xFF;
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
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 0] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 1] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 2] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 3] = 0xFF;
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
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 0] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 1] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 2] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 3] = 0xFF;
							}
						}
					}
					break;
				case BITS_32_A8R8G8B8:
					{
						uint8_t * pointer = m_data;
						for(int32_t yyy=0; yyy<m_height; yyy++) {
							for(int32_t xxx=0; xxx<m_width; xxx++) {
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 0] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 1] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 2] = *pointer++;
								m_dataGenerate[4*((m_height-yyy-1) * m_width + xxx ) + 3] = *pointer++;
							}
						}
					}
					break;
				default:
					EWOL_DEBUG("        mode = ERROR");
					break;
			}
		}
		
		~Bitmap(void)
		{
			if (NULL != m_data) {
				delete(m_data);
			}
			if (NULL != m_dataGenerate) {
				delete(m_dataGenerate);
			}
		}
		
		bool LoadOK(void) { if (NULL != m_dataGenerate) { return true; } else { return false; } };
		
		int32_t Width(void)  { return m_width; };
		int32_t Height(void) { return m_height; };
		uint8_t * Data(void)   { return m_dataGenerate; };
		
		void Display(void)
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
};



class LoadedTexture
{
	public:
		etk::File m_filename;
		int32_t   m_nbTimeLoaded;
		int32_t   m_imageSize; // must be x=y ...
		uint32_t  m_openGlTextureID;
};

etk::VectorType<LoadedTexture*> listLoadedTexture;


#undef __class__
#define __class__	"ewol"

int32_t ewol::LoadTexture(etk::File fileName)
{
	if (listLoadedTexture.Size()!=0) {
		for (int32_t iii=0; iii<listLoadedTexture.Size(); iii++) {
			if (listLoadedTexture[iii]->m_filename == fileName) {
				listLoadedTexture[iii]->m_nbTimeLoaded++;
				return listLoadedTexture[iii]->m_openGlTextureID;
			}
		}
	}
	etk::String fileExtention = fileName.GetExtention();
	if (fileExtention ==  "bmp") {
		if (false == fileName.Exist()) {
			EWOL_ERROR("File does not Exist ... " << fileName);
			return -1;
		}
		Bitmap myBitmap(fileName);
		myBitmap.Display();
		if (myBitmap.LoadOK() == true) {
			if (myBitmap.Width()!= myBitmap.Height()) {
				EWOL_ERROR("Texture can not have Width=" << myBitmap.Width() << "px different of height=" << myBitmap.Height() << "px in file:" << fileName);
				return -1;
			}
			GLuint textureid;
			glGenTextures(1, &textureid);
			glBindTexture(GL_TEXTURE_2D, textureid);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//--- mode nearest
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//--- Mode linear
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myBitmap.Width(), myBitmap.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myBitmap.Data());
			LoadedTexture *tmpTex = new LoadedTexture();
			if (NULL != tmpTex) {
				tmpTex->m_filename = fileName;
				tmpTex->m_nbTimeLoaded = 1;
				tmpTex->m_imageSize = myBitmap.Width();
				tmpTex->m_openGlTextureID = textureid;
				listLoadedTexture.PushBack(tmpTex);
			} else {
				EWOL_ERROR("Allocation ERROR... ");
			}
			return textureid;
		} else {
			return -1;
		}
	} else {
		EWOL_ERROR("Extention not managed " << fileName << " Sopported extention : .bmp");
		return -1;
	}
}

void ewol::UnLoadTexture(uint32_t textureID)
{
	for (int32_t iii=0; iii<listLoadedTexture.Size(); iii++) {
		if (listLoadedTexture[iii]->m_openGlTextureID == textureID) {
			listLoadedTexture[iii]->m_nbTimeLoaded--;
			if (0 == listLoadedTexture[iii]->m_nbTimeLoaded) {
				EWOL_DEBUG("Remove openGL texture ID=" << textureID << " file:" << listLoadedTexture[iii]->m_filename);
				glDeleteTextures(1,&listLoadedTexture[iii]->m_openGlTextureID);
				delete(listLoadedTexture[iii]);
				listLoadedTexture[iii] = NULL;
				listLoadedTexture.Erase(iii);
			}
			return;
		}
	}
	EWOL_CRITICAL("Can not find TextureId=" << (int)textureID << " in the list of texture loaded...==> to remove it ...");
}


int32_t ewol::GetTextureSize(uint32_t textureID)
{
	for (int32_t iii=0; iii<listLoadedTexture.Size(); iii++) {
		if (listLoadedTexture[iii]->m_openGlTextureID == textureID) {
			return listLoadedTexture[iii]->m_imageSize;
		}
	}
	EWOL_ERROR("Can not find TextureId=" << textureID << " in the list of texture loaded...");
	return -1;
}