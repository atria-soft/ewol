/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/game/Geometry.h>
#include <ewol/game/BoundingAABB.h>
#include <ewol/game/BoundingOBB.h>
#include <ewol/game/BoundingPlane.h>
#include <ewol/game/BoundingSphere.h>

game::Geometry::Geometry(game::MeshObject& object) :
	m_dynamic(false),
	m_object(object),
	m_bounding(NULL),
	m_mass(NULL)
{
	
}

game::Geometry::~Geometry(void)
{
	
}

void game::Geometry::SetStatic(void)
{
	m_dynamic = false;
}

void game::Geometry::SetDynamic(void)
{
	m_dynamic = true;
}

void game::Geometry::SetBoundingMode(game::boundingMode type)
{
	if (NULL != m_bounding) {
		if (m_bounding->GetType() != type) {
			delete(m_bounding);
			m_bounding = NULL;
		} else {
			return;
		}
	}
	/*
	switch(type)
	{
		default:
		case BoundingModeNone:
			break;
		case BoundingModePlane:
			EWOL_TODO("Implement plane...");
			break;
		case BoundingModeAABB:
			m_bounding = new game::BoundingAABB();
			break;
		case BoundingModeSphere:
			m_bounding = new game::BoundingSphere();
			break;
		case BoundingModeOBB:
			m_bounding = new game::BoundingOBB(m_object, m_property);
			break;
	}
	*/
}

void game::Geometry::BoundingUpdate(void)
{
	if (NULL != m_bounding) {
		//m_bounding->Update(m_object);
	}
}


