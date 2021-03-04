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
				uint this.texId; //!< openGl textureID.
				#ifdef EWOL_USE_FBO
					uint this.texPboId; //!< openGl textureID.
				#endif
				// openGl Context propoerties :
				egami::Image this.data;
				//! Last loaded size in the system openGL
				Vector2f this.lastSize;
				//! some image are not square == > we need to sqared it to prevent some openGl api error the the displayable size is not all the time 0.0 . 1.0
				Vector2f this.realImageSize;
				// internal state of the openGl system :
				boolean this.loaded;
				int this.lastTypeObject;
				int this.lastSizeObject;
			protected:
				boolean this.repeat; //!< repeate mode of the image (repeat the image if out of range [0..1]
			public:
				/**
				 * @brief Set the repeate mode of the images if UV range is out of [0..1]
				 * @param[in] _value Value of the new repeate mode
				 */
				void setRepeat(boolean _value);
			protected:
				enum ewol::resource::TextureFilter this.filter; //!< Filter apply at the image when rendering it
			public:
				/**
				 * @brief Set the Filter mode to apply at the image when display with a scale (not 1:1 ratio)
				 * @param[in] _value Value of the new filter mode
				 */
				void setFilterMode(enum ewol::resource::TextureFilter _filter);
			// Public API:
			protected:
				void init( String _filename);
				void init();
				Texture();
			public:
				DECLARE_RESOURCE_FACTORY(Texture);
				 ~Texture();
			public:
				// You must set the size here, because it will be set in multiple of pow(2)
				void setImageSize(Vector2i _newSize);
				// Get the reference on this image to draw nomething on it ...
				 egami::Image get() {
					return this.data;
				};
				/**
				 * @brief Set the image in the texture system
				 * @note It will reize in square2 if needed by the system.
				 * @param[in] _image Image to set. (use @code set(etk::move(xxx)); @endcode )
				 */
				void set(egami::Image _image);
				// Flush the data to send it at the openGl system
				void flush();
				boolean updateContext();
				void removeContext();
				void removeContextToLate();
				 Vector2i getOpenGlSize()  {
					return this.data.getSize();
				};
				 Vector2f getUsableSize()  {
					return this.realImageSize;
				};
				uint getRendererId()  {
					return this.texId;
				};
		};
	}
}

