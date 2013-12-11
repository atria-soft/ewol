/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_SPRITE_H__
#define __EWOL_COMPOSITING_SPRITE_H__

#include <ewol/debug.h>
#include <ewol/compositing/Image.h>
#include <ewol/resources/ResourceManager.h>

namespace ewol {
	namespace compositing {
		class Sprite : public ewol::compositing::Compose {
			protected:
				ivec2 m_nbSprite; //!< number of sprite in vertical and horizontal
				vec2 m_unitarySpriteSize; //!< size of a unique sprite
			public:
				Sprite(const std::string& _imageName, const ivec2& _nbSprite);
				virtual ~Sprite() {};
				void printSprite(const ivec2& _spriteID, const vec2& _size) { printSprite(_spriteID, vec3(_size.x(), _size.y(),0)); };
				void printSprite(const ivec2& _spriteID, const vec3& _size);
		};
	};
};

#endif

