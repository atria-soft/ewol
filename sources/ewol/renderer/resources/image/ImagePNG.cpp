/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <draw/Image.h>
#include <ewol/renderer/resources/image/ImagePNG.h>
#include <etk/os/FSNode.h>
#include <png/png.h>


#undef __class__
#define __class__	"texture::TexturePNG"

// we must change the access of the IO of the png lib :
static void local_ReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	etk::FSNode* fileNode = static_cast<etk::FSNode*>(png_get_io_ptr(png_ptr));
	if (NULL!=fileNode) {
		fileNode->FileRead(data, 1, length);
	}
}
/*
static void LocalWriteData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	etk::FSNode* fileNode = static_cast<etk::FSNode*>(png_get_io_ptr(png_ptr));
	if (NULL!=fileNode) {
		fileNode->FileWrite(data, 1, length);
	}
}

static void localFlushData(png_structp png_ptr)
{
	etk::FSNode* fileNode = static_cast<etk::FSNode*>(png_get_io_ptr(png_ptr));
	if (NULL!=fileNode) {
		fileNode->FileFlush();
	}
}
*/

bool ewol::imagePNG::GenerateImage(etk::UString & inputFile, draw::Image & ouputImage)
{
	etk::FSNode fileName(inputFile);
	
	if (false==fileName.Exist()) {
		EWOL_ERROR("File does not existed=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	
	// Vars
	int x, y = 0;
	int rowbytes;
	unsigned char header[8];
	png_infop info_ptr;
	png_structp png_ptr;
	png_bytep * row_pointers;
	
	if (fileName.FileRead(header,1,8) != 8) {
		EWOL_ERROR("error loading file header");
		fileName.FileClose();
		return false;
	}
	if (png_sig_cmp(header, 0, 8))
	{
		EWOL_ERROR("Invalid file :" << fileName);
		return false;
	}
	
	// PNG read setup
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);
	setjmp(png_jmpbuf(png_ptr));
	// overwrite the read and write function :
	#if 1
		png_set_read_fn(png_ptr,
		                &fileName,
		                &local_ReadData);
		/*
		png_set_write_fn(png_ptr,
		                 &fileName,
		                 &LocalWriteData,
		                 &localFlushData);
		*/
	#else
		png_init_io(png_ptr, fp);
	#endif
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	int32_t width = png_get_image_width(png_ptr, info_ptr);
	int32_t height = png_get_image_height(png_ptr, info_ptr);
	// reallocate the image 
	EWOL_DEBUG("Load PNG image : (" << width << "," << height << ")" );
	ouputImage.Resize(ivec2(width,height));
	
	int32_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	png_read_update_info(png_ptr, info_ptr);
	setjmp(png_jmpbuf(png_ptr));
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	rowbytes = width * ((bit_depth == 16) ? 8 : 4);

	// File read
	for (y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte*) malloc(rowbytes);
	}
	png_read_image(png_ptr, row_pointers);
	png_set_expand(png_ptr);
	png_set_strip_16(png_ptr);
	
	draw::Color tmpColor(0,0,0,0);
	switch (png_get_color_type(png_ptr, info_ptr) )
	{
		case PNG_COLOR_TYPE_RGBA:
			// Conversion to OpenGL texture
			for (y = 0; y < height; y++)
			{
				png_byte* row = row_pointers[y];
				for (x = 0; x < width; x++)
				{
					png_byte* ptr = &(row[x*4]);
					tmpColor.r = ptr[0];
					tmpColor.g = ptr[1];
					tmpColor.b = ptr[2];
					tmpColor.a = ptr[3];
					ouputImage.Set(ivec2(x,y), tmpColor);
				}
				delete row_pointers[y];
			}
			break;
		case PNG_COLOR_TYPE_RGB:
			// Conversion to OpenGL texture
			for (y = 0; y < height; y++)
			{
				png_byte* row = row_pointers[y];
				for (x = 0; x < width; x++)
				{
					png_byte* ptr = &(row[x*3]);
					tmpColor.r = ptr[0];
					tmpColor.g = ptr[1];
					tmpColor.b = ptr[2];
					tmpColor.a = 0xFF;
					ouputImage.Set(ivec2(x,y), tmpColor);
				}
				delete row_pointers[y];
			}
			break;
		default:
			EWOL_ERROR("Must be RGBA/RGB");
			return false;
	}

	fileName.FileClose();
	return true;
}
