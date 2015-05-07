/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Sprite.h>

#undef __class__
#define __class__ "ewol::compositing::Sprite"

ewol::compositing::Sprite::Sprite(const std::string& _imageName, const ivec2& _nbSprite) :
  ewol::compositing::Image(_imageName),
  m_nbSprite(_nbSprite),
  m_unitarySpriteSize(0,0) {
	/*
	vec2 imageSize = getRealSize();
	m_unitarySpriteSize.setValue(imageSize.x()/(float)m_nbSprite.x(),
	                             imageSize.y()/(float)m_nbSprite.y());
	*/
	m_unitarySpriteSize.setValue(1.0/(float)m_nbSprite.x(),
	                             1.0/(float)m_nbSprite.y());
}


void ewol::compositing::Sprite::printSprite(const ivec2& _spriteID, const vec3& _size) {
	if(    _spriteID.x()<0
	    || _spriteID.y()<0
	    || _spriteID.x() >= m_nbSprite.x()
	    || _spriteID.y() >= m_nbSprite.y()) {
		return;
	}
	printPart(vec2(_size.x(),_size.y()),
	          vec2((float)(_spriteID.x()  )*m_unitarySpriteSize.x(), (float)(_spriteID.y()  )*m_unitarySpriteSize.y()),
	          vec2((float)(_spriteID.x()+1)*m_unitarySpriteSize.x(), (float)(_spriteID.y()+1)*m_unitarySpriteSize.y()));
}


