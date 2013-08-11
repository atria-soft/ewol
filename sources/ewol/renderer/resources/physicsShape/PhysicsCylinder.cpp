/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/renderer/resources/physicsShape/PhysicsCylinder.h>


bool ewol::PhysicsCylinder::Parse(const char* _line)
{
	if (true== ewol::PhysicsShape::Parse(_line)) {
		return true;
	}
	if(0==strncmp(_line, "half-extents : ", 15) ) {
		sscanf(&_line[15], "%f %f %f", &m_size.m_floats[0], &m_size.m_floats[1], &m_size.m_floats[2] );
		EWOL_DEBUG("                halfSize=" << m_size);
		return true;
	}
	return false;
}
