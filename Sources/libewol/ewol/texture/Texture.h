/**
 *******************************************************************************
 * @file ewol/texture/Texture.h
 * @brief ewol Texture loading system (header)
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

#ifndef __EWOL_TEXTURE_H__
#define __EWOL_TEXTURE_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <draw/Image.h>
#include <ewol/openGl.h>

namespace ewol {
	class Texture {
		private:
			uint32_t        m_uniqueId;
			// openGl Context propoerties :
			draw::Image     m_data;
			// OpenGl textureID :
			GLuint          m_texId;
			// some image are not square ==> we need to sqared it to prevent some openGl api error the the displayable size is not all the time 0.0 -> 1.0
			Vector2D<float> m_endPointSize;
			// internal state of the openGl system :
			bool            m_loaded;
		// Ewol internal API:
		public:
			void UpdateContext(void);
			void RemoveContext(void);
			void RemoveContextToLate(void);
		// middleware interface:
		public:
			GLuint GetId(void) { return m_texId; };
			Vector2D<float> GetUsableSize(void) { return m_endPointSize; };
		// Public API:
		public:
			Texture(void);
			~Texture(void);
			// you must set the size here, because it will be set in multiple of pow(2)
			void SetImageSize(Vector2D<int32_t> newSize);
			// get the reference on this image to draw nomething on it ...
			inline draw::Image& Get(void) { return m_data; };
			// Flush the data to send it at the OpenGl system
			void Flush(void);
	};
	
};

#endif

