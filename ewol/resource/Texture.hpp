/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <egami/Image.hpp>
#include <gale/resource/Texture.hpp>

//#define EWOL_USE_FBO 1

namespace ewol {
	namespace resource {
		enum class TextureFilter {
			nearest,
			linear
		};
		class Texture : public gale::Resource {
			protected:
				uint32_t m_texId; //!< openGl textureID.
				#ifdef EWOL_USE_FBO
					uint32_t m_texPboId; //!< openGl textureID.
				#endif
				// openGl Context propoerties :
				egami::Image m_data;
				//! Last loaded size in the system openGL
				vec2 m_lastSize;
				//! some image are not square == > we need to sqared it to prevent some openGl api error the the displayable size is not all the time 0.0 -> 1.0
				vec2 m_realImageSize;
				// internal state of the openGl system :
				bool m_loaded;
				int32_t m_lastTypeObject;
				int32_t m_lastSizeObject;
			protected:
				bool m_repeat; //!< repeate mode of the image (repeat the image if out of range [0..1]
			public:
				/**
				 * @brief Set the repeate mode of the images if UV range is out of [0..1]
				 * @param[in] _value Value of the new repeate mode
				 */
				void setRepeat(bool _value);
			protected:
				enum ewol::resource::TextureFilter m_filter; //!< Filter apply at the image when rendering it
			public:
				/**
				 * @brief Set the Filter mode to apply at the image when display with a scale (not 1:1 ratio)
				 * @param[in] _value Value of the new filter mode
				 */
				void setFilterMode(enum ewol::resource::TextureFilter _filter);
			// Public API:
			protected:
				void init(const etk::String& _filename);
				void init();
				Texture();
			public:
				DECLARE_RESOURCE_FACTORY(Texture);
				virtual ~Texture();
			public:
				// you must set the size here, because it will be set in multiple of pow(2)
				void setImageSize(ivec2 _newSize);
				// get the reference on this image to draw nomething on it ...
				inline egami::Image& get() {
					return m_data;
				};
				// flush the data to send it at the openGl system
				void flush();
				bool updateContext();
				void removeContext();
				void removeContextToLate();
				const ivec2& getOpenGlSize() const {
					return m_data.getSize();
				};
				const vec2& getUsableSize() const {
					return m_realImageSize;
				};
				uint32_t getRendererId() const {
					return m_texId;
				};
		};
	}
}

