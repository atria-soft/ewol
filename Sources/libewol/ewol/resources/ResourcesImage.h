/**
 *******************************************************************************
 * @file ewol/resources/ResourcesImage.h
 * @brief ewol Resources image system (header)
 * @author Edouard DUPIN
 * @date 21/08/2012
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

#ifndef __EWOL_RESOURCES_IMAGE_H__
#define __EWOL_RESOURCES_IMAGE_H__

#include <etk/UString.h>
#include <draw/Image.h>

namespace ewol
{
	class ResourcesImage
	{
		private:
			uint32_t     m_resourceID;
			uint32_t     m_counter;
			draw::Image  m_image;
			etk::UString m_fileName;
		public:
			ResourcesImage(etk::UString fileName, Vector2D<int32_t> size);
			~ResourcesImage(void);
			bool HasName(etk::UString& fileName) { return fileName==m_fileName; };
			void Increment(void) { m_counter++; };
			bool Decrement(void) { m_counter--; return (m_counter==0)?true:false; };
			draw::Image& Get(void) { return m_image; };
	};
};


#endif

