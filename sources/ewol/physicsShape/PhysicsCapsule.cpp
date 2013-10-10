/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/physicsShape/PhysicsCapsule.h>



bool ewol::PhysicsCapsule::parse(const char* _line) {
	if (true ==  ewol::PhysicsShape::parse(_line)) {
		return true;
	}
	if(0 == strncmp(_line, "radius : ", 9) ) {
		sscanf(&_line[9], "%f", &m_radius );
		EWOL_VERBOSE("                radius=" << m_radius);
		return true;
	}
	if(0 == strncmp(_line, "height : ", 9) ) {
		sscanf(&_line[9], "%f", &m_height );
		EWOL_VERBOSE("                height=" << m_height);
		return true;
	}
	return false;
}
