/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/physicsShape/PhysicsShape.h>
#include <ewol/physicsShape/PhysicsBox.h>
#include <ewol/physicsShape/PhysicsCapsule.h>
#include <ewol/physicsShape/PhysicsCone.h>
#include <ewol/physicsShape/PhysicsConvexHull.h>
#include <ewol/physicsShape/PhysicsCylinder.h>
#include <ewol/physicsShape/PhysicsSphere.h>


ewol::PhysicsShape* ewol::PhysicsShape::create(const std::string& _name) {
	ewol::PhysicsShape* tmpp = NULL;
	std::string name = to_lower(_name);
	if (name == "box") {
		tmpp = new ewol::PhysicsBox();
	} else if (name == "sphere") {
		tmpp = new ewol::PhysicsSphere();
	} else if (name == "cone") {
		tmpp = new ewol::PhysicsCone();
	} else if (name == "cylinder") {
		tmpp = new ewol::PhysicsCylinder();
	} else if (name == "capsule") {
		tmpp = new ewol::PhysicsCapsule();
	} else if (name == "convexhull") {
		tmpp = new ewol::PhysicsConvexHull();
	} else {
		EWOL_ERROR("Create an unknow element : '" << _name << "' availlable : [BOX,SPHERE,CONE,CYLINDER,CAPSULE,CONVEXHULL]");
		return NULL;
	}
	if (tmpp == NULL) {
		EWOL_ERROR("Allocation error for physical element : '" << _name << "'");
	}
	return tmpp;
}


bool ewol::PhysicsShape::parse(const char* _line) {
	if(0 == strncmp(_line, "origin : ", 9) ) {
		sscanf(&_line[9], "%f %f %f", &m_origin.m_floats[0], &m_origin.m_floats[1], &m_origin.m_floats[2] );
		EWOL_VERBOSE("                Origin=" << m_origin);
		return true;
	}
	if(0 == strncmp(_line, "rotate : ", 9) ) {
		sscanf(&_line[9], "%f %f %f %f", &m_quaternion.m_floats[0], &m_quaternion.m_floats[1], &m_quaternion.m_floats[2], &m_quaternion.m_floats[3] );
		EWOL_VERBOSE("                rotate=" << m_quaternion);
		return true;
	}
	return false;
}

