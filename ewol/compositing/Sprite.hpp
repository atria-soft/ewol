/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <ewol/compositing/Image.hpp>

namespace ewol {
	namespace compositing {
		class Sprite : public ewol::compositing::Image {
			protected:
				ivec2 m_nbSprite; //!< number of sprite in vertical and horizontal
				vec2 m_unitarySpriteSize; //!< size of a unique sprite
			public:
				Sprite(const std::string& _imageName,
				       const ivec2& _nbSprite,
				       int32_t _size=ewol::compositing::Image::sizeAuto);
				virtual ~Sprite() {};
				void printSprite(const ivec2& _spriteID, const vec2& _size) {
					printSprite(_spriteID, vec3(_size.x(), _size.y(),0));
				};
				void printSprite(const ivec2& _spriteID, const vec3& _size);
		};
	}
}

