/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURE_H__
#define __EWOL_TEXTURE_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <draw/Image.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/resources/Resource.h>

namespace ewol {
	class Texture : public ewol::Resource {
		protected:
			// openGl Context propoerties :
			draw::Image     m_data;
			// OpenGl textureID :
			GLuint          m_texId;
			// some image are not square ==> we need to sqared it to prevent some openGl api error the the displayable size is not all the time 0.0 -> 1.0
			etk::Vector2D<float> m_endPointSize;
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
			etk::Vector2D<float> GetUsableSize(void) { return m_endPointSize; };
		// Public API:
		public:
			Texture(etk::UString tmpName);
			~Texture(void);
			virtual const char* GetType(void) { return "ewol::Texture"; };
			// you must set the size here, because it will be set in multiple of pow(2)
			void SetImageSize(etk::Vector2D<int32_t> newSize);
			// get the reference on this image to draw nomething on it ...
			inline draw::Image& Get(void) { return m_data; };
			// Flush the data to send it at the OpenGl system
			void Flush(void);
	};
	
};

#endif

