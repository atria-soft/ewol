/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/game/BoundingAABB.h>
#include <etk/math/Matrix4.h>


game::BoundingAABB::BoundingAABB(void) :
	Bounding(game::BoundingModeAABB)
{
	
}

game::BoundingAABB::~BoundingAABB(void)
{
	
}

void game::BoundingAABB::Update(game::MeshObject& object)
{
	if (true==m_markedToUpdate) {
		/*
		mat4 transMat = etk::matRotate(property.m_angle) * etk::matTranslate(property.m_position);
		m_markedToUpdate=false;
		for (int32_t iii=0; iii<object.m_vertices.Size(); iii++) {
			vec3 point = transMat * object.m_vertices[iii];
			
		}
		*/
	}
}



