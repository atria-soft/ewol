/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/oObject/OObject.h>


#undef __class__
#define __class__	"ewol::OObject"


ewol::OObject::OObject(void)
{
	m_hasClipping = false;
	m_scaling.x = 1.0;
	m_scaling.y = 1.0;
	m_clipping.x = 0;
	m_clipping.y = 0;
	m_clipping.w = 1024;
	m_clipping.h = 1024;
	m_nbLoadedTime = 1;
}


ewol::OObject::~OObject(void)
{
	
}

