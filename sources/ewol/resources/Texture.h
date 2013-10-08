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
#include <egami/Image.h>
#include <ewol/renderer/openGL.h>
#include <ewol/resources/Resource.h>

namespace ewol {
	class Texture : public ewol::Resource {
		protected:
			// openGl Context propoerties :
			egami::Image m_data;
			// openGl textureID :
			GLuint m_texId;
			// some image are not square  == > we need to sqared it to prevent some openGl api error the the displayable size is not all the time 0.0 -> 1.0
			vec2 m_endPointSize;
			// internal state of the openGl system :
			bool m_loaded;
		// Ewol internal API:
		public:
			void updateContext(void);
			void removeContext(void);
			void removeContextToLate(void);
		// middleware interface:
		public:
			GLuint getId(void) { return m_texId; };
			vec2 getUsableSize(void) { return m_endPointSize; };
		// Public API:
		protected:
			Texture(const etk::UString& _filename);
			Texture(void);
			~Texture(void);
		public:
			virtual const char* getType(void) { return "ewol::Texture"; };
			// you must set the size here, because it will be set in multiple of pow(2)
			void setImageSize(ivec2 newSize);
			// get the reference on this image to draw nomething on it ...
			inline egami::Image& get(void) { return m_data; };
			// flush the data to send it at the openGl system
			void flush(void);
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::Texture* keep(void);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(ewol::Texture*& _object);
	};
	
};

#endif

