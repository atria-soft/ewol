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
	vec2 imageSize = GetRealSize();
	m_unitarySpriteSize.setValue(imageSize.x()/(float)m_nbSprite.x(),
	                             imageSize.y()/(float)m_nbSprite.y());
	
}


void ewol::Sprite::PrintSprite(const ivec2& _spriteID, const vec2& _size)
{
	
}


