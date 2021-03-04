/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <ewol/compositing/Image.hpp>

namespace ewol {
	namespace compositing {
		class Sprite : public ewol::compositing::Image {
			protected:
				Vector2i this.nbSprite; //!< number of sprite in vertical and horizontal
				Vector2f this.unitarySpriteSize; //!< size of a unique sprite
			public:
				Sprite( String _imageName,
				        Vector2i _nbSprite,
				       int _size=ewol::compositing::Image::sizeAuto);
				 ~Sprite() {};
				void printSprite( Vector2i _spriteID,  Vector2f _size) {
					printSprite(_spriteID, Vector3f(_size.x(), _size.y(),0));
				};
				void printSprite( Vector2i _spriteID,  Vector3f _size);
		};
	}
}

