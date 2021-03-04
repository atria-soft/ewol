/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Sprite.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Sprite);

ewol::compositing::Sprite::Sprite( String _imageName,  Vector2i _nbSprite, int _size) :
  ewol::compositing::Image(_imageName, false, _size),
  this.nbSprite(_nbSprite),
  this.unitarySpriteSize(0,0) {
	/*
	Vector2f imageSize = getRealSize();
	this.unitarySpriteSize.setValue(imageSize.x()/(float)this.nbSprite.x(),
	                             imageSize.y()/(float)this.nbSprite.y());
	*/
	this.unitarySpriteSize.setValue(1.0/(float)this.nbSprite.x(),
	                             1.0/(float)this.nbSprite.y());
}


void ewol::compositing::Sprite::printSprite( Vector2i _spriteID,  Vector3f _size) {
	if(    _spriteID.x()<0
	    || _spriteID.y()<0
	    || _spriteID.x() >= this.nbSprite.x()
	    || _spriteID.y() >= this.nbSprite.y()) {
		return;
	}
	printPart(Vector2f(_size.x(),_size.y()),
	          Vector2f((float)(_spriteID.x()  )*this.unitarySpriteSize.x(), (float)(_spriteID.y()  )*this.unitarySpriteSize.y()),
	          Vector2f((float)(_spriteID.x()+1)*this.unitarySpriteSize.x(), (float)(_spriteID.y()+1)*this.unitarySpriteSize.y()));
}


