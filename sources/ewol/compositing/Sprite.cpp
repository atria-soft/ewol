/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <tinyXML/tinyxml.h>

#include <ewol/debug.h>
#include <ewol/compositing/Sprite.h>
#include <ewol/config.h>

#undef __class__
#define __class__	"ewol::Sprite"

ewol::Sprite::Sprite(const etk::UString& _imageName, const ivec2& _nbSprite) :
	ewol::Image(_imageName),
	m_nbSprite(_nbSprite),
	m_unitarySpriteSize(0,0)
{
	/*
	vec2 imageSize = GetRealSize();
	m_unitarySpriteSize.setValue(imageSize.x()/(float)m_nbSprite.x(),
	                             imageSize.y()/(float)m_nbSprite.y());
	*/
	m_unitarySpriteSize.setValue(1.0/(float)m_nbSprite.x(),
	                             1.0/(float)m_nbSprite.y());
	
}


void ewol::Sprite::PrintSprite(const ivec2& _spriteID, const vec2& _size)
{
	if(    _spriteID.x()<0
	    || _spriteID.y()<0
	    || _spriteID.x()>=m_nbSprite.x()
	    || _spriteID.y()>=m_nbSprite.y()) {
		return;
	}
	PrintPart(_size, vec2((float)(_spriteID.x()  )*m_unitarySpriteSize.x(), (float)(_spriteID.y()  )*m_unitarySpriteSize.y()),
	                 vec2((float)(_spriteID.x()+1)*m_unitarySpriteSize.x(), (float)(_spriteID.y()+1)*m_unitarySpriteSize.y()));
}


