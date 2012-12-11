/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */



#include <ewol/game/Element.h>

static int32_t uniqueId = 0;


game::Element::Element(void) :
	m_position(0,0,0),
	m_speed(0,0,0),
	m_orientation(0,0,0),
	m_uniqueId(uniqueId),
	m_groupId(0),
	m_type(0),
	m_visible(true),
	m_mass(0)
{
	uniqueId++;
}

game::Element::~Element(void)
{
	
}

