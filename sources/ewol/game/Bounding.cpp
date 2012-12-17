/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/game/Bounding.h>


game::Bounding::Bounding(boundingMode mode) :
	m_markedToUpdate(true),
	m_mode(mode)
{
	
}

game::Bounding::~Bounding(void)
{
	
}


