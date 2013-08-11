/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/renderer/resources/physicsShape/PhysicsConvexHull.h>



bool ewol::PhysicsConvexHull::Parse(const char* _line)
{
	if (true== ewol::PhysicsShape::Parse(_line)) {
		return true;
	}
	if(0==strncmp(_line, "points : ", 8) ) {
		EWOL_TODO("convex hull point parsing ...");
		return true;
	}
	if(0==strncmp(_line, "scale : ", 8) ) {
		sscanf(&_line[8], "%f", &m_scale );
		EWOL_DEBUG("                scale=" << m_scale);
		return true;
	}
	return false;
}
