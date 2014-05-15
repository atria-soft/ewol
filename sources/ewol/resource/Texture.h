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
#include <ewol/openGL/openGL.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace resource {
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
				void updateContext();
				void removeContext();
				void removeContextToLate();
			// middleware interface:
			public:
				GLuint getId() const {
					return m_texId;
				};
				const vec2& getUsableSize() const {
					return m_endPointSize;
				};
				const ivec2& getOpenGlSize() const {
					return m_data.getSize();
				};
			// Public API:
			protected:
				Texture(const std::string& _filename);
				Texture();
				~Texture();
			public:
				// you must set the size here, because it will be set in multiple of pow(2)
				void setImageSize(ivec2 newSize);
				// get the reference on this image to draw nomething on it ...
				inline egami::Image& get() {
					return m_data;
				};
				// flush the data to send it at the openGl system
				void flush();
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ewol::resource::Texture* keep();
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::Texture*& _object);
		};
	};
};

#endif

