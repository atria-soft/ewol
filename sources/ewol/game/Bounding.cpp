/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/game/Bounding.h>
#include <ewol/game/BoundingAABB.h>


game::Bounding::Bounding(boundingMode mode) :
	m_mode(mode)
{
	
}

game::Bounding::~Bounding(void)
{
	
}


game::Bounding* game::CreateBounding(game::boundingMode mode)
{
	switch(mode) {
		case game::BoundingModeAABB:
			
			return new game::BoundingAABB();
		default:
			return NULL;
	}
}
